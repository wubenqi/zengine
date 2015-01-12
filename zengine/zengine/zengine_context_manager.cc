// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/zengine_context_manager.h"

#include "zengine/zengine_context.h"

ZEngineContext* ZEngineContextManager::CreateContext(int context_type, base::PlatformThreadId instance_id, const std::string& instance_name, base::MessageLoop* message_loop) {
  ZEngineContext* context = new ZEngineContext(this, context_type, instance_id, instance_name, message_loop);
  context->Initialize();
  AddContext(context);
  return context;
}

//ZEngineContext* ZEngineContextManager::CreateContext(base::PlatformThreadId instance_id, const std::string& instance_name, ZEngineDaemon* daemon) {
//  ZEngineContext* context = new ZEngineContext(this, instance_id, instance_name, daemon);
//  context->Initialize();
//  AddContext(context);
//  return context;
//}
//
//ZEngineContext* ZEngineContextManager::CreateContext(base::PlatformThreadId instance_id, const std::string& instance_name, ZEngineThread* zthread) {
//  ZEngineContext* context = new ZEngineContext(this, instance_id, instance_name, zthread);
//  context->Initialize();
//  AddContext(context);
//  return context;
//}

void ZEngineContextManager::AddContext(ZEngineContext* context) {
  //if (context->context_type()==ZEngineContext::kZEngineContextType_Main) {
  //  main_thread_ = context->main_thread();
  //} else if (context->context_type()==ZEngineContext::kZEngineContextType_IO) {
  //  io_thread_ = context->io_thread();
  //}
  contexts_.push_back(context);
}

void ZEngineContextManager::RemoveContext(ZEngineContext* context) {
  //if (context->context_type()==ZEngineContext::kZEngineContextType_Main) {
  //  main_thread_ = NULL;
  //} else if (context->context_type()==ZEngineContext::kZEngineContextType_IO) {
  //  io_thread_ = NULL;
  //}
  for (size_t i=0; i<contexts_.size(); ++i) {
    if (context->instance_id() == contexts_[i]->instance_id()) {
      contexts_.erase(contexts_.begin()+i);
      break;
    }
  }
}

ZEngineContext* ZEngineContextManager::LookupContext(const std::string& instance_name) {
  ZEngineContext* context = NULL;
  for (size_t i=0; i<contexts_.size(); ++i) {
    if (instance_name == contexts_[i]->instance_name()) {
      context = contexts_[i];
      break;
    }
  }
  return context;
}

ZEngineContext* ZEngineContextManager::LookupContext(base::PlatformThreadId instance_id) {
  ZEngineContext* context = NULL;
  for (size_t i=0; i<contexts_.size(); ++i) {
    if (instance_id == contexts_[i]->instance_id()) {
      context = contexts_[i];
      break;
    }
  }
  return context;
}

ZEngineContext* ZEngineContextManager::LookupMainContext() {
  ZEngineContext* context = NULL;
  for (size_t i=0; i<contexts_.size(); ++i) {
    if (ZEngineContext::kZEngineContextType_Main == contexts_[i]->context_type()) {
      context = contexts_[i];
      break;
    }
  }
  return context;
}

ZEngineContext* ZEngineContextManager::LookupIOContext() {
  ZEngineContext* context = NULL;
  for (size_t i=0; i<contexts_.size(); ++i) {
    if (ZEngineContext::kZEngineContextType_IO == contexts_[i]->context_type()) {
      context = contexts_[i];
      break;
    }
  }
  return context;
}
