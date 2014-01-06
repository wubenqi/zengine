// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_ZENGINE_CONTEXT_H_
#define ZENGINE_ZENGINE_CONTEXT_H_
#pragma once

#include <string>
#include <vector>
#include "base/threading/platform_thread.h"
#include "base2/timer_manager.h"

#include "net/base/io_buffer.h"

class MessageLoop;
//class ZEngineDaemon;

//namespace net {
//class Reactor;
//}

//class ZEngineThread;
class ScriptManager;
class ZEngineContextManager;

class ZEngineContext : public TimerManager::Delegate {
public:
  enum ZEngineContextType {
    kZEngineContextType_Invalid = 0,
    kZEngineContextType_Main = 1,
    kZEngineContextType_IO = 2,
    kZEngineContextType_ZEngine = 3
  };
  
  static const std::string kMainInstanceName;
  static const std::string kIOInstanceName;


  ZEngineContext(ZEngineContextManager* context_manager, int context_type, base::PlatformThreadId instance_id, const std::string& instance_name, MessageLoop* message_loop)
    : context_type_(context_type)
    , instance_id_(instance_id)
    , instance_name_(instance_name)
    , message_loop_(message_loop)
    //, main_thread_(NULL)
    //, zengine_thread_(NULL)
    , script_manager_(NULL)
    , timer_manager_(NULL)
    , context_manager_(context_manager) {
  }

/*
  ZEngineContext()
    : context_type_(kZEngineContextType_Invalid)
    , instance_id_(base::kInvalidThreadId)
    , message_loop_(NULL)
    , script_manager_(NULL)
    , timer_manager_(NULL)
    , context_manager_(NULL) {
  }

  ZEngineContext(ZEngineContextManager* context_manager, base::PlatformThreadId instance_id, const std::string& instance_name, ZEngineDaemon* daemon)
    : context_type_(kZEngineContextType_Main)
    , instance_id_(instance_id)
    , instance_name_(instance_name)
    , io_thread_(NULL)
    , main_thread_(daemon)
    , zengine_thread_(NULL)
    , script_manager_(NULL)
    , timer_manager_(NULL)
    , context_manager_(context_manager) {
  }

  ZEngineContext(ZEngineContextManager* context_manager, base::PlatformThreadId instance_id, const std::string& instance_name, ZEngineThread* zthread)
    : context_type_(kZEngineContextType_ZEngine)
    , instance_id_(instance_id)
    , instance_name_(instance_name)
    , io_thread_(NULL)
    , main_thread_(NULL)
    , zengine_thread_(zthread)
    , script_manager_(NULL)
    , timer_manager_(NULL)
    , context_manager_(context_manager) {
  }
 */

  virtual ~ZEngineContext() {
  }

  bool Initialize();
  void Destroy();

  //
  bool PostTaskByInstanceName(const std::string& instance_name, const std::string& task_data);
  bool PostTaskByInstanceID(base::PlatformThreadId instance_id, const std::string& task_data);
  bool PostTaskToIOThread(const std::string& task_data);
  void PostTaskToMainThread(const std::string& task_data);
  void PostTask(const std::string& task_data);

  // 定时器应用
  void SetTimer(uint32 timer_id, int tm);
  void KillTimer(uint32 timer_id);

  // 获取属性数据
  int context_type() {
    return context_type_;
  }

  const std::string& instance_name() const {
    return instance_name_;
  }

  int instance_id() {
    return instance_id_;
  }

  //net::Reactor* io_thread() {
  //  return io_thread_;
  //}

  //ZEngineDaemon* main_thread() {
  //  return main_thread_;
  //}

  //ZEngineThread* zengine_thread() {
  //  return zengine_thread_;
  //}

  MessageLoop* message_loop() {
    return message_loop_;
  }

  ScriptManager* script_manager()  {
    return script_manager_;
  }

protected:
  // 定时器时间
  virtual void OnTimer(uint32 timer_id, int tm);
  void OnTaskDataReceived(std::string task_data);
  //MessageLoop* message_loop();

private:
  int context_type_;
  std::string instance_name_;
  base::PlatformThreadId instance_id_;

  //net::Reactor* io_thread_;
  //ZEngineDaemon* main_thread_;
  //ZEngineThread* zengine_thread_;
  MessageLoop* message_loop_;
  ScriptManager* script_manager_;

  ZEngineContextManager* context_manager_;
  TimerManager* timer_manager_;
};


#endif
