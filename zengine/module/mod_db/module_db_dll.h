// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_DB_MODULE_DB_DLL_H_
#define MOD_DB_MODULE_DB_DLL_H_
#pragma once

#ifdef ZENGINE_STATIC_LIB
#include <string>

void DllStartModule_DB(void);
void DllStopModule_DB(void);
const std::string& ModuleName_DB(void);
#endif

#endif
