// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_console/module_console_dll.h"

#include <string>
#include "base/logging.h"

#include "zengine/zengine_root.h"
#include "mod_console/module_console_plugin.h"

zengine::ModuleConsolePlugin* g_module_console_plugin = NULL;

#ifndef ZENGINE_STATIC_LIB
extern "C" void DllStartModule(void) {
  g_module_console_plugin = new zengine::ModuleConsolePlugin(&zengine::ModuleConsolePlugin::g_console);
  zengine::Root::GetInstance()->InstallModule(g_module_console_plugin);
}

extern "C" void DllStopModule(void) {
  zengine::Root::GetInstance()->UninstallModule(g_module_console_plugin);
  delete g_module_console_plugin;
  g_module_console_plugin = NULL;
}

#else

void DllStartModule_Console(void) {
  g_module_console_plugin = new zengine::ModuleConsolePlugin(&zengine::ModuleConsolePlugin::g_console);
  zengine::Root::GetInstance()->InstallModule(g_module_console_plugin);
}

void DllStopModule_Console(void) {
  zengine::Root::GetInstance()->UninstallModule(g_module_console_plugin);
  delete g_module_console_plugin;
}

const std::string& ModuleName_Console(void) {
  return zengine::ModuleConsolePlugin::GetModuleConsoleName();
}

#endif
