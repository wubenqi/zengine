// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
//	参考CEGUI的部分代码
//

#ifndef BASE2_XML_XML_ATTRIBUTES_H_
#define BASE2_XML_XML_ATTRIBUTES_H_
#pragma once

#include <string>
#include <map>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace base {

class XMLAttributes {
public:
	XMLAttributes(void);
	virtual ~XMLAttributes(void);

	void Add(const std::string& attr_name, const std::string& attr_val);
	void Remove(const std::string& attr_name);
	bool Exists(const std::string& attr_name) const;
	size_t GetCount(void) const;
	
	//Zero copy
	bool GetName(size_t index, std::string** val) const;
	bool GetValue(size_t index, std::string** val) const;
	bool GetValue(const std::string& attr_name, std::string** val) const;
  bool GetValueByAttrName(const std::string& attr_name, std::string** val) const;
	bool GetValueAsString(const std::string& attr_name, const std::string& default_val, std::string** val) const;

	bool GetValueAsBool(const std::string& attr_name, bool default_val, bool* val) const;
	bool GetValueAsInteger(const std::string& attr_name, int default_val, int* val) const;
	bool GetValueAsFloat(const std::string& attr_name, float default_val, float* val) const;

protected:
	typedef std::map<std::string, std::string> AttributeMap;
	AttributeMap    d_attrs;
};

}


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif
