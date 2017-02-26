#include "plugin_container.h"

#include <boost/dll/import.hpp>
#include <boost/log/trivial.hpp>

namespace fs = boost::filesystem;
namespace dll = boost::dll;

namespace ModDiscord
{
  PluginContainer::PluginContainer()
  {
  }

  std::shared_ptr<PluginContainer> PluginContainer::load(boost::filesystem::path file)
  {
    BOOST_LOG_TRIVIAL(info) << "Loading plugin " << file;

    dll::shared_library lib(file, dll::load_mode::append_decorations);

    if (lib.has("create_plugin"))
    {
      BOOST_LOG_TRIVIAL(info) << "Plugin has plugin member.";
      auto container = std::make_shared<PluginContainer>();

      BOOST_LOG_TRIVIAL(info) << "Made shared_ptr.";

      container->m_plugin_factory = dll::import_alias<plugin_create_t>(
        file,
        "create_plugin",
        dll::load_mode::append_decorations
      );

      BOOST_LOG_TRIVIAL(info) << "Loaded factory.";

      container->m_plugin = container->m_plugin_factory();

      BOOST_LOG_TRIVIAL(info) << "Loaded plugin: " 
        << container->m_plugin->author() << "'s " 
        << container->m_plugin->name() << " ("
        << container->m_plugin->version() + ")";

      return container;
    }

    BOOST_LOG_TRIVIAL(info) << "Plugin does not have plugin member.";
    
    return nullptr;
  }

}