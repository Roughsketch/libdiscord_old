#include "api.h"
#include "api/api_channel.h"
#include "channel.h"
#include "emoji.h"
#include "invite.h"
#include "message.h"
#include "user.h"

#include <cpprest/http_msg.h>

namespace Discord
{
  namespace API
  {
    namespace Channel
    {
      static std::map<Snowflake, std::shared_ptr<Discord::Channel>> ChannelCache;

      void update_cache(std::shared_ptr<Discord::Channel> channel)
      {
        if (ChannelCache.count(channel->id()))
        {
          LOG(TRACE) << "Merging new channel information with cached value.";
          auto old = get_channel(channel->id());
          old->merge(channel);
        }
        else
        {
          ChannelCache[channel->id()] = channel;
        }
      }

      void remove_cache(std::shared_ptr<Discord::Channel> channel)
      {
        auto itr = ChannelCache.find(channel->id());

        if (itr != std::end(ChannelCache))
        {
          ChannelCache.erase(itr);
        }
        else
        {
          LOG(ERROR) << "Attempted to delete a channel that wasn't in the cache.";
        }
      }

      std::shared_ptr<Discord::Channel> get_channel(Snowflake channel_id)
      {
        if (ChannelCache.count(channel_id))
        {
          LOG(TRACE) << "Returning channel from cache.";
          return ChannelCache[channel_id];
        }

        auto json = request(GetChannel, channel_id, GET, "channels/" + channel_id.to_string());

        if (!json.empty())
        {
          auto chan = std::make_shared<Discord::Channel>(json);
          update_cache(chan);
          return chan;
        }

        LOG(ERROR) << "Could not get Channel object with id " << channel_id.to_string();
        return nullptr;
      }

      std::shared_ptr<Discord::Channel> modify_text_channel(Snowflake channel_id, std::string name, uint32_t position, std::string topic)
      {
        auto chan = get_channel(channel_id);

        auto response = request(ModifyChannel, channel_id, PATCH, "channels/" + channel_id.to_string(), {
          { "name", name },
          { "position", position },
          { "topic", topic }
        });

        if (response["response_status"].get<int>() != Status::OK)
        {
          LOG(ERROR) << "Modify text channel returned bad response (" << response["response_status"].get<int>() << ")";
        }

        return chan;
      }

      std::shared_ptr<Discord::Channel> modify_voice_channel(Snowflake channel_id, std::string name, uint32_t position, uint32_t bitrate, uint32_t user_limit)
      {
        auto chan = get_channel(channel_id);

        auto response = request(ModifyChannel, channel_id, PATCH, "channels/" + channel_id.to_string(), {
          { "name", name },
          { "position", position },
          { "bitrate", bitrate },
          { "user_limit", user_limit }
        });

        if (response["response_status"].get<int>() != Status::OK)
        {
          LOG(ERROR) << "Modify voice channel returned bad response (" << response["response_status"].get<int>() << ")";
        }

        return chan;
      }

      std::shared_ptr<Discord::Channel> delete_channel(Snowflake channel_id)
      {
        auto response = request(DeleteChannel, channel_id, DEL, "channels/" + channel_id.to_string());

        return std::make_shared<Discord::Channel>(response);
      }

      std::vector<std::shared_ptr<Message>> get_messages(Snowflake channel_id, int32_t limit, SearchCriteria method, Snowflake pivot)
      {
        nlohmann::json payload = { { "limit", limit } };

        if (method != SearchCriteria::None && pivot == 0)
        {
          LOG(ERROR) << "Search method passed to get_messages, but pivot id is zero.";
          return std::vector<std::shared_ptr<Message>>();
        }

        switch (method)
        {
        case SearchCriteria::None:
          break;
        case SearchCriteria::Before:
          payload["before"] = pivot;
          break;
        case SearchCriteria::After:
          payload["after"] = pivot;
          break;
        case SearchCriteria::Around:
          payload["around"] = pivot;
          break;
        default:
          LOG(ERROR) << "Invalid search method passed to get_messages.";
          return std::vector<std::shared_ptr<Message>>();
        }

        auto response = request(GetChannelMessages, channel_id, GET, "channels/" + channel_id.to_string() + "/messages", payload);

        return response.get<std::vector<std::shared_ptr<Message>>>();
      }

      std::shared_ptr<Message> get_message(Snowflake channel_id, Snowflake message_id)
      {
        auto response = request(GetChannelMessage, channel_id, GET, "channels/" + channel_id.to_string() + "/messages/" + message_id.to_string());

        return std::make_shared<Message>(response);
      }

      std::shared_ptr<Message> create_message(Snowflake channel_id, std::string content, bool tts)
      {
        auto response = request(CreateMessage, channel_id, POST, "channels/" + channel_id.to_string() + "/messages", {
          { "content", content },
          { "mentions", nlohmann::json::array() },
          { "tts", tts },
          { "embed", nullptr }
        });

        return std::make_shared<Message>(response);
      }

      bool create_reaction(Snowflake channel_id, Snowflake message_id, std::shared_ptr<Emoji> emoji)
      {
        auto response = request(CreateReaction, channel_id, PUT, 
          "channels/" + channel_id.to_string() + "/messages/" + message_id.to_string() + "/reactions/" + emoji->name());

        return response["response_status"].get<int>() == Status::NoContent;
      }

      bool delete_own_reaction(Snowflake channel_id, Snowflake message_id, std::shared_ptr<Emoji> emoji)
      {
        auto response = request(DeleteOwnReaction, channel_id, DEL, 
          "channels/" + channel_id.to_string() + "/messages/" + message_id.to_string() + "/reactions/" + emoji->name() + "/@me");

        return response["response_status"].get<int>() == Status::NoContent;
      }

