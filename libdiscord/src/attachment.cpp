#include "attachment.h"

namespace Discord
{
  Attachment::Attachment()
  {
    m_size = 0;
    m_height = 0;
    m_width = 0;
  }

  Attachment::Attachment(nlohmann::json data)
  {
    set_from_json(m_filename, "filename", data);
    set_from_json(m_size, "size", data);
    set_from_json(m_url, "url", data);
    set_from_json(m_proxy_url, "proxy_url", data);
    set_from_json(m_height, "height", data);
    set_from_json(m_width, "width", data);
  }

  std::string Attachment::filename() const
  {
    return m_filename;
  }

  uint32_t Attachment::size() const
  {
    return m_size;
  }

  std::string Attachment::url() const
  {
    return m_url;
  }

  std::string Attachment::proxy_url() const
  {
    return m_proxy_url;
  }

  uint32_t Attachment::height() const
  {
    return m_height;
  }

  uint32_t Attachment::width() const
  {
    return m_width;
  }
}
