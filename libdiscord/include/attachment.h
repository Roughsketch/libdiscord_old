#pragma once

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class Attachment : public Identifiable
  {
    std::string m_filename;
    uint32_t m_size;
    std::string m_url;
    std::string m_proxy_url;
    uint32_t m_height;
    uint32_t m_width;
  public:
    Attachment();
    explicit Attachment(nlohmann::json data);

    std::string filename() const;
    uint32_t size() const;
    std::string url() const;
    std::string proxy_url() const;
    uint32_t height() const;
    uint32_t width() const;
  };

  inline void from_json(const nlohmann::json& json, Attachment& attachment)
  {
    attachment = Attachment(attachment);
  }

  inline void to_json(nlohmann::json& json, const Attachment& attachment)
  {
    json["id"] = attachment.id();
    json["filename"] = attachment.filename();
    json["size"] = attachment.size();
    json["url"] = attachment.url();
    json["proxy_url"] = attachment.proxy_url();

    if (attachment.height() > 0)
    {
      json["height"] = attachment.height();
    }

    if (attachment.width() > 0)
    {
      json["width"] = attachment.width();
    }
  }
}