#include "bot.h"

#include "api.h"
#include "channel.h"
#include "common.h"
#include "emoji.h"
#include "event/event_message.h"
#include "gateway.h"
#include "guild.h"
#include "member.h"
#include "role.h"
#include "user.h"

namespace ModDiscord
{
  Bot::Bot()
  {
    m_is_user = false;

    m_on_message = [](std::shared_ptr<MessageEvent>) {};
    m_on_message_edited = [](std::shared_ptr<MessageEvent>) {};
    m_on_emoji_created = [](std::shared_ptr<Emoji>) {};
    m_on_emoji_deleted = [](std::shared_ptr<Emoji>) {};
    m_on_emoji_updated = [](std::shared_ptr<Emoji>) {};
  }

  std::shared_ptr<Bot> Bot::create(nlohmann::json settings)
  {
    auto bot = std::make_shared<Bot>();

    set_from_json(bot->m_client_id, "client_id", settings);
    set_from_json(bot->m_is_user, "user_account", settings);

    std::string token;
    set_from_json(token, "token", settings);

    if (!bot->m_is_user)
    {
      token = "Bot " + token;
    }

    ModDiscord::API::set_token(token);

    bot->m_gateway = std::make_shared<Gateway>(token);
    bot->m_gateway->set_bot(bot); //  Let the gateway know about the bot so it can send events.

    return bot;
  }

  void Bot::run() const
  {
    m_gateway->start();
  }

  std::shared_ptr<User> Bot::profile() const
  {
    return m_self;
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
    LOG(INFO) << "Bot.handle_dispatch entered with " << event_name.c_str() << ".";

    if (event_name == "READY")
    {
      set_from_json(m_self, "user", data);
      set_from_json(m_private_channels, "private_channels", data);
    }
    else if (event_name == "CHANNEL_CREATE" || event_name == "CHANNEL_UPDATE")
    {
      ModDiscord::API::Channel::update_cache(std::make_shared<Channel>(data));
    }
    else if (event_name == "CHANNEL_DELETE")
    {
      ModDiscord::API::Channel::remove_cache(std::make_shared<Channel>(data));
    }
    else if (event_name == "GUILD_CREATE")
    {
      auto guild = ModDiscord::API::Guild::update_cache(std::make_shared<Guild>(data));
      m_guilds.push_back(guild);
    }
    else if (event_name == "GUILD_UPDATE")
    {
      auto guild = ModDiscord::API::Guild::update_cache(std::make_shared<Guild>(data));
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
        ModDiscord::API::Guild::mark_unavailable(id);
      }
      else
      {
        //  The user was removed from the guild, remove it from our cache.
        ModDiscord::API::Guild::remove_cache(id);
      }
    }
    else if (event_name == "GUILD_BAN_ADD")
    {
      auto user = std::make_shared<User>(data);
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"].get<Snowflake>());
      LOG(INFO) << "User " << user->distinct() << " has been banned from " << guild->name() << ".";
    }
    else if (event_name == "GUILD_BAN_REMOVE")
    {
      auto user = std::make_shared<User>(data);
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"].get<Snowflake>());
      LOG(INFO) << "User " << user->distinct() << " has been unbanned from " << guild->name();
    }
    else if (event_name == "GUILD_EMOJIS_UPDATE")
    {
      m_threads.push_back(std::async(std::launch::async, [&](nlohmann::json json) {
        update_emojis(json);
      }, data));
    }
    else if (event_name == "GUILD_INTEGRATIONS_UPDATE")
    {
      LOG(INFO) << "Got a Guild Integrations Update, but left it unhandled.";
    }
    else if (event_name == "GUILD_MEMBER_ADD")
    {
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"]);
      guild->add_member(data);
    }
    else if (event_name == "GUILD_MEMBER_REMOVE")
    {
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"]);
      guild->remove_member(data);
    }
    else if (event_name == "GUILD_MEMBER_UPDATE")
    {
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"]);

      auto roles = data["roles"].get<std::vector<Snowflake>>();
      auto user = data["user"].get<User>();
      auto nick = data["nick"].get<std::string>();

      guild->update_member(roles, user, nick);
    }
    else if (event_name == "GUILD_MEMBERS_CHUNK")
    {
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"]);
      auto members = data["members"].get<std::vector<Member>>();

      for (auto& member : members)
      {
        guild->add_member(member);
      }
    }
    else if (event_name == "GUILD_ROLE_CREATE")
    {
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"]);
      guild->add_role(data["role"].get<Role>());
    }
    else if (event_name == "GUILD_ROLE_UPDATE")
    {
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"]);
      guild->update_role(data["role"].get<Role>());
    }
    else if (event_name == "GUILD_ROLE_DELETE")
    {
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"]);
      guild->remove_role(data["role"].get<Snowflake>());
    }
    else if (event_name == "MESSAGE_CREATE")
    {
      m_threads.push_back(std::async(std::launch::async, [&](std::shared_ptr<MessageEvent> msg, OnMessageCallback callback) {
        LOG(DEBUG) << "Got message: " << msg->content();
        callback(msg);
      }, std::make_shared<MessageEvent>(data), m_on_message));
    }
    else if (event_name == "MESSAGE_UPDATE")
    {
      m_threads.push_back(std::async(std::launch::async, [&](std::shared_ptr<MessageEvent> msg, OnMessageCallback callback) {
        LOG(DEBUG) << "Got edited message: " << msg->content();
        callback(msg);
      }, std::make_shared<MessageEvent>(data), m_on_message_edited));
    }
    else if (event_name == "MESSAGE_DELETE")
    {
      m_threads.push_back(std::async(std::launch::async, [&](std::shared_ptr<MessageDeletedEvent> msg, OnMessageDeletedCallback callback) {
        LOG(DEBUG) << "Got deleted message: " << msg->id().to_string();
        callback(msg);
      }, std::make_shared<MessageDeletedEvent>(data), m_on_message_deleted));
    }
    else if (event_name == "MESSAGE_DELETE_BULK")
    {
      auto ids = data["ids"].get<std::vector<Snowflake>>();
      auto chan_id = data["channel_id"].get<Snowflake>();

      LOG(INFO) << "Sending out " << ids.size() << " MessageDeletedEvents";

      for (auto& id : ids)
      {
        m_threads.push_back(std::async(std::launch::async, [&](std::shared_ptr<MessageDeletedEvent> msg, OnMessageDeletedCallback callback) {
          LOG(DEBUG) << "Got deleted message: " << msg->id().to_string();
          callback(msg);
        }, std::make_shared<MessageDeletedEvent>(id, chan_id), m_on_message_deleted));
      }
    }
    else if (event_name == "PRESENCE_UPDATE")
    {

    }
    else if (event_name == "TYPING_START")
    {

    }
    else if (event_name == "USER_SETTINGS_UPDATE")
    {

    }
    else if (event_name == "USER_UPDATE")
    {

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

  void Bot::update_emojis(nlohmann::json data)
  {
    auto guild = API::Guild::get_guild(data["guild_id"].get<Snowflake>());
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
      m_on_emoji_created(emoji);
    }

    for (auto& emoji : deleted_emoji)
    {
      LOG(TRACE) << "Sending Emoji Deleted event with emoji " << emoji->name();
      m_on_emoji_deleted(emoji);
    }

    for (auto& emoji : updated_emoji)
    {
      LOG(TRACE) << "Sending Emoji Updated event with emoji " << emoji->name();
      m_on_emoji_updated(emoji);
    }
  }
}
