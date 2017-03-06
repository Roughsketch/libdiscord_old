#pragma once

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  /** A container for an attachment on a message. */
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

    /** Get the filename of this attachment.
     
        @return The filename of the attachment.
     */
    std::string filename() const;
    
    /** Get the size of the attachment.
     
        @return The size of the attachment in bytes.
     */
    uint32_t size() const;

    /** Get the URL to download this attachment.
     
        @return The URL that can be used to download this attachment.
     */
    std::string url() const;

    /** Get a proxied URL of the file.
     
        @return The proxied URL of the file.
     */
    std::string proxy_url() const;

    /** Get the height of this attachment. 
     
        @return The height of this attachment.
     */
    uint32_t height() const;

    /** Get the width of this attachment.

        @return The width of this attachment.
    */
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