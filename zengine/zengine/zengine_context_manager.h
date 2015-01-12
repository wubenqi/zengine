// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_ZENGINE_CONTEXT_MANAGER_H_
#define ZENGINE_ZENGINE_CONTEXT_MANAGER_H_
#pragma once

#include <string>
#include <vector>
#include "base/threading/platform_thread.h"
#include "base/memory/singleton.h"

//class ZEngineDaemon;
namespace base {
class MessageLoop;
}

class ZEngineContext;
//class ZEngineThread;
class ZEngineContextManager {
public:
  static ZEngineContextManager* GetInstance() {
    return Singleton<ZEngineContextManager>::get();
  }

  ZEngineContext* CreateContext(int context_type, base::PlatformThreadId instance_id, const std::string& instance_name, base::MessageLoop* message_loop);
  //ZEngineContext* CreateContext(base::PlatformThreadId instance_id, const std::string& instance_name, ZEngineDaemon* daemon);
  //ZEngineContext* CreateContext(base::PlatformThreadId instance_id, const std::string& instance_name, ZEngineThread* zthread);

  void Destroy() {
    //main_thread_ = NULL;
    //io_thread_ = NULL;
    contexts_.clear();
  }

  ZEngineContext* LookupContext(const std::string& instance_name);
  ZEngineContext* LookupContext(base::PlatformThreadId instance_id);

  ZEngineContext* LookupMainContext();
  ZEngineContext* LookupIOContext();

private:
  ZEngineContextManager(/*const std::string& app_name*/)
    /*: main_thread_(NULL)
    , io_thread_(NULL)
    , app_name_(app_name)*/ {
  }
  ~ZEngineContextManager() {
  }

  friend class ZEngineContext;
  friend struct DefaultSingletonTraits<ZEngineContextManager>;

  void AddContext(ZEngineContext* context);
  void RemoveContext(ZEngineContext* context);

  std::vector<ZEngineContext*> contexts_;
  //ZEngineDaemon* main_thread_;
  //net::Reactor* io_thread_;
  //const std::string& app_name_;
};

#endif
