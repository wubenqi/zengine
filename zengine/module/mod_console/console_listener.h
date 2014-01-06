// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef BASE_CONSOLE_CONSOLE_LISTENER_H_
#define BASE_CONSOLE_CONSOLE_LISTENER_H_
#pragma once

#include "mod_console/base_console.h"

class ConsoleListener {
public:
	ConsoleListener(BaseConsole* console)
		: console_(console) {}

	virtual ~ConsoleListener() {}

	virtual void OnConsoleInput(const char* input) {}

protected:
	BaseConsole* console_;
};

#endif
