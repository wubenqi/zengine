// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZGATED_CONFIG_INFO_H_
#define ZGATED_CONFIG_INFO_H_

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/memory/singleton.h"

#include "zcommon/config_util.h"

//// 网络连接节点配置信息
//struct CommNodeInfo {
//	void Destroy() {
//		addr.Destroy();
//		service_host.clear();
//		conn_passwd.clear();
//		reconnected_time = 600;
//	}
//
//	IpAddrInfo addr;
//	std::string service_host;
//	std::string conn_passwd;
//	int reconnected_time;
//};

class ConfigInfo {
public:
	static ConfigInfo* GetInstance() {
		return Singleton<ConfigInfo>::get();
	}

	int Initialize(const FilePath& config_file);
	int Load();
	void Destroy();

	inline CommNodeInfo& client_node() { return client_node_; }
  inline CommNodeInfo& server_node() { return server_node_; }

	inline const FilePath& GetExePath() { return exe_file_; }

private:
	ConfigInfo() {
		Destroy();
	}

	~ConfigInfo() {
	}

	friend class Singleton<ConfigInfo>;
	friend struct DefaultSingletonTraits<ConfigInfo>;

	DISALLOW_COPY_AND_ASSIGN(ConfigInfo);

  CommNodeInfo client_node_;
	CommNodeInfo server_node_;
	//CommNodeInfo login_node_;
	//CommNodeInfo name_node_;
	//DatabaseDescrInfo db_;

	FilePath config_file_;
	FilePath exe_file_;
};

#endif
