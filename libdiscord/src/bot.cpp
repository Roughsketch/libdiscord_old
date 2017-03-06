#include "bot.h"

#include "api.h"
#include "api/api_channel.h"
#include "api/api_guild.h"
#include "api/api_user.h"
#include "channel.h"
#include "common.h"
#include "emoji.h"
#include "events.h"
#include "event/event_message.h"
#include "gateway.h"
#include "guild.h"
#include "member.h"
#include "role.h"
#include "user.h"

namespace Discord
{
  Bot::Bot()
  {
    m_is_user = false;

    m_on_message = nullptr;
    m_on_message_edited = nullptr;
    m_on_emoji_created = nullptr;
    m_on_emoji_deleted = nullptr;
    m_on_emoji_updated = nullptr;
    m_on_typing = nullptr;
    m_on_presence = nullptr;
  }

  std::shared_ptr<Bot> Bot::create(nlohmann::json settings)
  {
    auto bot = std::make_shared<Bot>();

    set_from_json(bot->m_client_id, "client_id", settings);
    set_from_json(bot->m_is_user, "user_account", settings);
    set_from_json(bot->m_prefix, "prefix", settings);

    std::string token;
    set_from_json(token, "token", settings);

    if (!bot->m_is_user)
    {
      token = "Bot " + token;
    }

    Discord::API::set_token(token);

    bot->m_gateway = std::make_shared<Gateway>(token);
    bot->m_gateway->set_bot(bot); //  Let the gateway know about the bot so it can send events.

    return bot;
  }

  std::shared_ptr<Bot> Bot::create(std::string token, std::string prefix)
  {
    return create({ 
      { "token", token },
      { "prefix", prefix } 
    });
  }

  std::shared_ptr<Bot> Bot::create_from_json(std::string filename)
  {
    auto settings = Discord::read_json_file(filename);

    if (settings.empty())
    {
      LOG(ERROR) << "Settings file " << filename << " is either invalid JSON or empty.";
      return nullptr;
    }

    return create(settings);
  }

  void Bot::run() const
  {
    m_gateway->start();

    for (;;)
    {
      std::this_thread::sleep_for(std::chrono::seconds(100000));
    }
  }

  void Bot::run_async() const
  {
    m_gateway->start();
  }

  std::shared_ptr<User> Bot::profile() const
  {
    return m_self;
  }

  std::string Bot::prefix() const
  {
    return m_prefix;
  }

  std::string Bot::invite_url() const
  {
    return "https://discordapp.com/oauth2/authorize?client_id=" + profile()->id().to_string() + "&scope=bot";
  }

  std::vector<std::shared_ptr<Guild>> Bot::guilds() const
  {
    return m_guilds;
  }

