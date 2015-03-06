// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_XML_MODULE_XML_DLL_H_
#define MOD_XML_MODULE_XML_DLL_H_

#ifdef ZENGINE_STATIC_LIB
#include <string>

void DllStartModule_Xml(void);
void DllStopModule_Xml(void);
const std::string& ModuleName_Xml(void);
#endif


#endif
