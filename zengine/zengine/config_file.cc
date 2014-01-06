// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/config_file.h"

#include "base/logging.h"
#include "base/file_util.h"
#include "base/string_util.h"
#include "base/string_number_conversions.h"

#include "base2/file_path_util.h"
#include "base2/xml/xml_lite.h"

#include "zengine/config_file_xml_handler.h"

namespace {

const char* GetValueByKey(const ConfigFile::KeyValuePairList& key_values, const char* key, const char* default_val=NULL) {
  const char* val = default_val;
  for (size_t i=0; i<key_values.size(); ++i) {
    if (key_values[i].first == key) {
      val = key_values[i].second.c_str();
    }
  }
  return val;
}

void GetValueListByKey(const ConfigFile::KeyValuePairList& key_values, const char* key, std::vector<const char*>* values) {
  for (size_t i=0; i<key_values.size(); ++i) {
    if (key_values[i].first == key) {
      values->push_back(key_values[i].second.c_str());
    }
  }
}

void GetValueListByKey(const ConfigFile::KeyValuePairList& key_values, const char* key, std::vector<std::string>* values) {
  for (size_t i=0; i<key_values.size(); ++i) {
    if (key_values[i].first == key) {
      values->push_back(key_values[i].second.c_str());
    }
  }
}

}


int ConfigFile::Initialize(const FilePath& config_file_path) {
  config_file_path_ = config_file_path;
  return Load();
}

int ConfigFile::Load() {
  ConfigFileXmlHandler xml_handler(config_map_);
  bool result = base::XMLLite::GetInstance()->GetXmlParser()->ParseXMLFile(xml_handler, config_file_path_);
  if (!result) {
    Destroy();
  }
  return result?0:-1;
}

void ConfigFile::Destroy() {
  config_map_.clear();
}

const char* ConfigFile::GetString(const char* section, const char* key, const char* default_val) const {
  const char* val = default_val;
  SectionMap::const_iterator it = config_map_.find(section);
  if (it != config_map_.end()) {
    val = GetValueByKey(it->second, key, default_val);
  }
  return val;
}

int ConfigFile::GetInt(const char* section, const char* key, int default_val) const {
  int val = default_val;
  const char* str_val = GetString(section, key, NULL);
  if (str_val!=NULL) {
    base::StringToInt(str_val, &val);
  }
  return val;
}

zengine::CStringVector ConfigFile::GetCStringList(const char* section, const char* key) const {
  zengine::CStringVector values;
  SectionMap::const_iterator it = config_map_.find(section);
  if (it != config_map_.end()) {
    GetValueListByKey(it->second, key, &(values.cstring_vector));
  }
  return values;
}

zengine::StringVector ConfigFile::GetStringList(const char* section, const char* key) const {
  zengine::StringVector values;
  SectionMap::const_iterator it = config_map_.find(section);
  if (it != config_map_.end()) {
    GetValueListByKey(it->second, key, &(values.string_vector));
  }
  return values;
}

zengine::StringVector ConfigFile::GetSectionKey(const char *section) const {
  zengine::StringVector keys;
  SectionMap::const_iterator it = config_map_.find(section);
  if (it != config_map_.end()) {
    for (size_t i=0; i<it->second.size(); ++i) {
      keys.string_vector.push_back(it->second[i].first);
    }
  }
  return keys;
}

bool ConfigFile::CheckBySection(const char* section) const {
  return config_map_.find(section) != config_map_.end();
}

bool ConfigFile::CheckBySectionAndKey(const char* section, const char* key) const {
  bool result = false;
  SectionMap::const_iterator it = config_map_.find(section);
  if (it != config_map_.end()) {
    for (size_t i=0; i<it->second.size(); ++i) {
      if (it->second[i].first == key) {
        result = true;
        break;
      }
    }
  }
  return result;
}

bool ConfigFile::CheckByValue(const char* section, const char* key, const char* val) const {
  bool result = false;
  SectionMap::const_iterator it = config_map_.find(section);
  if (it != config_map_.end()) {
    for (size_t i=0; i<it->second.size(); ++i) {
      if (it->second[i].first == key && it->second[i].second == val) {
        result = true;
        break;
      }
    }
  }
  return result;
}


zengine::StringVector ConfigFile::GetSectionName() const {
  zengine::StringVector keys;
  SectionMap::const_iterator it;
  for (it=config_map_.begin(); it!=config_map_.end(); ++it) {
    keys.string_vector.push_back(it->first);
  }
  return keys;
}

#if 0
#include "base2/xml_ini_file.h"

const std::string CommNodeInfo::kServiceType_Connect("connect");
const std::string CommNodeInfo::kServiceType_Bind("bind");
const int CommNodeInfo::kDefaultReconnectedTimeout = 10;   // 10√Î
const std::string CommNodeInfo::kDefaultAuthPassword("0123456789");

int ConfigInfo::Initialize(const FilePath& config_file) {
	config_file_ = config_file;
	exe_file_ = config_file_.DirName();
	return Load();
}

int ConfigInfo::Load() {
	if (config_file_.empty()) {
		LOG(ERROR) << "config_file is empty()";
		return -1;
	}

	XMLIniFile xml_ini;
	if(!xml_ini.LoadXMLIniFile(file_path_util::ToStringHack(config_file_).c_str())) {
		LOG(ERROR) << "parser config_file " << file_path_util::ToStringHack(config_file_) << " error.";
		return -1;
	}

  Destroy();
	
  bool result = true;

  CommNodeInfo comm_node;
  std::string addr_info;

  result = xml_ini.GetString("Server", "service_type", comm_node.service_type);
  CHECK(result) << "Parse [Server/service_type] item error";
  result = comm_node.CheckServiceType();
  CHECK(result) << "CheckServiceType error, service_type = " << comm_node.service_type;
  result = xml_ini.GetString("Server", "service_name", comm_node.service_name);
  CHECK(result) << "Parse [Server/service_name] item error";
  result = xml_ini.GetString("Server", "addr_info", addr_info);
  CHECK(result) << "Parse [Server/addr_info] item error";

  size_t pos = addr_info.find_first_of(':');
  comm_node.addr = addr_info.substr(0, pos);
  comm_node.port = addr_info.substr(pos+1, addr_info.length()-pos-1);

  xml_ini.GetString("Server", "auth_password", comm_node.auth_password, CommNodeInfo::kDefaultAuthPassword.c_str());
  xml_ini.GetInt("Server", "reconnected_time", comm_node.reconnected_timeout, CommNodeInfo::kDefaultReconnectedTimeout);

  comm_nodes_.push_back(comm_node);

	return 0;
}

void ConfigInfo::Destroy() {
  comm_nodes_.clear();
}
#endif
