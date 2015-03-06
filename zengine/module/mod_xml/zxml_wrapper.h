// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_XML_ZXML_WRAPPER_H_
#define MOD_XML_ZXML_WRAPPER_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base2/xml/xml_attributes.h"
#include "base2/xml/xml_handler.h"

#include "scriptengine/luatinker/lua_tinker.h"

class ScriptEngine;
namespace zengine {

class ZXmlWrapper :
  public base::XMLHandler {

public:
  ~ZXmlWrapper() {}

  static ZXmlWrapper* GetInstance() {
    return Singleton<ZXmlWrapper>::get();
  }

  void Initialize();

  bool DoParseXml(lua_tinker::table xml, const char* data);

  // from base::XMLHandler
  virtual bool ElementStart(const std::string& element, const base::XMLAttributes& attributes);
  virtual bool ElementEnd(const std::string& element);
  virtual bool Text(const std::string& text);

protected:
  friend struct DefaultSingletonTraits<ZXmlWrapper>;

  ZXmlWrapper() 
    : script_(NULL) {
    // http_fetcher_.reset(HttpFetchFactory::Create());
  }


  ScriptEngine* script_;
  scoped_ptr<lua_tinker::table> table_;
};

}


#endif
