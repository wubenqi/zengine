// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_net/module_net_dll.h"

#include "base/logging.h"

#include "zengine/zengine_root.h"
#include "mod_net/module_net_plugin.h"

zengine::ModuleNetPlugin* g_module_net_plugin = NULL;

#ifndef ZENGINE_STATIC_LIB

extern "C" void DllStartModule(void) {
  g_module_net_plugin = new zengine::ModuleNetPlugin();
  zengine::Root::GetInstance()->InstallModule(g_module_net_plugin);
}

extern "C" void DllStopModule(void) {
  zengine::Root::GetInstance()->UninstallModule(g_module_net_plugin);
  delete g_module_net_plugin;
  g_module_net_plugin = NULL;
}

#else

void DllStartModule_Net(void) {
  g_module_net_plugin = new zengine::ModuleNetPlugin();
  zengine::Root::GetInstance()->InstallModule(g_module_net_plugin);
}

void DllStopModule_Net(void) {
  zengine::Root::GetInstance()->UninstallModule(g_module_net_plugin);
  delete g_module_net_plugin;
  g_module_net_plugin = NULL;
}

const std::string& ModuleName_Net(void) {
  return zengine::ModuleNetPlugin::GetModuleNetName();
}

#endif

// bool CreateTCPConnector2(const std::string& addr, const std::string& port, int reconnector_timeout, const std::string& instance_name) {
//   DCHECK(g_module_net2_plugin);
//   return g_module_net2_plugin->CreateTCPConnector(addr, port, reconnector_timeout, instance_name);
// }
// 
// bool CreateTCPAcceptor2(const std::string& addr, const std::string& port, const std::string& instance_name) {
//   DCHECK(g_module_net2_plugin);
//   return g_module_net2_plugin->CreateTCPAcceptor(addr, port, instance_name);
// }
