// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_XML_MODULE_XML_PLUGIN_H_
#define MOD_XML_MODULE_XML_PLUGIN_H_

#include "base/memory/singleton.h"

#include "zengine/zengine_plugin.h"

namespace zengine {

class ModuleXmlPlugin : public Plugin {
public:
  ModuleXmlPlugin();
  virtual ~ModuleXmlPlugin();

  static const std::string& GetModuleXmlName();

  //////////////////////////////////////////////////////////////////////////
  virtual const std::string& GetPluginName() const;
  virtual void Install(const base::ConfigFile* config_file);
  virtual void Initialize();
  virtual void Shutdown();
  virtual void Uninstall();
};

class ModuleXmlPluginFactory : public PluginFactory {
public:
  static ModuleXmlPluginFactory* GetInstance() {
    return Singleton<ModuleXmlPluginFactory>::get();
  }

  virtual const std::string& GetPluginName() const;
  virtual Plugin* CreateInstance();
  virtual void DestroyInstance(Plugin* module);

private:
  friend struct DefaultSingletonTraits<ModuleXmlPluginFactory>;

  ModuleXmlPluginFactory() {}
  virtual ~ModuleXmlPluginFactory ()  {}
};

}

#endif
