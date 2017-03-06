#include "embed.h"

namespace Discord
{
  EmbedFooter::EmbedFooter(const nlohmann::json& data)
  {
    set_from_json(m_text, "text", data);
    set_from_json(m_icon_url, "icon_url", data);
    set_from_json(m_proxy_icon_url, "proxy_icon_url", data);
  }

  EmbedFooter::EmbedFooter(std::string text, std::string icon_url, std::string proxy_icon_url)
    : m_text(text), m_icon_url(icon_url), m_proxy_icon_url(proxy_icon_url)
  {
  }

  std::string EmbedFooter::content() const
  {
    return m_text;
  }

  std::string EmbedFooter::icon_url() const
  {
    return m_icon_url;
  }

  std::string EmbedFooter::proxy_icon_url() const
  {
    return m_proxy_icon_url;
  }

  void from_json(const nlohmann::json& json, EmbedFooter& footer)
  {
    footer = EmbedFooter(json);
  }

  void to_json(nlohmann::json& json, const EmbedFooter& footer)
  {
    json["text"] = footer.content();

    if (!footer.icon_url().empty())
    {
      json["icon_url"] = footer.icon_url();
    }

    if (!footer.proxy_icon_url().empty())
    {
      json["proxy_icon_url"] = footer.proxy_icon_url();
    }
  }

  EmbedImage::EmbedImage()
  {
    m_height = 0;
    m_width = 0;
  }

  EmbedImage::EmbedImage(const nlohmann::json& data)
  {
    set_from_json(m_url, "url", data);
    set_from_json(m_proxy_url, "proxy_url", data);
    set_from_json(m_height, "height", data);
    set_from_json(m_width, "width", data);
  }

  EmbedImage::EmbedImage(std::string url, uint32_t width, uint32_t height, std::string proxy_url)
    : m_url(url), m_proxy_url(proxy_url), m_height(height), m_width(width)
  {
  }

  std::string EmbedImage::url() const
  {
    return m_url;
  }

  std::string EmbedImage::proxy_url() const
  {
    return m_proxy_url;
  }

  uint32_t EmbedImage::width() const
  {
    return m_width;
  }

  uint32_t EmbedImage::height() const
  {
    return m_height;
  }

  void from_json(const nlohmann::json& json, EmbedImage& image)
  {
    image = EmbedImage(json);
  }

  void to_json(nlohmann::json& json, const EmbedImage& image)
  {
    json["url"] = image.url();

    if (image.width() > 0)
    {
      json["width"] = image.width();
    }

    if (image.height() > 0)
    {
      json["height"] = image.height();
    }

    if (!image.proxy_url().empty())
    {
      json["proxy_url"] = image.proxy_url();
    }
  }

  EmbedVideo::EmbedVideo()
  {
    m_height = 0;
    m_width = 0;
  }

  EmbedVideo::EmbedVideo(const nlohmann::json& data)
  {
    set_from_json(m_url, "url", data);
    set_from_json(m_height, "height", data);
    set_from_json(m_width, "width", data);
  }

  EmbedVideo::EmbedVideo(std::string url, uint32_t width, uint32_t height)
    : m_url(url), m_height(height), m_width(width)
  {
  }

  std::string EmbedVideo::url() const
  {
    return m_url;
  }

  uint32_t EmbedVideo::width() const
  {
    return m_width;
  }

  uint32_t EmbedVideo::height() const
  {
    return m_height;
  }

  void from_json(const nlohmann::json& json, EmbedVideo& video)
  {
    video = EmbedVideo(json);
  }

  void to_json(nlohmann::json& json, const EmbedVideo& video)
  {
    json["url"] = video.url();

    if (video.width() > 0)
    {
      json["width"] = video.width();
    }

    if (video.height() > 0)
    {
      json["height"] = video.height();
    }
  }

  EmbedProvider::EmbedProvider(const nlohmann::json& data)
  {
    set_from_json(m_name, "name", data);
    set_from_json(m_url, "url", data);
  }

  EmbedProvider::EmbedProvider(std::string name, std::string url)
    : m_name(name), m_url(url)
  {
  }

  std::string EmbedProvider::name() const
  {
    return m_name;
  }

  std::string EmbedProvider::url() const
  {
    return m_url;
  }

  void from_json(const nlohmann::json& json, EmbedProvider& provider)
  {
    provider = EmbedProvider(json);
  }

  void to_json(nlohmann::json& json, const EmbedProvider& provider)
  {
    json["name"] = provider.name();

    if (!provider.url().empty())
    {
      json["url"] = provider.url();
    }
  }

  EmbedAuthor::EmbedAuthor(const nlohmann::json& data)
  {
    set_from_json(m_name, "name", data);
    set_from_json(m_url, "url", data);
    set_from_json(m_icon_url, "icon_url", data);
    set_from_json(m_proxy_icon_url, "proxy_icon_url", data);
  }

  EmbedAuthor::EmbedAuthor(std::string name, std::string url, std::string icon_url, std::string proxy_icon_url)
    : m_name(name), m_url(url), m_icon_url(icon_url), m_proxy_icon_url(proxy_icon_url)
  {
  }

  std::string EmbedAuthor::name() const
  {
    return m_name;
  }

  std::string EmbedAuthor::url() const
  {
    return m_url;
  }

  std::string EmbedAuthor::icon_url() const
  {
    return m_icon_url;
  }

  std::string EmbedAuthor::proxy_icon_url() const
  {
    return m_proxy_icon_url;
  }

  void from_json(const nlohmann::json& json, EmbedAuthor& author)
  {
    author = EmbedAuthor(json);
  }

