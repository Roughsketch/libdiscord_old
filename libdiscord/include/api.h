#pragma once

#include "external/json.hpp"

#include "api/api_channel.h"
#include "api/api_guild.h"
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
      NoContent = 204
    };

    enum APIKey : uint16_t
    {
      //  Gateway API Calls
      GetGateway,
      GetGatewayBot,

      //  Channel API Calls
      GetChannel,
      ModifyChannel,
      DeleteChannel,
      GetChannelMessages,
      GetChannelMessage,
      CreateMessage,
      CreateReaction,
      DeleteOwnReaction,
      DeleteUserReaction,
      GetReactions,
      DeleteAllReactions,
      EditMessage,
      DeleteMessage,
      BulkDeleteMessage,
      EditChannelPermissions,
      GetChannelInvites,
      CreateChannelInvite,
      DeleteChannelPermission,
      TriggerTypingIndicator,
      GetPinnedMessages,
      AddPinnedChannelMessage,
      DeletePinnedChannelMessage,
      GroupDMAddRecipient,
      GroupDMRemoveRecipient,

      //  Guild API Calls
      CreateGuild,
      GetGuild,
      ModifyGuild,
      DeleteGuild,
      GetGuildChannels,
      CreateGuildChannel,
      ModifyGuildChannelPositions,
      GetGuildMember,
      ListGuildMembers,
      AddGuildMember,
      ModifyGuildMember,
      ModifyCurrentUsersNick,
      AddGuildMemberRole,
      RemoveGuildMemberRole,
      RemoveGuildMember,
      GetGuildBans,
      CreateGuildBan,
      RemoveGuildBan,
      GetGuildRoles,
      CreateGuildRole,
      ModifyGuildRolePositions,
      ModifyGuildRole,
      DeleteGuildRole,
      GetGuildPruneCount,
      BeginGuildPrune,
      GetGuildVoiceRegions,
      GetGuildInvites,
      GetGuildIntegrations,
      CreateGuildIntegration,
      ModifyGuildIntegration,
      DeleteGuildIntegration,
      SyncGuildIntegration,
      GetGuildEmbed,
      ModifyGuildEmbed,

      //  Invite API Calls
      GetInvite,
      DeleteInvite,
      AcceptInvite,

      //  User API Calls
      GetCurrentUser,
      GetUser,
      ModifyCurrentUser,
      GetCurrentUserGuilds,
      LeaveGuild,
      GetUserDMs,
      CreateDM,
      CreateGroupDM,
      GetUsersConnections,

      //  Voice API Calls
      ListVoiceRegions,

      //  Webhook API Calls
      CreateWebhook,
      GetChannelWebhook,
      GetGuildWebhooks,
      GetWebhook,
      GetWebhookWithToken,
      ModifyWebhook,
      ModifyWebhookWithToken,
      DeleteWebhook,
      DeleteWebhookWithToken,
      ExecuteWebhook,
      ExecuteSlackCompatibleWebhook,
      ExecuteGithubCompatibleWebhook
    };



    void set_token(std::string token);
    std::string get_wss_url();
    nlohmann::json request(APIKey key, Snowflake major, RequestType type, std::string endpoint, nlohmann::json data = {});
  }
}
