// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_ZENGINE_PLUGIN_DLL_MANAGER_H_
#define ZENGINE_ZENGINE_PLUGIN_DLL_MANAGER_H_
#pragma once

#include <string>
#include <vector>
#include "base/tuple.h"
#include "base/memory/singleton.h"
#include "zengine/zengine_plugin_dll_func.h"

namespace zengine {

class PluginDllManager {
public:

  static PluginDllManager* GetInstance() {
    return Singleton<PluginDllManager>::get();
  }

  void Initialize();

  void DllStartModule(const std::string& module_name);
  void DllStopModule(const std::string& module_name);

private:
  friend struct DefaultSingletonTraits<PluginDllManager>;
 
  PluginDllManager() {}
  ~PluginDllManager() {}

  typedef Tuple3<std::string, Func_DllStartModule, Func_DllStopModule> DllModuleNameAndFuncs;
  typedef std::vector<DllModuleNameAndFuncs> DllModuleNameAndFuncsList;
  DllModuleNameAndFuncsList dll_funcs_;
};

}

#endif
