// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef BASE_CONSOLE_CONSOLE_THREAD_H_
#define BASE_CONSOLE_CONSOLE_THREAD_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/threading/simple_thread.h"

class ConsoleListener;
class ConsoleThread : public base::DelegateSimpleThread::Delegate {
public:
  ConsoleThread(ConsoleListener* console_listener);
  ~ConsoleThread();

  void Initialize();
  void Destroy();

  virtual void Run();

private:
  bool running_;
  ConsoleListener* console_listener_;
  scoped_ptr<base::DelegateSimpleThread> thread_;
};


#endif
