// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
//	参考CEGUI的部分代码
//

#ifndef BASE2_XML_XML_PARSER_H_
#define BASE2_XML_XML_PARSER_H_
#pragma once

#include <string>
#include "base/file_path.h"

namespace base {

class XMLHandler;
class XMLParser {
public:
	enum XmlParserType {
		EXPAT_XMLPARSER = 0,
		TINYXML_XMLPARSER = 1,
		MARKUPSTL_XMLPARSER = 2
	};

	XMLParser(void);
	virtual ~XMLParser(void);

	bool Initialise(void);
	void Cleanup(void);
	virtual bool ParseXMLData(XMLHandler& handler, const std::string &xml_text) = 0;
	virtual bool ParseXMLData(XMLHandler& handler, const char* xml_text, size_t xml_text_size) = 0;
	virtual bool ParseXMLFile(XMLHandler& handler, const FilePath& filename) = 0;
	const std::string& GetIdentifierString() const;

protected:
	virtual bool InitialiseImpl(void) = 0;
	virtual void CleanupImpl(void) = 0;
	std::string d_identifierString;

private:
	bool d_initialised;     //!< true if the parser module has been initialised,
};

}


#endif
