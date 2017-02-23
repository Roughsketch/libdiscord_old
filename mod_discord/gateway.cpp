#include "gateway.h"

namespace ModDiscord
{
  static const uint8_t LARGE_SERVER = 100;
  static const std::string VERSION = "6";

  Gateway::Gateway(std::string token) : m_token(token)
  {

  }
}