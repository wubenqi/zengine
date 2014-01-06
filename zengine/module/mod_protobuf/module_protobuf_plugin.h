// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_PROTOBUF_MODULE_PROTOBUF_PLUGIN_H_
#define MOD_PROTOBUF_MODULE_PROTOBUF_PLUGIN_H_
#pragma once

#include "base/memory/singleton.h"
#include "base2/dyn_lib.h"

#include "zengine/zengine_plugin.h"

namespace zengine {

class ModuleProtobufPlugin : public Plugin {
public:
  ModuleProtobufPlugin();
  virtual ~ModuleProtobufPlugin();

  static const std::string& GetModuleProtobufName();

  //////////////////////////////////////////////////////////////////////////
  virtual const std::string& GetPluginName() const;
  virtual void Install();
  virtual void Initialize();
  virtual void Shutdown();
  virtual void Uninstall();

private:
  base::DynLib* dyn_lib_;
};

class ModuleProtobufPluginFactory : public PluginFactory {
public:
  static ModuleProtobufPluginFactory* GetInstance() {
    return Singleton<ModuleProtobufPluginFactory>::get();
  }

  virtual const std::string& GetPluginName() const;
  virtual Plugin* CreateInstance();
  virtual void DestroyInstance(Plugin* module);

private:
  friend struct DefaultSingletonTraits<ModuleProtobufPluginFactory>;

  ModuleProtobufPluginFactory() {}
  virtual ~ModuleProtobufPluginFactory ()  {}
};

}

#endif
