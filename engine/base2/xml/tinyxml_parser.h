// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
//	参考CEGUI的部分代码
//

#ifndef BASE2_XML_TINYXML_PARSER_H_
#define BASE2_XML_TINYXML_PARSER_H_
#pragma once

#include "base2/xml/xml_parser.h"

namespace base {

class TinyxmlParser : 
	public XMLParser {
public:
	TinyxmlParser(void);
	~TinyxmlParser(void);
	// Implementation of public abstract interface
	bool ParseXMLData(XMLHandler& handler, const std::string &xml_text);
	bool ParseXMLData(XMLHandler& handler, const char* xml_text, size_t xml_text_size);
	bool ParseXMLFile(XMLHandler& handler, const FilePath& filename);

protected:
	// Implementation of protected abstract interface.
	bool InitialiseImpl(void);
	// Implementation of protected abstract interface.
	void CleanupImpl(void);
};

}

#endif
