// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
//	参考CEGUI的部分代码
//

//#include "exception.h"
#include "base2/xml/xml_attributes.h"

#include <sstream>
#include <iterator>

#include "base/string_util.h"
#include "base/logging.h"

namespace base{

XMLAttributes::XMLAttributes(void) {

}

XMLAttributes::~XMLAttributes(void) {

}

void XMLAttributes::Add(const std::string& attrName, const std::string& attrValue) {
	d_attrs[attrName] = attrValue;
}

void XMLAttributes::Remove(const std::string& attrName) {
	AttributeMap::iterator pos = d_attrs.find(attrName);

	if (pos != d_attrs.end()) {
		d_attrs.erase(pos);
	}
}

bool XMLAttributes::Exists(const std::string& attrName) const {
	return d_attrs.find(attrName) != d_attrs.end();
}

size_t XMLAttributes::GetCount(void) const {
	return d_attrs.size();
}

bool XMLAttributes::GetName(size_t index, std::string** val) const {
	if (index >= d_attrs.size()) {
		LOG(ERROR) << "XMLAttributes::getName - The specified index is out of range for this XMLAttributes block.";
		return false;
	}

	AttributeMap::const_iterator iter = d_attrs.begin();
	std::advance(iter, index);
	*val = const_cast<std::string*>(&(*iter).first);
	//*val = &(*iter).first;
	return true;
}

bool XMLAttributes::GetValue(size_t index, std::string** val) const {
	if (index >= d_attrs.size()) {
		LOG(ERROR) << "XMLAttributes::getValue - The specified index is out of range for this XMLAttributes block.";
		return false;
	}

	AttributeMap::const_iterator iter = d_attrs.begin();
	std::advance(iter, index);

	*val = const_cast<std::string*>(&(*iter).second);
	return true;
}

bool XMLAttributes::GetValue(const std::string& attr_name, std::string** val) const {
	AttributeMap::const_iterator pos = d_attrs.find(attr_name);

	if (pos != d_attrs.end()) {
		*val = const_cast<std::string*>(&(*pos).second);
		return true;
	} else {
		LOG(ERROR) << "XMLAttributes::getValue - no value exists for an attribute named '" << attr_name << "'.";
		return false;
	}
}

bool XMLAttributes::GetValueByAttrName(const std::string& attr_name, std::string** val) const {
  return GetValue(attr_name, val);
  //AttributeMap::const_iterator pos = d_attrs.find(attr_name);

  //if (pos != d_attrs.end()) {
  //  *val = const_cast<std::string*>(&(*pos).second);
  //  return true;
  //} else {
  //  LOG(ERROR) << "XMLAttributes::getValue - no value exists for an attribute named '" << attr_name << "'.";
  //  return false;
  //}
}

bool XMLAttributes::GetValueAsString(const std::string& attr_name, const std::string& def, std::string** val) const {
	if (Exists(attr_name)) {
		return GetValue(attr_name, val);
	} else {
		*val = const_cast<std::string*>(&def);
		return true;
	}
}


bool XMLAttributes::GetValueAsBool(const std::string& attr_name, bool def, bool* val) const {
	if (!Exists(attr_name)) {
		*val = def;
		return true;
	}

	std::string* val2=NULL;
	if(!GetValue(attr_name, &val2)) {
		return false;
	}

	if (*val2 == "false" || *val2 == "0") {
		*val = false;
		return true;
	} else if (*val2 == "true" || *val2 == "1") {
		*val = true;
		return true;
	} else {
		LOG(ERROR) << "XMLAttributes::getValueAsInteger - failed to convert attribute '" << attr_name <<  "' with value '" << *val2 << "' to bool.";
		//d_error = -3;
		return false;
	}
}

bool XMLAttributes::GetValueAsInteger(const std::string& attr_name, int def, int* val) const {
	if (!Exists(attr_name)) {
		*val = def;
		return true;
	}

	std::string* str_val=NULL;
	if (!GetValue(attr_name, &str_val)) {
		return false;
	}
	
	int val2;
	std::istringstream strm(str_val->c_str());
	strm >> val2;

	// success?
	if (strm.fail()) {
		LOG(ERROR) << "XMLAttributes::getValueAsInteger - failed to convert attribute '" << attr_name <<  "' with value '" << *str_val << "' to integer.";
		//d_error = -3;
		return false;
	}
	
	*val = val2;
	return true;
}

bool XMLAttributes::GetValueAsFloat(const std::string& attr_name, float def, float* val) const {
	if (!Exists(attr_name)) {
		*val = def;
		return true;
	}

	std::string* str_val=NULL;
	if (!GetValue(attr_name, &str_val)) {
		return false;
	}

	std::istringstream strm(str_val->c_str());
	float val2;
	strm >> val2;

	// success?
	if (strm.fail()) {
		//d_error = -3;
		LOG(ERROR) << "XMLAttributes::getValueAsInteger - failed to convert attribute '" << attr_name <<  "' with value '" << *str_val << "' to float.";
		return false;
	}

	*val = val2;
	return true;
}

} // End of  base namespace section

