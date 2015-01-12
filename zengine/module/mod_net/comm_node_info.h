// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_NET_COMM_NODE_INFO_H_
#define MOD_NET_COMM_NODE_INFO_H_

#include <string>
#include <set>

// 网络连接节点配置信息
struct CommNodeInfo {
  static const std::string kNetType_TCPConnector;
  static const std::string kNetType_TCPAcceptor;
  static const int kDefaultReconnectedTimeout;

  CommNodeInfo() {
    Destroy();
  }

  void Destroy() {
    net_type = kNetType_TCPConnector;
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
  int reconnected_timeout;
};

#endif

