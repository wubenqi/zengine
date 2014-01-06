// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_protobuf/module_protobuf_dll.h"

#include "base/logging.h"

#include "zengine/zengine_root.h"

#include "mod_protobuf/module_protobuf_plugin.h"

zengine::ModuleProtobufPlugin* g_module_protobuf_plugin = NULL;

#ifndef ZENGINE_STATIC_LIB

extern "C" void DllStartModule(void) {
  g_module_protobuf_plugin = new zengine::ModuleProtobufPlugin();
  zengine::Root::GetInstance()->InstallModule(g_module_protobuf_plugin);
}

extern "C" void DllStopModule(void) {
  zengine::Root::GetInstance()->UninstallModule(g_module_protobuf_plugin);
  delete g_module_protobuf_plugin;
  g_module_protobuf_plugin = NULL;
}

#else

void DllStartModule_Protobuf(void) {
  g_module_protobuf_plugin = new zengine::ModuleProtobufPlugin();
  zengine::Root::GetInstance()->InstallModule(g_module_protobuf_plugin);
}

void DllStopModule_Protobuf(void) {
  zengine::Root::GetInstance()->UninstallModule(g_module_protobuf_plugin);
  delete g_module_protobuf_plugin;
}

const std::string& ModuleName_Protobuf(void) {
  return zengine::ModuleProtobufPlugin::GetModuleProtobufName();
}

#endif
