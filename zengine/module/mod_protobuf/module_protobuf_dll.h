// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_PROTOBUF_MODULE_PROTOBUF_DLL_H_
#define MOD_PROTOBUF_MODULE_PROTOBUF_DLL_H_
#pragma once

#ifdef ZENGINE_STATIC_LIB
#include <string>

void DllStartModule_Protobuf(void);
void DllStopModule_Protobuf(void);
const std::string& ModuleName_Protobuf(void);
#endif


#endif
