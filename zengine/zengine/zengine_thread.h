// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_ZENGINE_THREAD_H_
#define ZENGINE_ZENGINE_THREAD_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"

#include "net/base/io_buffer.h"
#include "zengine/script/script_manager.h"

class ZEngineContext;
//class ZEngineContextManager;
class ZEngineThread : public base::Thread {
public:
  ZEngineThread(const std::string& thread_name);
  virtual ~ZEngineThread();

  void OnTaskDataReceived(net::IOBufferPtr task_data);

protected:
  // ´Óbase::Thread¼Ì³Ð
  virtual void Init();
  virtual void CleanUp();

private:
  //scoped_ptr<ScriptManager> script_manager_;
  //ZEngineContextManager* context_manager_;
  ZEngineContext* context_;
};


#endif
