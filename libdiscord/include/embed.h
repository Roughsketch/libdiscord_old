#pragma once

#include "common.h"

namespace Discord
{
  /** The footer object of an embed. */
  class EmbedFooter
  {
    std::string m_text;
    std::string m_icon_url;
    std::string m_proxy_icon_url;
  public:
    explicit EmbedFooter(const nlohmann::json& data);
    explicit EmbedFooter(std::string text = "", std::string icon_url = "", std::string proxy_icon_url = "");

    /** Get the text from this footer.
     
        @return The text of the footer.
     */
    std::string content() const;
    
    /** Get the icon url.
     
        @return The icon url.
     */
    std::string icon_url() const;

    /** Get the proxy icon url.
     
        @return The proxy icon url.
     */
    std::string proxy_icon_url() const;
  };

  void from_json(const nlohmann::json& json, EmbedFooter& footer);
  void to_json(nlohmann::json& json, const EmbedFooter& footer);

  /** An image object of an embed. */
  class EmbedImage
  {
    std::string m_url;
    std::string m_proxy_url;
    uint32_t m_height;
    uint32_t m_width;
  public:
    EmbedImage();
    explicit EmbedImage(const nlohmann::json& data);
    explicit EmbedImage(std::string url = "", uint32_t width = 0, uint32_t height = 0, std::string proxy_url = "");

    /** Get the URL of the image.
     
        @return The URL of the image.
     */
    std::string url() const;

    /** Get the proxied URL of the image.
     
        @return The proxied URL of the image.
     */
    std::string proxy_url() const;
    
    /** Get the width of the image.
     
        @return The width of the image
     */
    uint32_t width() const;

    /** Get the height of the image.
     
        @return The height of the image.
     */
    uint32_t height() const;
  };

  void from_json(const nlohmann::json& json, EmbedImage& image);
  void to_json(nlohmann::json& json, const EmbedImage& image);

  //  EmbedImage and EmbedThumbnail objects are identical, so just typedef it.
  typedef EmbedImage EmbedThumbnail;

  /** A video object of an embed. */
  class EmbedVideo
  {
    std::string m_url;
    uint32_t m_height;
    uint32_t m_width;
  public:
    EmbedVideo();
    explicit EmbedVideo(const nlohmann::json& data);
    explicit EmbedVideo(std::string url = "", uint32_t width = 0, uint32_t height = 0);

    /** Get the URL of the video.
     
        @return The URL of the video.
     */
    std::string url() const;

    /** Get the width of the video.
     
        @return The width of the video.
     */
    uint32_t width() const;

    /** Get the height of the video.
     
        @return The height of the video.
     */
    uint32_t height() const;
  };

  void from_json(const nlohmann::json& json, EmbedVideo& video);
  void to_json(nlohmann::json& json, const EmbedVideo& video);

  /** Wraps embed provider information */
  class EmbedProvider
  {
    std::string m_name;
    std::string m_url;
  public:
    explicit EmbedProvider(const nlohmann::json& data);
    explicit EmbedProvider(std::string name = "", std::string url = "");

    /** Get the name of the provider.
     
        @return The name of the provider.
     */
    std::string name() const;

    /** Get the URL of the provider.
     
        @return The URL of the provider.
     */
    std::string url() const;
  };

  void from_json(const nlohmann::json& json, EmbedProvider& provider);
  void to_json(nlohmann::json& json, const EmbedProvider& provider);

  /** Holds information on the listed author of an embed. */
  class EmbedAuthor
  {
    std::string m_name;
    std::string m_url;
    std::string m_icon_url;
    std::string m_proxy_icon_url;
  public:
    explicit EmbedAuthor(const nlohmann::json& data);
    explicit EmbedAuthor(std::string name = "", std::string url = "", std::string icon_url = "", std::string proxy_icon_url = "");

    /** Get the name of the author.
     
        @return The name of the author.
     */
    std::string name() const;

    /** Get the URL of the author.

        @return The URL of the author.
    */
    std::string url() const;

