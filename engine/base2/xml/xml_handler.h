// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
//	参考CEGUI的部分代码
//

#ifndef BASE2_XML_XML_HANDLER_H_
#define BASE2_XML_XML_HANDLER_H_
#pragma once

#include <string>

namespace base {

class XMLAttributes;
class XMLHandler {
public:
	XMLHandler(void) {}
	virtual ~XMLHandler(void) {}

	virtual bool ElementStart(const std::string& element, const XMLAttributes& attributes) { return true; }
	virtual bool ElementEnd(const std::string& element) { return true; }
	virtual bool Text(const std::string& text) { return true; }
};

}

#endif  //
