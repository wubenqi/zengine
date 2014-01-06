// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
//	参考CEGUI的部分代码
//

#ifndef BASE2_XML_XML_PARSER_MODULE_H_
#define BASE2_XML_XML_PARSER_MODULE_H_
#pragma once

#include "base2/xml/xml_parser.h"

base::XMLParser* CreateParser(base::XMLParser::XmlParserType xml_type);
void DestroyParser(base::XMLParser* parser);

#endif
