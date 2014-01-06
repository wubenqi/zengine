// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "base2/xml/xml_lite.h"

#include "base2/xml/xml_parser_module.h"

namespace base {

XMLLite::XMLLite(XMLParser::XmlParserType xml_type) {
	xml_parser_ = ::CreateParser(xml_type);
	xml_parser_->Initialise();

}

XMLLite::~XMLLite() {
	if (xml_parser_) {
		xml_parser_->Cleanup();
		::DestroyParser(xml_parser_);
	}
}

}
