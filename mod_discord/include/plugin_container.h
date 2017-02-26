#pragma once
#include <string>
#include <memory>

#include "plugin.h"
#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace ModDiscord
{
  class Plugin;

  class PluginContainer
  {
    boost::shared_ptr<Plugin> m_plugin;
    typedef boost::shared_ptr<Plugin> (plugin_create_t)();
    boost::function<plugin_create_t> m_plugin_factory;
  public:
    PluginContainer();
    static std::shared_ptr<PluginContainer> load(boost::filesystem::path file);

    boost::shared_ptr<Plugin> get() const
    {
      return m_plugin;
    }
  };
}
