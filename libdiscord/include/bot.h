#pragma once

#include <future>
#include <memory>

#include "common.h"

namespace Discord
{
  class Channel;
  class Emoji;
  class Gateway;
  class Guild;
  class MessageEvent;
  class MessageDeletedEvent;
  class PresenceUpdate;
  class TypingEvent;
  class User;

  class Bot
  {
    Snowflake m_client_id;
    bool m_is_user;
    std::string m_prefix;
    std::shared_ptr<User> m_self;

    std::shared_ptr<Gateway> m_gateway;

    std::vector<std::shared_ptr<Guild>> m_guilds;
    std::vector<std::shared_ptr<Channel>> m_private_channels;

    std::vector<std::future<void>> m_threads;
    std::mutex m_thread_mutex;

    //  Callbacks for events
    std::function<void(MessageEvent)> m_on_message;
    std::function<void(MessageEvent)> m_on_message_edited;
    std::function<void(MessageDeletedEvent)> m_on_message_deleted;
    std::function<void(Emoji)> m_on_emoji_created;
    std::function<void(Emoji)> m_on_emoji_deleted;
    std::function<void(Emoji)> m_on_emoji_updated;
    std::function<void(TypingEvent)> m_on_typing;
    std::function<void(PresenceUpdate)> m_on_presence;

    std::map<std::string, std::function<void(MessageEvent)>> m_commands;

    void update_emojis(nlohmann::json data);
  public:
    explicit Bot();

    /** Used to create a Bot. Should be used over constructor.
    
        @param settings The settings with which to load the Bot. Includes token and other miscellaneous data.
        @return A shared pointer to the bot that was created.
     */
    static std::shared_ptr<Bot> create(nlohmann::json settings);

    /** Used to create a Bot with just a token.
     
        @param token The token for this bot.
        @param prefix The prefix that commands will use.
        @return A shared pointer to the bot that was created.
     */
    static std::shared_ptr<Bot> create(std::string token, std::string prefix = "");

    /** Used to create a Bot using settings loaded from a JSON file.
    
        The only required field for this file is "token", which will be your bot's token.

        @code
        {
          "token": "YOUR_TOKEN_HERE"
        }
        @endcode

        @param filename The JSON file to load settings from.
        @return A shared pointer to the bot that was created. If invalid settings are found it will return nullptr.
     */
    static std::shared_ptr<Bot> create_from_json(std::string filename);

    /** Starts the bot and loops forever. */
    void run() const;

    /** Starts a bot, but allows execution after the gateway is started. */
    void run_async() const;

    /** Get the Bot's user profile.
     
        @return The Bot's user profile.
     */
    std::shared_ptr<User> profile() const;

    /** Get the Bot's command prefix.
     
        @return The Bot's command prefix.
     */
    std::string prefix() const;

    /** Get the Bot's invite url.
     
        @return The Bot's invite url.
     */
    std::string invite_url() const;

    /** Get a Bot's current guilds.
     
        @return A list of guilds the bot is currently in.
     */
    std::vector<std::shared_ptr<Guild>> guilds() const;

    /** Called by the Gateway when an event occurs. Should not be called manually. */
    void handle_dispatch(std::string event_name, nlohmann::json data);

    /** Assign a callback for when a message is received. There may only be one callback at a time.
    
        @code
        bot->on_message([](MessageEvent event){
            if (event.content() == "Ping!") {
                event.respond("Pong!");
            }
        });
        @endcode

        @param callback The callback to call when a message is received.
     */
    void on_message(std::function<void(MessageEvent)> callback);

    /** Assign a callback for when a message is edited. There may only be one callback at a time.

    @code
    bot->on_message_edited([](MessageEvent event){
        event.respond("You can't hide your editing from me " + event.author()->distinct() + "!");
    });
    @endcode

    @param callback The callback to call when a message is edited.
    */
    void on_message_edited(std::function<void(MessageEvent)> callback);

    /** Assign a callback for when a message is deleted. There may only be one callback at a time.

        @code
        bot->on_message_edited([](MessageEvent event){
            event.respond("Message with id " + event.id().to_string() + " was deleted.");
        });
        @endcode

        @param callback The callback to call when a message is deleted.
    */
    void on_message_deleted(std::function<void(MessageDeletedEvent)> callback);

    /** Assign a callback for when an emoji is created. There may only be one callback at a time.

        @param callback The callback to call when an emoji is created.
    */
    void on_emoji_created(std::function<void(Emoji)> callback);

    /** Assign a callback for when an emoji is deleted. There may only be one callback at a time.

        @param callback The callback to call when an emoji is deleted.
    */
    void on_emoji_deleted(std::function<void(Emoji)> callback);

    /** Assign a callback for when an emoji is updated. There may only be one callback at a time.

        @param callback The callback to call when an emoji is updated.
    */
    void on_emoji_updated(std::function<void(Emoji)> callback);

    /** Assign a callback that is called whenever a user starts typing.
      
        @param callback The callback to call when a typing event is raised.
     */
    void on_typing(std::function<void(TypingEvent)> callback);

    /** Assign a callback that is called whenever a presence update is received.
     
        @param callback The callback to call when a presence update is received.
     */
    void on_presence(std::function<void(PresenceUpdate)> callback);

    /** Add a command to the bot. Requires the bot have a prefix.
     
        @param command The command name without the prefix.
        @param callback The callback to call when the command it issued.
     */
    void add_command(std::string command, std::function<void(MessageEvent)> callback);
  };
}
