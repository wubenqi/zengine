// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_CONSOLE_MODULE_HTML_DLL_H_
#define MOD_CONSOLE_MODULE_HTML_DLL_H_
#pragma once

#ifdef ZENGINE_STATIC_LIB
#include <string>

void DllStartModule_Console(void);
void DllStopModule_Console(void);
const std::string& ModuleName_Console(void);
#endif

#endif
