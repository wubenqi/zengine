// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
//	参考CEGUI的部分代码
//

#define USE_TINYXML_PARSER

//#define USE_EXPAT_PARSER
//#define USE_MARKUPSTL_PARSER

#include "base2/xml/xml_parser_module.h"

#if defined(USE_EXPAT_PARSER)
#include "base2/xml/expat_parser.h"
#endif

#if defined(USE_TINYXML_PARSER)
#include "base2/xml/tinyxml_parser.h"
#endif

#if defined(USE_MARKUPSTL_PARSER)
#include "base2/xml/markupstl_parser.h"
#endif

#if !defined(USE_MARKUPSTL_PARSER) && !defined(USE_TINYXML_PARSER) && !defined(USE_MARKUPSTL_PARSER)
#include "base2/xml/tinyxml_parser.h"
#endif

base::XMLParser* CreateParser(base::XMLParser::XmlParserType xml_type) {
	base::XMLParser* xml_parser = NULL;
#if defined(USE_EXPAT_PARSER)
	if (xml_type==base::XMLParser::EXPAT_XMLPARSER) {
		xml_parser = new base::ExpatParser();
	}
#endif

#if defined(USE_TINYXML_PARSER)
	if (xml_type==base::XMLParser::TINYXML_XMLPARSER) {
		xml_parser = new base::TinyxmlParser();
	}
#endif

#if defined(USE_MARKUPSTL_PARSER)
	if (xml_type==base::XMLParser::MARKUPSTL_XMLPARSER) {
		xml_parser = new base::MarkupstlParser();
	}
#endif

	if (!xml_parser) {
		xml_parser = new base::TinyxmlParser();
	}
	return xml_parser;
}

void DestroyParser(base::XMLParser* parser) {
    delete parser;
}