      bool delete_user_reaction(Snowflake channel_id, Snowflake message_id, std::shared_ptr<Emoji> emoji, Snowflake user_id)
      {
        auto response = request(DeleteUserReaction, channel_id, DEL,
          "channels/" + channel_id.to_string() + "/messages/" + message_id.to_string() + "/reactions/" + emoji->name() + "/" + user_id.to_string());

        return response["response_status"].get<int>() == Status::NoContent;
      }

      std::vector<std::shared_ptr<User>> get_reactions(Snowflake channel_id, Snowflake message_id, std::shared_ptr<Emoji> emoji)
      {
        auto response = request(GetReactions, channel_id, GET,
          "channels/" + channel_id.to_string() + "/messages/" + message_id.to_string() + "/reactions/" + emoji->name());

        return response.get<std::vector<std::shared_ptr<User>>>();
      }

      void delete_all_reactions(Snowflake channel_id, Snowflake message_id)
      {
        request(DeleteAllReactions, channel_id, DEL, 
          "channels/" + channel_id.to_string() + "/messages/" + message_id.to_string() + "/reactions");
      }

      std::shared_ptr<Message> edit_message(Snowflake channel_id, Snowflake message_id, std::string new_content)
      {
        auto response = request(EditMessage, channel_id, PATCH, "channels/" + channel_id.to_string() + "/messages/" + message_id.to_string());

        return response.get<std::shared_ptr<Message>>();
      }

      bool delete_message(Snowflake channel_id, Snowflake message_id)
      {
        auto response = request(DeleteMessage, channel_id, DEL, "channels/" + channel_id.to_string() + "/messages/" + message_id.to_string());
        return response["response_status"].get<int>() == Status::NoContent;
      }

      bool bulk_delete_messages(Snowflake channel_id, std::vector<Snowflake> message_ids)
      {
        auto response = request(DeleteMessage, channel_id, POST, "channels/" + channel_id.to_string() + "/messages/bulk-delete", {
          { "messages", message_ids }
        });

        return response["response_status"].get<int>() == Status::NoContent;
      }

      bool edit_permissions(Snowflake channel_id, std::shared_ptr<Overwrite> overwrite, uint32_t allow, uint32_t deny, std::string type)
      {
        auto response = request(EditChannelPermissions, channel_id, PUT, 
          "channels/" + channel_id.to_string() + "/permissions/" + overwrite->id().to_string(), {
            { "allow", allow },
            { "deny", deny },
            { "type", type }
        });
        return response["response_status"].get<int>() == Status::NoContent;
      }

      std::vector<std::shared_ptr<Invite>> get_channel_invites(Snowflake channel_id)
      {
        auto response = request(GetChannelInvites, channel_id, GET, "channels/" + channel_id.to_string() + "/invites");

        return response.get<std::vector<std::shared_ptr<Invite>>>();
      }

      std::shared_ptr<Invite> create_channel_invite(Snowflake channel_id, uint32_t max_age, uint32_t max_uses, bool temporary, bool unique)
      {
        auto response = request(CreateChannelInvite, channel_id, POST, "channels/" + channel_id.to_string() + "/invites", {
          { "max_age", max_age },
          { "max_uses", max_uses },
          { "temporary", temporary},
          { "unique", unique }
        });

        return response.get<std::shared_ptr<Invite>>();
      }

      bool delete_permission(Snowflake channel_id, std::shared_ptr<Overwrite> overwrite)
      {
        auto response = request(DeleteChannelPermission, channel_id, DEL, 
          "channels/" + channel_id.to_string() + "/permissions/" + overwrite->id().to_string());

        return response["response_status"].get<int>() == Status::NoContent;
      }

      bool trigger_typing_indicator(Snowflake channel_id)
      {
        auto response = request(TriggerTypingIndicator, channel_id, POST, "channels/" + channel_id.to_string() + "/typing");

        return response["response_status"].get<int>() == Status::NoContent;
      }

      std::vector<std::shared_ptr<Message>> get_pinned_messages(Snowflake channel_id)
      {
        auto response = request(GetPinnedMessages, channel_id, GET, "channels/" + channel_id.to_string() + "/pins");

        return response.get<std::vector<std::shared_ptr<Message>>>();
      }

      bool add_pinned_message(Snowflake channel_id, Snowflake message_id)
      {
        auto response = request(AddPinnedChannelMessage, channel_id, POST, "channels/" + channel_id.to_string() + "/pins/" + message_id.to_string());

        return response["response_status"].get<int>() == Status::NoContent;
      }

      bool delete_pinned_message(Snowflake channel_id, Snowflake message_id)
      {
        auto response = request(DeletePinnedChannelMessage, channel_id, DEL, "channels/" + channel_id.to_string() + "/pins/" + message_id.to_string());

        return response["response_status"].get<int>() == Status::NoContent;
      }

      void group_dm_add_recipient(Snowflake channel_id, Snowflake user_id, std::string access_token, std::string nickname)
      {
        request(GroupDMAddRecipient, channel_id, PUT, "channels/" + channel_id.to_string() + "/recipients/" + user_id.to_string(), {
          { "access_token", access_token },
          { "nick", nickname }
        });
      }

      void group_dm_remove_recipient(Snowflake channel_id, Snowflake user_id)
      {
        request(GroupDMAddRecipient, channel_id, DEL, "channels/" + channel_id.to_string() + "/recipients/" + user_id.to_string());
      }
    }
  }
}
