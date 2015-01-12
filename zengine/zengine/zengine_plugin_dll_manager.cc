// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/zengine_plugin_dll_manager.h"

#include "mod_net/module_net_dll.h"
//#include "mod_db/module_db_dll.h"
//#include "mod_protobuf/module_protobuf_dll.h"
//#include "mod_http_client/module_http_client_dll.h"
//#include "mod_ctemplate/module_ctemplate_dll.h"
//#include "mod_html/module_html_dll.h"
//#include "mod_tidy/module_tidy_dll.h"
//#include "mod_xpath/module_xpath_dll.h"
#include "mod_console/module_console_dll.h"

namespace zengine {

void PluginDllManager::Initialize() {
  dll_funcs_.push_back(MakeTuple(ModuleName_Net(), &DllStartModule_Net, &DllStopModule_Net));
  //dll_funcs_.push_back(MakeTuple(ModuleName_DB(), &DllStartModule_DB, &DllStopModule_DB));
  //dll_funcs_.push_back(MakeTuple(ModuleName_Protobuf(), &DllStartModule_Protobuf, &DllStopModule_Protobuf));
  //dll_funcs_.push_back(MakeTuple(ModuleName_HttpClient(), &DllStartModule_HttpClient, &DllStopModule_HttpClient));

  //dll_funcs_.push_back(MakeTuple(ModuleName_CTemplate(), &DllStartModule_CTemplate, &DllStopModule_CTemplate));
  //dll_funcs_.push_back(MakeTuple(ModuleName_Html(), &DllStartModule_Html, &DllStopModule_Html));
  //dll_funcs_.push_back(MakeTuple(ModuleName_Tidy(), &DllStartModule_Tidy, &DllStopModule_Tidy));
  //dll_funcs_.push_back(MakeTuple(ModuleName_XPath(), &DllStartModule_XPath, &DllStopModule_XPath));
  dll_funcs_.push_back(MakeTuple(ModuleName_Console(), &DllStartModule_Console, &DllStopModule_Console));
}


void PluginDllManager::DllStartModule(const std::string& module_name) {
  for (size_t i=0; i<dll_funcs_.size(); ++i) {
    if (dll_funcs_[i].a == module_name) {
      dll_funcs_[i].b();
    }
  }
}

void PluginDllManager::DllStopModule(const std::string& module_name) {
  for (size_t i=0; i<dll_funcs_.size(); ++i) {
    if (dll_funcs_[i].a == module_name) {
      dll_funcs_[i].c();
    }
  }
}

}
