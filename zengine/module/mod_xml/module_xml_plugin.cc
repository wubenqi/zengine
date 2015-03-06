// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_xml/module_xml_plugin.h"

#include "base/logging.h"

#include "base2/config_file.h"

#include "scriptengine/script_engine.h"

#include "mod_xml/module_xml_dll.h"
#include "mod_xml/register_helper/xml_register_helper.h"
#include "mod_xml/zxml_wrapper.h"

namespace zengine {

const std::string kXmlModuleName("mod_xml");

const std::string& ModuleXmlPlugin::GetModuleXmlName() {
  return kXmlModuleName;
}

//////////////////////////////////////////////////////////////////////////
ModuleXmlPlugin::ModuleXmlPlugin() {
}

ModuleXmlPlugin::~ModuleXmlPlugin() {
}

const std::string& ModuleXmlPlugin::GetPluginName() const {
  return kXmlModuleName;
}

void ModuleXmlPlugin::Install(const base::ConfigFile* config_file) {
  // ConfigFile* config = ConfigFile::GetInstance();
  Xml_Register_Helper();
}

void ModuleXmlPlugin::Initialize() {
  ZXmlWrapper::GetInstance()->Initialize();
}

void ModuleXmlPlugin::Shutdown() {
}

void ModuleXmlPlugin::Uninstall() {
}

//////////////////////////////////////////////////////////////////////////
const std::string& ModuleXmlPluginFactory::GetPluginName() const {
  return kXmlModuleName;
}

Plugin* ModuleXmlPluginFactory::CreateInstance() {
  return new ModuleXmlPlugin();
}

void ModuleXmlPluginFactory::DestroyInstance(Plugin* module) {
  ModuleXmlPlugin* mod = reinterpret_cast<ModuleXmlPlugin*>(module);
  if (mod) {
    delete mod;
    mod = NULL;
  }
}

}
