// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_NET_MODULE_NET_DLL_H_
#define MOD_NET_MODULE_NET_DLL_H_

#ifdef ZENGINE_STATIC_LIB
#include <string>

void DllStartModule_Net(void);
void DllStopModule_Net(void);
const std::string& ModuleName_Net(void);
#endif


#endif
