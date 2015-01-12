// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_CONSOLE_MODULE_CONSOLE_PLUGIN_H_
#define MOD_CONSOLE_MODULE_CONSOLE_PLUGIN_H_
#pragma once

#include "base/memory/singleton.h"
#include "zengine/zengine_plugin.h"

#include "mod_console/console_listener.h"
#include "mod_console/console_thread.h"
#include "mod_console/base_console.h"

class ZEngineContext;
namespace zengine {

class ModuleConsolePlugin : public Plugin, public ConsoleListener {
public:
  ModuleConsolePlugin(BaseConsole* console)
    : ConsoleListener(console)
    , console_thread_(this)
    , listen_instance_name_("main") {
  }
  virtual ~ModuleConsolePlugin() {}

  static const std::string& GetModuleConsoleName();

  //////////////////////////////////////////////////////////////////////////
  virtual const std::string& GetPluginName() const;
  virtual void Install(const base::ConfigFile* config_file);
  virtual void Initialize();
  virtual void Shutdown();
  virtual void Uninstall();

  //////////////////////////////////////////////////////////////////////////
  virtual void OnConsoleInput(const char* input);

  static LocalConsole g_console;

private:
  std::string listen_instance_name_;
  ConsoleThread console_thread_;
  ZEngineContext* listen_context_;
};

class ModuleConsolePluginFactory : public PluginFactory {
public:
  static ModuleConsolePluginFactory* GetInstance() {
    return Singleton<ModuleConsolePluginFactory>::get();
  }

  virtual const std::string& GetPluginName() const;
  virtual Plugin* CreateInstance();
  virtual void DestroyInstance(Plugin* module);

private:
  friend struct DefaultSingletonTraits<ModuleConsolePluginFactory>;

  ModuleConsolePluginFactory() {}
  virtual ~ModuleConsolePluginFactory ()  {}
};

}

#endif
