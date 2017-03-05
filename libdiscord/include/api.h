#pragma once

#include "external/json.hpp"

#include "api_exceptions.h"
#include "common.h"

namespace Discord
{
  namespace API
  {
    enum RequestType : uint8_t
    {
      GET,
      POST,
      PUT,
      DEL,
      PATCH
    };

    enum Status : uint16_t
    {
      OK = 200,
      Created = 201,
      NoContent = 204
    };

    enum ResponseCode : uint32_t
    {
      UnknownAccount = 10001,
      UnknownApplication,
      UnknownChannel,
      UnknownGuild,
      UnknownIntegration,
      UnknownInvite,
      UnknownMember,
      UnknownMessage,
      UnknownOverwrite,
      UnknownProvider,
      UnknownRole,
      UnknownToken,
      UnknownUser,
      UnknownEmoji,
      UserOnlyEndpoint = 20001,
      BotOnlyEndpoint,
      MaxGuildsReached = 30001,
      MaxFriendsReached,
      MaxPinsReached,
      MaxGuildRolesReached = 30005,
      TooManyReactions = 30010,
      Unauthorized = 40001,
      MissingAccess = 50001,
      InvalidAccountType,
      CannotExecuteOnDM,
      EmbedDisabled,
      CannotEditAnotherUser,
      CannotSendEmptyMessage,
      CannotSendMessageToUser,
      CannotSendMessagesToVoiceChannel,
      ChannelVerificationTooHigh,
      OAuth2DoesNotHaveBot,
      OAuth2AppLimitReached,
      InvalidOAuth2State,
      MissingPermissions,
      InvalidAuthToken,
      NoteTooLong,
      TooFewOrTooManyMessagesToDelete,
      InvalidChannelForPin = 50019,
      TooOldToDelete = 50034,
      ReactionBlocked = 90001
    };

    class APICall
    {
      Snowflake m_major;
      std::string m_key;
      std::string m_endpoint;
    public:
      APICall() {};
      APICall(Snowflake major) : m_major(major) {};
      APICall& operator<<(const Snowflake& id)
      {
        m_endpoint += "/" + id.to_string();
        m_key += "id";

        return *this;
      }

      APICall& operator<<(const std::string& value)
      {
        m_endpoint += "/" + value;
        m_key += value;
        return *this;
      }

      std::string endpoint() const
      {
        return m_endpoint;
      }

      size_t hash() const
      {
        return std::hash<std::string>()(m_key + m_major.to_string());
      }
    };

    void set_token(std::string token);
    std::string get_wss_url();
    nlohmann::json request(APICall& key, RequestType type, nlohmann::json data = {});
  }
}