    /** Get the icon URL of the author.

        @return The icon URL of the author.
    */
    std::string icon_url() const;

    /** Get the proxied icon URL of the author.

        @return The proxied icon URL of the author.
    */
    std::string proxy_icon_url() const;
  };

  void from_json(const nlohmann::json& json, EmbedAuthor& author);
  void to_json(nlohmann::json& json, const EmbedAuthor& author);

  /** Represents a field of an embed. */
  class EmbedField
  {
    std::string m_name;
    std::string m_value;
    bool m_inline;
  public:
    EmbedField();
    explicit EmbedField(const nlohmann::json& data);
    explicit EmbedField(std::string name = "", std::string value = "", bool is_inline = false);

    /** Get the name of the field.
     
        @return The name of the field.
     */
    std::string name() const;

    /** Get the content of the field.
     
        @return The content of the field.
     */
    std::string content() const;

    /** Whether this field is inline or not.
     
        @return Whether this field is inline or not.
     */
    bool is_inline() const;
  };

  void from_json(const nlohmann::json& json, EmbedField& field);
  void to_json(nlohmann::json& json, const EmbedField& field);

  /** Represents a Discord embed object. */
  class Embed
  {
    std::string m_title;
    std::string m_type;
    std::string m_description;
    std::string m_url;
    std::string m_timestamp;
    uint32_t m_color;
    std::shared_ptr<EmbedFooter> m_footer;
    std::shared_ptr<EmbedImage> m_image; 
    std::shared_ptr<EmbedThumbnail> m_thumbnail; 
    std::shared_ptr<EmbedVideo> m_video; 
    std::shared_ptr<EmbedProvider> m_provider; 
    std::shared_ptr<EmbedAuthor> m_author;
    std::vector<std::shared_ptr<EmbedField>> m_fields;
  public:
    Embed();
    explicit Embed(const nlohmann::json& data);

    /** Sets the title of the embed.
     
        @param title The title for this embed.
     */
    void set_title(std::string title);

    /** Sets the description of the embed.
     
        @param description The description for this embed.
     */
    void set_description(std::string description);

    /** Sets the url of the embed.

    @param url The url for this embed.
    */
    void set_url(std::string url);

    /** Sets the footer of the embed.

        @param text The text of the footer.
        @param icon_url The icon to use for the footer.
        @param proxy_icon_url
    */
    void set_footer(std::string text, std::string icon_url = "", std::string proxy_icon_url = "");

    /** Sets the image of the embed.

        @param url A URL to the image.
    */
    void set_image(std::string url);

    /** Sets the thumbnail of the embed.

        @param url A URL to the thumbnail.
    */
    void set_thumbnail(std::string url);

    /** Sets the video of the embed.

        @param url A URL to the video.
    */
    void set_video(std::string url);

    /** Sets the author of the embed.

        @param name The name of the author.
        @param url A URL to the author.
        @param icon_url A URL to an icon to use for the author.
    */
    void set_author(std::string name, std::string url = "", std::string icon_url = "");
    
    /** Adds a field to the embed.
     
        @param name The name of this field.
        @param value The content of this embed.
        @param is_inline Whether or not this field should be displayed inline or not.
     */
    void add_field(std::string name, std::string value = "", bool is_inline = false);

    std::string title() const;
    std::string type() const;
    std::string description() const;
    std::string url() const;
    std::string timestamp() const;
    uint32_t color() const;
    std::shared_ptr<EmbedFooter> footer() const;
    std::shared_ptr<EmbedImage> image() const;
    std::shared_ptr<EmbedThumbnail> thumbnail() const;
    std::shared_ptr<EmbedVideo> video() const;
    std::shared_ptr<EmbedProvider> provider() const;
    std::shared_ptr<EmbedAuthor> author() const;
    std::vector<std::shared_ptr<EmbedField>> fields() const;
  };

  void from_json(const nlohmann::json& json, Embed& embed);
  void to_json(nlohmann::json& json, const Embed& embed);
}