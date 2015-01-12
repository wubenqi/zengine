// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_console/module_console_plugin.h"

#include "base2/config_file.h"
#include "zengine/zengine_context.h"
#include "zengine/zengine_context_manager.h"
#include "mod_console/register_helper/console_register_helper.h"

namespace zengine {

const std::string kConsoleModuleName("mod_console");

LocalConsole ModuleConsolePlugin::g_console;

const std::string& ModuleConsolePlugin::GetModuleConsoleName() {
  return kConsoleModuleName;
}
//////////////////////////////////////////////////////////////////////////
const std::string& ModuleConsolePlugin::GetPluginName() const {
  return kConsoleModuleName;
}

void ModuleConsolePlugin::Install(const base::ConfigFile* config_file) {
  Plugin::Install(config_file);
}

void ModuleConsolePlugin::Initialize() {
  // ConfigFile* config = ConfigFile::GetInstance();

  const char* listen_instance_name = config_file_->GetString(kConsoleModuleName.c_str(), "listen_instance_name");
  if (listen_instance_name) {
    listen_instance_name_ = listen_instance_name;
    listen_context_ = ZEngineContextManager::GetInstance()->LookupContext(listen_instance_name_);
  }
  
  if (!listen_context_) {
    listen_context_ = ZEngineContextManager::GetInstance()->LookupMainContext();
    CHECK(listen_context_);
  }
  console_thread_.Initialize();
}

void ModuleConsolePlugin::Shutdown() {
  console_thread_.Destroy();
}

void ModuleConsolePlugin::Uninstall() {
}

void ModuleConsolePlugin::OnConsoleInput(const char* input) {
  if (input && listen_context_) {
    listen_context_->PostTask(input);
  }
}

//////////////////////////////////////////////////////////////////////////
const std::string& ModuleConsolePluginFactory::GetPluginName() const {
  return kConsoleModuleName;
}

Plugin* ModuleConsolePluginFactory::CreateInstance() {
  return new ModuleConsolePlugin(&ModuleConsolePlugin::g_console);
}

void ModuleConsolePluginFactory::DestroyInstance(Plugin* module) {
  ModuleConsolePlugin* mod = reinterpret_cast<ModuleConsolePlugin*>(module);
  if (mod) {
    delete mod;
    mod = NULL;
  }
}

}
