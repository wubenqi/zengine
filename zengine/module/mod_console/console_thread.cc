// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_console/console_thread.h"

#include "base/basictypes.h"

#include "mod_console/console_listener.h"
//#include "base/console/base_console.h"


//LocalConsole g_local_console;

#ifndef OS_WIN
#include <termios.h>
#else
#include <windows.h>
#endif

ConsoleThread::ConsoleThread(ConsoleListener* console_listener) 
  : running_(false)
  , console_listener_(console_listener) {
}

ConsoleThread::~ConsoleThread() {
  Destroy();
}

void ConsoleThread::Initialize() {
  thread_.reset(new base::DelegateSimpleThread(this, "console_thread"));
  thread_->Start();
}

void ConsoleThread::Destroy() {
  if (running_) {
    running_ = false;

#ifdef OS_WIN
    /* write the return keydown/keyup event */
    DWORD dwTmp;
    INPUT_RECORD ir[2];
    ir[0].EventType = KEY_EVENT;
    ir[0].Event.KeyEvent.bKeyDown = TRUE;
    ir[0].Event.KeyEvent.dwControlKeyState = 288;
    ir[0].Event.KeyEvent.uChar.AsciiChar = 13;
    ir[0].Event.KeyEvent.wRepeatCount = 1;
    ir[0].Event.KeyEvent.wVirtualKeyCode = 13;
    ir[0].Event.KeyEvent.wVirtualScanCode = 28;
    ir[1].EventType = KEY_EVENT;
    ir[1].Event.KeyEvent.bKeyDown = FALSE;
    ir[1].Event.KeyEvent.dwControlKeyState = 288;
    ir[1].Event.KeyEvent.uChar.AsciiChar = 13;
    ir[1].Event.KeyEvent.wRepeatCount = 1;
    ir[1].Event.KeyEvent.wVirtualKeyCode = 13;
    ir[1].Event.KeyEvent.wVirtualScanCode = 28;
    WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), ir, 2, & dwTmp );

#endif

    printf( "Waiting for console thread to terminate....\n" );
    thread_->Join();
    printf( "Console shut down.\n" );
  }
}

static const char kCmdLinePrefix[] = "[client]$ ";


void ConsoleThread::Run() {
  // SetThreadName("Console Interpreter");
  size_t i = 0;
  size_t len;
  char cmd[300];
#ifndef OS_WIN
  fd_set fds;
  struct timeval tv;
#endif

  //Thread::Sleep(1000);

  running_ = true;
  while( running_ ) {
#ifdef OS_WIN
    // Read in single line from "stdin"
    memset( cmd, 0, sizeof( cmd ) ); 
    printf(kCmdLinePrefix);
    if( fgets( cmd, 300, stdin ) == NULL )
      continue;

    if( !running_ )
      break;
#else
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    FD_ZERO( &fds );
    FD_SET( STDIN_FILENO, &fds );
    if( select( 1, &fds, NULL, NULL, &tv ) <= 0 )
    {
      if(running_)	// timeout
        continue;
      else
        break;
    }

    // Read in single line from "stdin"
    memset( cmd, 0, sizeof( cmd ) ); 
    if( fgets( cmd, 300, stdin ) == NULL )
      continue;
#endif

    len = strlen(cmd);
    for( i = 0; i < len; ++i ) {
      if(cmd[i] == '\n' || cmd[i] == '\r')
        cmd[i] = '\0';
    }

    if (console_listener_) {
      console_listener_->OnConsoleInput(cmd);
    }
  }
}
