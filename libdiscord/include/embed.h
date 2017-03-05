#pragma once

#include "common.h"

namespace Discord
{
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

    std::string url() const;
    std::string proxy_url() const;
    uint32_t width() const;
    uint32_t height() const;
  };

  void from_json(const nlohmann::json& json, EmbedImage& image);
  void to_json(nlohmann::json& json, const EmbedImage& image);

  typedef EmbedImage EmbedThumbnail;


  class EmbedVideo
  {
    std::string m_url;
    uint32_t m_height;
    uint32_t m_width;
  public:
    EmbedVideo();
    explicit EmbedVideo(const nlohmann::json& data);
    explicit EmbedVideo(std::string url = "", uint32_t width = 0, uint32_t height = 0);

    std::string url() const;
    uint32_t width() const;
    uint32_t height() const;
  };

  void from_json(const nlohmann::json& json, EmbedVideo& video);
  void to_json(nlohmann::json& json, const EmbedVideo& video);

  class EmbedProvider
  {
    std::string m_name;
    std::string m_url;
  public:
    explicit EmbedProvider(const nlohmann::json& data);
    explicit EmbedProvider(std::string name = "", std::string url = "");

    std::string name() const;
    std::string url() const;
  };

  void from_json(const nlohmann::json& json, EmbedProvider& provider);
  void to_json(nlohmann::json& json, const EmbedProvider& provider);

  class EmbedAuthor
  {
    std::string m_name;
    std::string m_url;
    std::string m_icon_url;
    std::string m_proxy_icon_url;
  public:
    explicit EmbedAuthor(const nlohmann::json& data);
    explicit EmbedAuthor(std::string name = "", std::string url = "", std::string icon_url = "", std::string proxy_icon_url = "");

    std::string name() const;
    std::string url() const;
    std::string icon_url() const;
    std::string proxy_icon_url() const;
  };

  void from_json(const nlohmann::json& json, EmbedAuthor& author);
  void to_json(nlohmann::json& json, const EmbedAuthor& author);

  class EmbedField
  {
    std::string m_name;
    std::string m_value;
    bool m_inline;
  public:
    EmbedField();
    explicit EmbedField(const nlohmann::json& data);
    explicit EmbedField(std::string name = "", std::string value = "", bool is_inline = false);

    std::string name() const;
    std::string content() const;
    bool is_inline() const;
  };

  void from_json(const nlohmann::json& json, EmbedField& field);
  void to_json(nlohmann::json& json, const EmbedField& field);

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
    
    /** Used to create an Embed.
     
        @code
        auto embed = Embed::create([](std::shared_ptr<Embed> embed) {
            embed->set_title("Creating an embed.");
            embed->set_description("This is how you create an embed object.");
            embed->set_author("Roughsketch", 
                              "https://github.com/Roughsketch",
                              "https://avatars2.githubusercontent.com/u/1938661?v=3&s=40);
            embed->add_field("Field Example 1", "This is an inline field", true);
            embed->add_field("Field Example 2", "This is also an inline field", true);
            embed->add_field("Field Example 3", "This isn't an inline field");
            embed->set_footer("This is a footer with just text.");
        });
        @endcode
     */
    std::shared_ptr<Embed> create(std::function<void(std::shared_ptr<Embed>)> modify_callback) const;

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