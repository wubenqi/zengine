// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef BASE_CONSOLE_BASE_CONSOLE_H_
#define BASE_CONSOLE_BASE_CONSOLE_H_
#pragma once

class BaseConsole {
public:
  virtual ~BaseConsole() {}
  virtual void Write(const char * format, ...) = 0;
  virtual void WriteNA(const char * format) = 0;
};

class LocalConsole : public BaseConsole {
public:
  void Write(const char * format, ...);
  void WriteNA(const char * format);
};

//class RemoteConsole : public BaseConsole {
//	ConsoleSocket * m_pSocket;
//public:
//	RemoteConsole(ConsoleSocket* pSocket);
//	void Write(const char * format, ...);
//	void WriteNA(const char * format);
//};

#endif
