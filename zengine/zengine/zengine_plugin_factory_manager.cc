// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/zengine_plugin_factory_manager.h"

#include "mod_db/module_db_plugin.h"
#include "mod_net2/module_net2_plugin.h"
#include "mod_protobuf/module_protobuf_plugin.h"

namespace zengine {

void PluginFactoryManager::Initialize() {
  module_factorys_.push_back(ModuleNet2PluginFactory::GetInstance());
  module_factorys_.push_back(ModuleDBPluginFactory::GetInstance());
  module_factorys_.push_back(ModuleProtobufPluginFactory::GetInstance());
}

Plugin* PluginFactoryManager::CreateInstance(const std::string& module_name) {
  Plugin* module = NULL;
  for (size_t i=0; i<module_factorys_.size(); ++i) {
    if (module_factorys_[i]->GetPluginName() == module_name) {
      module = module_factorys_[i]->CreateInstance();
      break;
    }
  }
  return module;
}

void PluginFactoryManager::Shutdown() {
  module_factorys_.clear();
}

}
