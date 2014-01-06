// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_NET2_MODULE_NET_DLL_H_
#define MOD_NET2_MODULE_NET_DLL_H_
#pragma once

#ifdef ZENGINE_STATIC_LIB
#include <string>

void DllStartModule_Net2(void);
void DllStopModule_Net2(void);
const std::string& ModuleName_Net2(void);
#endif


#endif