  void to_json(nlohmann::json& json, const EmbedAuthor& author)
  {
    json["name"] = author.name();

    if (!author.url().empty())
    {
      json["url"] = author.url();
    }

    if (!author.icon_url().empty())
    {
      json["icon_url"] = author.icon_url();
    }

    if (!author.proxy_icon_url().empty())
    {
      json["proxy_icon_url"] = author.proxy_icon_url();
    }
  }

  EmbedField::EmbedField()
  {
    m_inline = false;
  }

  EmbedField::EmbedField(const nlohmann::json& data)
  {
    set_from_json(m_name, "name", data);
    set_from_json(m_value, "value", data);
    set_from_json(m_inline, "inline", data);
  }

  EmbedField::EmbedField(std::string name, std::string value, bool is_inline)
    : m_name(name), m_value(value), m_inline(is_inline)
  {
  }

  std::string EmbedField::name() const
  {
    return m_name;
  }

  std::string EmbedField::content() const
  {
    return m_value;
  }

  bool EmbedField::is_inline() const
  {
    return m_inline;
  }

  void from_json(const nlohmann::json& json, EmbedField& field)
  {
    field = EmbedField(json);
  }

  void to_json(nlohmann::json& json, const EmbedField& field)
  {
    json["name"] = field.name();
    json["value"] = field.content();

    if (field.is_inline())
    {
      json["inline"] = true;
    }
  }

  Embed::Embed()
  {
    m_color = 0;
  }

  Embed::Embed(const nlohmann::json& data)
  {
    set_from_json(m_title, "title", data);
    set_from_json(m_type, "type", data);
    set_from_json(m_description, "description", data);
    set_from_json(m_url, "url", data);
    set_from_json(m_timestamp, "timestamp", data);
    set_from_json(m_color, "color", data);
    set_from_json(m_footer, "footer", data);
    set_from_json(m_image, "image;", data);
    set_from_json(m_thumbnail, "thumbnail;", data);
    set_from_json(m_video, "video;", data);
    set_from_json(m_provider, "provider;", data);
    set_from_json(m_author, "author", data);
    set_from_json(m_fields, "fields", data);
  }

  std::shared_ptr<Embed> Embed::create(std::function<void(std::shared_ptr<Embed>)> modify_callback)
  {
    auto embed = std::shared_ptr<Embed>();

    modify_callback(embed);

    return embed;
  }

  void Embed::set_title(std::string title)
  {
    m_title = title;
  }

  void Embed::set_description(std::string description)
  {
    m_description = description;
  }

  void Embed::set_url(std::string url)
  {
    m_url = url;
  }

  void Embed::set_footer(std::string text, std::string icon_url, std::string proxy_icon_url)
  {
    m_footer = std::make_shared<EmbedFooter>(text, icon_url, proxy_icon_url);
  }

  void Embed::set_image(std::string url)
  {
    m_image = std::make_shared<EmbedImage>(url);
  }

  void Embed::set_thumbnail(std::string url)
  {
    m_thumbnail = std::make_shared<EmbedThumbnail>(url);
  }

  void Embed::set_video(std::string url)
  {
    m_video = std::make_shared<EmbedVideo>(url);
  }

  void Embed::set_author(std::string name, std::string url, std::string icon_url)
  {
    m_author = std::make_shared<EmbedAuthor>(name, url, icon_url);
  }

  void Embed::add_field(std::string name, std::string value, bool is_inline)
  {
    m_fields.push_back(std::make_shared<EmbedField>(name, value, is_inline));
  }

  std::string Embed::title() const
  {
    return m_title;
  }

  std::string Embed::type() const
  {
    return m_type;
  }

  std::string Embed::description() const
  {
    return m_description;
  }

  std::string Embed::url() const
  {
    return m_url;
  }

  std::string Embed::timestamp() const
  {
    return m_timestamp;
  }

  uint32_t Embed::color() const
  {
    return m_color;
  }

  std::shared_ptr<EmbedFooter> Embed::footer() const
  {
    return m_footer;
  }

  std::shared_ptr<EmbedImage> Embed::image() const
  {
    return m_image;
  }

  std::shared_ptr<EmbedThumbnail> Embed::thumbnail() const
  {
    return m_thumbnail;
  }

  std::shared_ptr<EmbedVideo> Embed::video() const
  {
    return m_video;
  }

  std::shared_ptr<EmbedProvider> Embed::provider() const
  {
    return m_provider;
  }

  std::shared_ptr<EmbedAuthor> Embed::author() const
  {
    return m_author;
  }

  std::vector<std::shared_ptr<EmbedField>> Embed::fields() const
  {
    return m_fields;
  }

  void from_json(const nlohmann::json& json, Embed& embed)
  {
    embed = Embed(json);
  }

  void to_json(nlohmann::json& json, const Embed& embed)
  {
    if (!embed.title().empty())
    {
      json["title"] = embed.title();
    }

    if (!embed.description().empty())
    {
      json["description"] = embed.description();
    }

    if (!embed.url().empty())
    {
      json["url"] = embed.url();
    }

    if (embed.color() != 0)
    {
      json["color"] = embed.color();
    }

    if (embed.footer())
    {
      json["footer"] = embed.footer();
    }

    if (embed.image())
    {
      json["image"] = embed.image();
    }

    if (embed.thumbnail())
    {
      json["thumbnail"] = embed.thumbnail();
    }

    if (embed.video())
    {
      json["video"] = embed.video();
    }

    if (embed.author())
    {
      json["author"] = embed.author();
    }

    if (embed.fields().size() > 0)
    {
      json["fields"] = embed.fields();
    }
  }
}