  void Bot::handle_dispatch(std::string event_name, nlohmann::json data)
  {
    //LOG(INFO) << "Bot.handle_dispatch entered with " << event_name.c_str() << ".";

    if (event_name == "READY")
    {
      set_from_json(m_self, "user", data);
      set_from_json(m_private_channels, "private_channels", data);
    }
    else if (event_name == "CHANNEL_CREATE" || event_name == "CHANNEL_UPDATE")
    {
      auto channel = std::make_shared<Channel>(data);
      auto guild_id = channel->guild_id();

      Discord::API::Channel::update_cache(channel);

      auto owner = std::find_if(std::begin(m_guilds), std::end(m_guilds), [guild_id](std::shared_ptr<Guild> guild)
      {
        return guild->id() == guild_id;
      });

      if (owner == std::end(m_guilds))
      {
        LOG(ERROR) << "Tried to add a channel from a non-existent guild.";
      }
      else
      {
        owner->get()->add_channel(channel);
      }
    }
    else if (event_name == "CHANNEL_DELETE")
    {
      auto channel = std::make_shared<Channel>(data);
      auto guild_id = channel->guild_id();

      Discord::API::Channel::remove_cache(channel);

      auto owner = std::find_if(std::begin(m_guilds), std::end(m_guilds), [guild_id](std::shared_ptr<Guild> guild)
      {
        return guild->id() == guild_id;
      });

      if (owner == std::end(m_guilds))
      {
        LOG(ERROR) << "Tried to remove a channel from a non-existent guild.";
      }
      else
      {
        owner->get()->remove_channel(channel);
      }
    }
    else if (event_name == "GUILD_CREATE")
    {
      auto guild = Discord::API::Guild::update_cache(std::make_shared<Guild>(data));
      m_guilds.push_back(guild);
    }
    else if (event_name == "GUILD_UPDATE")
    {
      auto guild = Discord::API::Guild::update_cache(std::make_shared<Guild>(data));
      auto itr = std::find_if(std::begin(m_guilds), std::end(m_guilds), [guild](std::shared_ptr<Guild> g) { return g->id() == guild->id(); });
      
      if (itr == std::end(m_guilds))
      {
        LOG(WARNING) << "Attempting to update a guild that doesn't exist. Simply adding it instead.";
        m_guilds.push_back(guild);
      }

      auto old_guild = *itr;
      old_guild->merge(guild);
    }
    else if (event_name == "GUILD_DELETE")
    {
      Snowflake id;
      set_from_json(id, "id", data);

      if (data.count("unavailable"))
      {
        //  The guild is just unavailable, mark it as such.
        Discord::API::Guild::mark_unavailable(id);
      }
      else
      {
        //  The user was removed from the guild, remove it from our cache and list.
        Discord::API::Guild::remove_cache(id);

        m_guilds.erase(std::remove_if(std::begin(m_guilds), std::end(m_guilds), [id](std::shared_ptr<Guild> guild)
        {
          return guild->id() == id;
        }));
      }
    }
    else if (event_name == "GUILD_BAN_ADD")
    {
      auto user = std::make_shared<User>(data);
      auto guild = Discord::API::Guild::get(data["guild_id"].get<Snowflake>());
      LOG(DEBUG) << "User " << user->distinct() << " has been banned from " << guild->name() << ".";
    }
    else if (event_name == "GUILD_BAN_REMOVE")
    {
      auto user = std::make_shared<User>(data);
      auto guild = Discord::API::Guild::get(data["guild_id"].get<Snowflake>());
      LOG(DEBUG) << "User " << user->distinct() << " has been unbanned from " << guild->name();
    }
    else if (event_name == "GUILD_EMOJIS_UPDATE")
    {
      m_threads.push_back(std::async(std::launch::async, [&](nlohmann::json json) {
        update_emojis(json);
      }, data));
    }
    else if (event_name == "GUILD_INTEGRATIONS_UPDATE")
    {
      LOG(DEBUG) << "Got a Guild Integrations Update, but left it unhandled.";
    }
    else if (event_name == "GUILD_MEMBER_ADD")
    {
      auto guild = Discord::API::Guild::get(data["guild_id"]);
      guild->add_member(data);
    }
    else if (event_name == "GUILD_MEMBER_REMOVE")
    {
      auto guild = Discord::API::Guild::get(data["guild_id"]);
      guild->remove_member(data);
    }
    else if (event_name == "GUILD_MEMBER_UPDATE")
    {
      auto guild = Discord::API::Guild::get(data["guild_id"]);

      std::vector<Snowflake> roles;
      std::shared_ptr<User> user;
      std::string nick;

      set_from_json(roles, "roles", data);
      set_from_json(user, "user", data);
      set_from_json(nick, "nick", data);

      guild->update_member(roles, user, nick);
    }
    else if (event_name == "GUILD_MEMBERS_CHUNK")
    {
      auto guild = Discord::API::Guild::get(data["guild_id"]);
      auto members = data["members"].get<std::vector<std::shared_ptr<Member>>>();

      for (auto& member : members)
      {
        guild->add_member(member);
      }
    }
    else if (event_name == "GUILD_ROLE_CREATE")
    {
      auto guild = Discord::API::Guild::get(data["guild_id"]);
      guild->add_role(data["role"].get<Role>());
    }
    else if (event_name == "GUILD_ROLE_UPDATE")
    {
      auto guild = Discord::API::Guild::get(data["guild_id"]);
      guild->update_role(data["role"].get<Role>());
    }
    else if (event_name == "GUILD_ROLE_DELETE")
    {
      auto guild = Discord::API::Guild::get(data["guild_id"]);
      guild->remove_role(data["role"].get<Snowflake>());
    }
    else if (event_name == "MESSAGE_CREATE")
    {
      auto event = MessageEvent(data);
      auto word = event.content().substr(0, event.content().find_first_of(" \n"));

      //  If we have a prefix and it's the start of this message and it's a command
      if (!m_prefix.empty() &&
          word.size() > m_prefix.size() && 
          (word.compare(0, m_prefix.size(), m_prefix) == 0) &&
          m_commands.count(word.substr(m_prefix.size()))
        )
      {
        //  Call the command
        m_threads.push_back(std::async(std::launch::async, m_commands[word.substr(m_prefix.size())], event));
      }
      else if (m_on_message) 
      {
        //  Not a command, but if we have an OnMessage handler call that instead.
        m_threads.push_back(std::async(std::launch::async, m_on_message, event));
      }
    }
    else if (event_name == "MESSAGE_UPDATE")
    {
      if (m_on_message_edited)
      {
        m_threads.push_back(std::async(std::launch::async, m_on_message_edited, MessageEvent(data)));
      }
    }
    else if (event_name == "MESSAGE_DELETE")
    {
      if (m_on_message_deleted)
      {
        m_threads.push_back(std::async(std::launch::async, m_on_message_deleted, MessageDeletedEvent(data)));
      }
    }
    else if (event_name == "MESSAGE_DELETE_BULK")
    {
      auto ids = data["ids"].get<std::vector<Snowflake>>();
      auto chan_id = data["channel_id"].get<Snowflake>();

      LOG(DEBUG) << "Sending out " << ids.size() << " MessageDeletedEvents";

      if (m_on_message_deleted)
      {
        for (auto& id : ids)
        {
          m_threads.push_back(std::async(std::launch::async, m_on_message_deleted, MessageDeletedEvent(id, chan_id)));
        }
      }
    }
    else if (event_name == "PRESENCE_UPDATE")
    {
      auto presence = std::make_shared<PresenceUpdate>(data);
      auto guild = Discord::API::Guild::get(data["guild_id"]);

      guild->update_presence(presence);
    }
    else if (event_name == "TYPING_START")
    {
      if (m_on_typing)
      {
        m_threads.push_back(std::async(std::launch::async, m_on_typing, TypingEvent(data)));
      }
    }
    else if (event_name == "VOICE_STATE_UPDATE")
    {

    }
    else if (event_name == "VOICE_SERVER_UPDATE")
    {

    }

    //  If we can lock the thread array, then do so.
    if (m_thread_mutex.try_lock())
    {
      std::lock_guard<std::mutex> lock(m_thread_mutex, std::adopt_lock);

      //  Remove all futures from the array which have finished.
      m_threads.erase(
        std::remove_if(std::begin(m_threads), std::end(m_threads),
          [](const std::future<void>& f) {
            using namespace std::chrono;
            return f.wait_until(system_clock::now() + 1ms) == std::future_status::ready;
      }), std::end(m_threads));
    }
  }

