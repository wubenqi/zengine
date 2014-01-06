// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_NET2_MODULE_NET_PLUGIN_H_
#define MOD_NET2_MODULE_NET_PLUGIN_H_
#pragma once

#include "base/memory/singleton.h"
#include "net/engine/net_engine_manager.h"
#include "net/engine/tcp_acceptor.h"
#include "net/engine/tcp_connector.h"

#include "zcommon/config_util.h"
#include "zengine/zengine_plugin.h"

namespace zengine {

class ModuleNet2Plugin : public Plugin, public net::NetEngineManagerDelegate {
public:
  ModuleNet2Plugin();
  virtual ~ModuleNet2Plugin();

  static const std::string& GetModuleNet2Name();

  //////////////////////////////////////////////////////////////////////////
  virtual const std::string& GetPluginName() const;
  virtual void Install();
  virtual void Initialize();
  virtual void Shutdown();
  virtual void Uninstall();

  bool CreateTCPConnector(const std::string& addr, const std::string& port, int reconnector_timeout, const std::string& instance_name);
  bool CreateTCPAcceptor(const std::string& addr, const std::string& port, const std::string& instance_name);

protected:
  // ю╢вт net::NetEngineManagerDelegate
  virtual void OnNetEngineCreate(net::Reactor* reactor);
  virtual void OnNetEngineDestroy(net::Reactor* reactor);

private:
  net::NetEngineManager* net_engine_manager_;

  std::vector<net::TCPAcceptorPtr> accs_;
  std::vector<net::TCPConnectorPtr> conns_;

  std::vector<CommNodeInfo*> comm_nodes_;
};

class ModuleNet2PluginFactory : public PluginFactory {
public:
  static ModuleNet2PluginFactory* GetInstance() {
    return Singleton<ModuleNet2PluginFactory>::get();
  }

  virtual const std::string& GetPluginName() const;
  virtual Plugin* CreateInstance();
  virtual void DestroyInstance(Plugin* module);

private:
  friend struct DefaultSingletonTraits<ModuleNet2PluginFactory>;

  ModuleNet2PluginFactory() {}
  virtual ~ModuleNet2PluginFactory ()  {}
};

}

#endif
