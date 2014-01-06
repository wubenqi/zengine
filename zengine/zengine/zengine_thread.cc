// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/zengine_thread.h"

#include "zengine/zengine_context.h"
#include "zengine/zengine_context_manager.h"

ZEngineThread::ZEngineThread(const std::string& thread_name)
  : base::Thread(thread_name.c_str()) {

}

ZEngineThread::~ZEngineThread() {
}

void ZEngineThread::Init() {
  context_ = ZEngineContextManager::GetInstance()->CreateContext(ZEngineContext::kZEngineContextType_ZEngine, base::PlatformThread::CurrentId(), thread_name(), message_loop());
  context_->script_manager()->ExeScript_OnThreadInitialize(context_);
}

void ZEngineThread::CleanUp() {
  context_->script_manager()->ExeScript_OnThreadDestroy(context_);
  context_->Destroy();
  context_ = NULL;
  //script_manager_->ExeScript_OnThreadDestroy(zengine_, thread_name());
  //script_manager_.reset();
}

void ZEngineThread::OnTaskDataReceived(net::IOBufferPtr task_data) {
  if (context_) {
    context_->script_manager()->ExeScript_OnTaskDataReceived(context_, task_data->data());
  }
}
