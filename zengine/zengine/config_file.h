// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_CONFIG_FILE_H_
#define ZENGINE_CONFIG_FILE_H_
#pragma once

#include "zengine/zengine_types.h"
#include "base/file_path.h"
#include "base/memory/singleton.h"

#include <map>

class ConfigFile {
public:
  typedef std::pair<std::string, std::string> KeyValuePair;
  typedef std::vector<KeyValuePair> KeyValuePairList;
  typedef std::map<std::string, KeyValuePairList> SectionMap;

  static ConfigFile* GetInstance() {
    return Singleton<ConfigFile>::get();
  }

  int Initialize(const FilePath& config_file_path);
  int Load();
  void Destroy();

  inline FilePath& config_file_path() { return config_file_path_; }
  inline const FilePath& config_file_path() const { return config_file_path_; }

  const char* GetString(const char* section, const char* key, const char* default_val=NULL) const;
  int GetInt(const char* section, const char* key, int default_val=0) const;
  zengine::CStringVector GetCStringList(const char* section, const char* key) const;
  zengine::StringVector GetStringList(const char* section, const char* key) const;

  // 取一section下所有的key
  zengine::StringVector GetSectionKey(const char *section) const;
  bool CheckBySection(const char* section) const;
  bool CheckBySectionAndKey(const char* section, const char* key) const;
  bool CheckByValue(const char* section, const char* key, const char* val) const;

  // 得到所有section的名字
  zengine::StringVector GetSectionName() const;

private:
  friend struct DefaultSingletonTraits<ConfigFile>;
  ConfigFile() {}
  ~ConfigFile() {}

  SectionMap config_map_;

  DISALLOW_COPY_AND_ASSIGN(ConfigFile);

  FilePath config_file_path_;
};



#if 0
#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/memory/singleton.h"

class ConfigInfo {
public:
	static ConfigInfo* GetInstance() {
		return Singleton<ConfigInfo>::get();
	}

	int Initialize(const FilePath& config_file);
	int Load();
	void Destroy();

  inline const FilePath& GetExePath() { return exe_file_; }
  inline std::vector<CommNodeInfo>& GetCommNodeInfos() { return comm_nodes_; }

private:
	ConfigInfo() {
		Destroy();
	}

	~ConfigInfo() {
	}

	friend class Singleton<ConfigInfo>;
	friend struct DefaultSingletonTraits<ConfigInfo>;

	DISALLOW_COPY_AND_ASSIGN(ConfigInfo);

	FilePath config_file_;
	FilePath exe_file_;

  std::vector<CommNodeInfo> comm_nodes_;
};
#endif

#endif
