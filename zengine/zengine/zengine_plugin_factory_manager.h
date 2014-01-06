// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_ZENGINE_PLUGIN_FACTORY_MANAGER_H_
#define ZENGINE_ZENGINE_PLUGIN_FACTORY_MANAGER_H_
#pragma once

#include <string>
#include <vector>
#include "base/memory/singleton.h"

#include "zengine/zengine_plugin.h"

namespace zengine {

class Plugin;
class PluginFactory;
class PluginFactoryManager {
public:
  static PluginFactoryManager* GetInstance() {
    return Singleton<PluginFactoryManager>::get();
  }

  void Initialize();
  Plugin* CreateInstance(const std::string& module_name);
  void Shutdown();

private:
  friend struct DefaultSingletonTraits<PluginFactoryManager>;
 
  PluginFactoryManager() {}
  ~PluginFactoryManager() {
    Shutdown();
  }

  std::vector<PluginFactory*> module_factorys_;
};

}

#endif
