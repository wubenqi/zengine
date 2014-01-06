// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZCOMMON_CONFIG_UTIL_H_
#define ZCOMMON_CONFIG_UTIL_H_

#include <string>
#include <set>

// 网络连接节点配置信息
struct CommNodeInfo {
  static const std::string kNetType_TCPConnector;
  static const std::string kNetType_TCPAcceptor;
  static const int kDefaultReconnectedTimeout;
  static const std::string kDefaultAuthPassword;

  CommNodeInfo() {
    Destroy();
  }

  void Destroy() {
    net_type = kNetType_TCPConnector;
    auth_password = kDefaultAuthPassword;
    reconnected_timeout = kDefaultReconnectedTimeout;
    instance_name.clear();
    addr.clear();
    port.clear();
  }

  bool CheckNetType() {
    return (net_type==kNetType_TCPConnector || net_type==kNetType_TCPAcceptor)?true:false;
  }

  bool NetTypeIsTCPConnector() {
    return net_type == kNetType_TCPConnector;
  }
  bool NetTypeIsTCPAcceptor() {
    return net_type == kNetType_TCPAcceptor;
  }

  std::string instance_name;
  std::string addr;
  std::string port;

  std::string net_type;
  std::string auth_password;
  int reconnected_timeout;
};

struct DatabaseDescrInfo {
  DatabaseDescrInfo()
    : port(3306)
    , type(1)
    , max_conn_size(3) {
  }

  void Destroy() {
    instance_name.clear();
    host_name.clear();
    user_name.clear();
    password.clear();
    db_name.clear();
    port = 3306;
    type = 1;
    max_conn_size = 3;
  }

  std::string instance_name;
  std::string host_name;
  std::string user_name;
  std::string password;
  std::string db_name;
  int port;
  int type;
  int max_conn_size;
};

struct IpAddrInfo {
	IpAddrInfo(const std::string& _addr, const std::string& _port) 
		: addr(_addr)
		, port(_port) {
	}

	IpAddrInfo() {
	}

	void Destroy() {
		addr.clear();
		port.clear();
	}

	bool operator==(const IpAddrInfo& other) const {
		if (addr.empty() && port.empty()) {
			return false;
		}
		return addr==other.addr && port==other.port;
	}

	// Allows GURL to used as a key in STL (for example, a std::set or std::map).
	bool operator < (const IpAddrInfo& other) const {
		if( addr < other.addr ) {
			return true;
		} else if ( addr == other.addr) {
			return port < other.port;
		}
		return false;
	}
	std::string addr;			// IP地址
	std::string port;			// 端口号
};


typedef std::set<std::string> HostList;

#endif

