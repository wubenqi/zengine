// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_db/module_db_dll.h"

#include "base/logging.h"

#include "zengine/zengine_root.h"
#include "mod_db/module_db_plugin.h"

zengine::ModuleDBPlugin* g_module_db_plugin = NULL;

#ifndef ZENGINE_STATIC_LIB
extern "C" void DllStartModule(void) {
  g_module_db_plugin = new zengine::ModuleDBPlugin();
  zengine::Root::GetInstance()->InstallModule(g_module_db_plugin);
}

extern "C" void DllStopModule(void) {
  zengine::Root::GetInstance()->UninstallModule(g_module_db_plugin);
  delete g_module_db_plugin;
  g_module_db_plugin = NULL;
}

#else

void DllStartModule_DB(void) {
  g_module_db_plugin = new zengine::ModuleDBPlugin();
  zengine::Root::GetInstance()->InstallModule(g_module_db_plugin);
}

void DllStopModule_DB(void) {
  zengine::Root::GetInstance()->UninstallModule(g_module_db_plugin);
  delete g_module_db_plugin;
}

const std::string& ModuleName_DB(void) {
  return zengine::ModuleDBPlugin::GetModuleDBName();
}

#endif

CdbConnPoolManagerProxy* GetDBConnPoolMgrByInstanceName(const std::string& instance_name) {
  return g_module_db_plugin==NULL?NULL:g_module_db_plugin->GetDBConnPoolMgrByInstanceName(instance_name);
}

CdbConnPoolManagerProxy* GetDBConnPoolMgr() {
  return g_module_db_plugin==NULL?NULL:g_module_db_plugin->GetDBConnPoolMgr();
}
