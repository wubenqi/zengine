// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef BASE2_XML_XML_LITE_H_
#define BASE2_XML_XML_LITE_H_
#pragma once

#include <string>
#include "base/basictypes.h"
#include "base/memory/singleton.h"

#include "base2/xml/xml_parser.h"

namespace base {


class XMLLite {
public:
	static XMLLite* GetInstance() {
		return Singleton<base::XMLLite>::get();
	}

	XMLParser* GetXmlParser() {
		return xml_parser_;
	}

private:
	friend struct DefaultSingletonTraits<XMLLite>;

	explicit XMLLite(XMLParser::XmlParserType xml_type=XMLParser::TINYXML_XMLPARSER);
	virtual ~XMLLite();

	XMLParser* xml_parser_;
	DISALLOW_COPY_AND_ASSIGN(XMLLite);
};

}

//typedef Singleton<base::XMLLite>	XMLLiteSinleton;

#endif
