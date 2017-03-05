#pragma once

#include <exception>
#include <string>

namespace Discord
{
  class UnknownException : public std::exception
  {
  protected:
    std::string m_msg;

  public:
    explicit UnknownException(const char* message) : m_msg(message)
    {
    }

    explicit UnknownException(const std::string& message) :
      m_msg(message)
    {}

    virtual ~UnknownException() throw () {}

    virtual const char* what() const throw () override {
      return m_msg.c_str();
    }
  };

  class PermissionException : public std::exception
  {
  protected:
    std::string m_msg;

  public:
    explicit PermissionException(const char* message) : m_msg(message)
    {
    }

    explicit PermissionException(const std::string& message) :
      m_msg(message)
    {}

    virtual ~PermissionException() throw () {}

    virtual const char* what() const throw () override {
      return m_msg.c_str();
    }
  };

  class AuthorizationException : public std::exception
  {
  protected:
    std::string m_msg;

  public:
    explicit AuthorizationException(const char* message) : m_msg(message)
    {
    }

    explicit AuthorizationException(const std::string& message) :
      m_msg(message)
    {}

    virtual ~AuthorizationException() throw () {}

    virtual const char* what() const throw () override {
      return m_msg.c_str();
    }
  };

  class EmbedException : public std::exception
  {
  protected:
    std::string m_msg;

  public:
    explicit EmbedException(const char* message) : m_msg(message)
    {
    }

    explicit EmbedException(const std::string& message) :
      m_msg(message)
    {}

    virtual ~EmbedException() throw () {}

    virtual const char* what() const throw () override {
      return m_msg.c_str();
    }
  };

  class TooManyException : public std::exception
  {
  protected:
    std::string m_msg;

  public:
    explicit TooManyException(const char* message) : m_msg(message)
    {
    }

    explicit TooManyException(const std::string& message) :
      m_msg(message)
    {}

    virtual ~TooManyException() throw () {}

    virtual const char* what() const throw () override {
      return m_msg.c_str();
    }
  };

  class DiscordException : public std::exception
  {
  protected:
    std::string m_msg;

  public:
    explicit DiscordException(const char* message) : m_msg(message)
    {
    }

    explicit DiscordException(const std::string& message) :
      m_msg(message)
    {}

    virtual ~DiscordException() throw () {}

    virtual const char* what() const throw () override {
      return m_msg.c_str();
    }
  };
}