  void Bot::on_message(std::function<void(MessageEvent)> callback)
  {
    m_on_message = callback;
  }

  void Bot::on_message_edited(std::function<void(MessageEvent)> callback)
  {
    m_on_message_edited = callback;
  }

  void Bot::on_message_deleted(std::function<void(MessageDeletedEvent)> callback)
  {
    m_on_message_deleted = callback;
  }

  void Bot::on_emoji_created(std::function<void(Emoji)> callback)
  {
    m_on_emoji_created = callback;
  }

  void Bot::on_emoji_deleted(std::function<void(Emoji)> callback)
  {
    m_on_emoji_deleted = callback;
  }

  void Bot::on_emoji_updated(std::function<void(Emoji)> callback)
  {
    m_on_emoji_updated = callback;
  }

  void Bot::on_typing(std::function<void(TypingEvent)> callback)
  {
    m_on_typing = callback;
  }

  void Bot::on_presence(std::function<void(PresenceUpdate)> callback)
  {
    m_on_presence = callback;
  }

  void Bot::add_command(std::string command, std::function<void(MessageEvent)> callback)
  {
    m_commands[command] = callback;
  }

  void Bot::update_emojis(nlohmann::json data)
  {
    auto guild = API::Guild::get(data["guild_id"].get<Snowflake>());
    auto new_emojis = data["emojis"].get<std::vector<std::shared_ptr<Emoji>>>();
    auto old_emojis = guild->emojis();

    //  Must sort or set differences won't work properly.
    std::sort(std::begin(new_emojis), std::end(new_emojis));
    std::sort(std::begin(old_emojis), std::end(old_emojis));

    guild->set_emojis(new_emojis);

    std::vector<std::shared_ptr<Emoji>> added_emoji;
    std::vector<std::shared_ptr<Emoji>> deleted_emoji;
    std::vector<std::shared_ptr<Emoji>> updated_emoji;

    std::set_difference(std::begin(new_emojis), std::end(new_emojis), std::begin(old_emojis), std::end(old_emojis), std::back_inserter(added_emoji));
    std::set_difference(std::begin(old_emojis), std::end(old_emojis), std::begin(new_emojis), std::end(new_emojis), std::back_inserter(deleted_emoji));
    std::set_intersection(std::begin(old_emojis), std::end(old_emojis), std::begin(new_emojis), std::end(new_emojis), std::back_inserter(updated_emoji));

    updated_emoji.erase(std::remove_if(std::begin(updated_emoji), std::end(updated_emoji),
      [old_emojis](std::shared_ptr<Emoji> e) {
      auto other = std::find_if(std::begin(old_emojis), std::end(old_emojis),
        [e](std::shared_ptr<Emoji> a) {
        return a->id() == e->id();
      });

      if (other == std::end(old_emojis))
      {
        LOG(ERROR) << "Somehow a new emoji got into the updated list.";
        return false;
      }

      auto other_val = *other;

      //  Although data seems to be sent in sorted order, we need to be sure since vector compare requires sorted order to work.
      auto e_roles = e->roles();
      auto other_roles = other_val->roles();

      std::sort(std::begin(e_roles), std::end(e_roles));
      std::sort(std::begin(other_roles), std::end(other_roles));

      return (e->name() == other_val->name() && e_roles == other_roles);
    })
    );

    for (auto& emoji : added_emoji)
    {
      LOG(TRACE) << "Sending Emoji Created event with emoji " << emoji->name();
      m_on_emoji_created(*emoji);
    }

    for (auto& emoji : deleted_emoji)
    {
      LOG(TRACE) << "Sending Emoji Deleted event with emoji " << emoji->name();
      m_on_emoji_deleted(*emoji);
    }

    for (auto& emoji : updated_emoji)
    {
      LOG(TRACE) << "Sending Emoji Updated event with emoji " << emoji->name();
      m_on_emoji_updated(*emoji);
    }
  }
}
