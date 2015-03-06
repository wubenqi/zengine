// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_xml/register_helper/xml_register_helper.h"

#include "base/basictypes.h"
#include "scriptengine/luatinker/lua_tinker.h"

#include "zengine/zengine_context.h"
#include "mod_xml/module_xml_dll.h"
#include "mod_xml/zxml_wrapper.h"

namespace {

bool DoParseXml(lua_tinker::table xml, const char* data) {
  return zengine::ZXmlWrapper::GetInstance()->DoParseXml(xml, data);
}

void Luabind_Xml_Register(lua_State* L) {
  //const base::XMLAttributes& attributes
  lua_tinker::class_add<base::XMLAttributes>(L, "XMLAttributes");

  lua_tinker::class_def<base::XMLAttributes>(L, "GetName", (std::string(base::XMLAttributes::*)(size_t) const)&base::XMLAttributes::GetName);
  lua_tinker::class_def<base::XMLAttributes>(L, "GetValue", &base::XMLAttributes::GetValue2);
  lua_tinker::class_def<base::XMLAttributes>(L, "GetCount", &base::XMLAttributes::GetCount);
  lua_tinker::class_def<base::XMLAttributes>(L, "GetValueByName", &base::XMLAttributes::GetValueByName);

  lua_tinker::def(L, "DoParseXml", &DoParseXml);
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

//void LuabindPB_RegisterAll(lua_State* L);

namespace zengine {

void Xml_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_Xml(), &Luabind_Xml_Register);
  //LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_Protobuf(), &LuabindPB_RegisterAll);
}

}
