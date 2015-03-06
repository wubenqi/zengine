// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "module/mod_xml/zxml_wrapper.h"

#include "base/logging.h"
#include "base2/xml/xml_lite.h"

#include "zengine/zengine_context_manager.h"
#include "zengine/script/script_manager.h"
#include "zengine/zengine_context.h"

namespace zengine {

void ZXmlWrapper::Initialize() {
  ZEngineContext* ctx = ZEngineContextManager::GetInstance()->LookupMainContext();
  CHECK(ctx);
  script_ = ctx->script_manager()->GetScriptEngine();
}

bool ZXmlWrapper::DoParseXml(lua_tinker::table xml, const char* data) {
  // 传入一张表
  // 取到表
  // 然后传入LUA
  // ConfigFileXmlHandler xml_handler(config_map_);
  table_.reset(new lua_tinker::table(xml));

  base::XMLLite xml_lite;
  size_t data_len = strlen(data);
  bool result = xml_lite.GetXmlParser()->ParseXMLData(*this, data, data_len);
  if (!result) {
    // 
  }
  table_.reset();

  return result;
}

bool ZXmlWrapper::ElementStart(const std::string& element, const base::XMLAttributes& attributes) {

  int result = script_->CallFunction<int, lua_tinker::table, const char*, base::XMLAttributes>("ElementStart", *table_, element.c_str(), attributes);
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute ElementStart() error, error_code = " << result;
  }

  return true;
}

bool ZXmlWrapper::ElementEnd(const std::string& element) {

  int result = script_->CallFunction<int, lua_tinker::table, const char*>("ElementEnd", *table_, element.c_str());
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute ElementEnd() error, error_code = " << result;
  }

  return true;
}

bool ZXmlWrapper::Text(const std::string& text) {

  int result = script_->CallFunction<int, lua_tinker::table, const char*>("Text", *table_, text.c_str());
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute Text() error, error_code = " << result;
  }

  return true;
}

}
