// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/script/register_helper/base_register_helper.h"

#include "scriptengine/luatinker/lua_tinker.h"

#include "base/md5.h"
#include "base2/utf8_util.h"
#include "base/rsa_help.h"

namespace {

static const std::string kBaseModuleName("mod_base");

std::string UTF8ToNativeMB(const char* utf8) {
#if defined(OS_WIN)
  return base::UTF8ToNativeMB(utf8);
#else
  return utf8;
#endif      
}

std::string MD5String2(const std::string& str) {
  base::MD5Digest digest;
  base::MD5Sum(str.c_str(), str.length(), &digest);
  return base::MD5DigestToBase16(digest);
}

const char* ToString(const std::string& s) {
  return s.c_str();
  //lua_pushlstring(L, s.c_str(), s.length());
}

std::string EncryptByKey(const char* pubkey, const char* data) {
  char depubkey[1024];
  int  depubLen = 0;
  RSAHelper::Base64decode(pubkey, depubkey, depubLen);

  char pwd[128];
  strncpy(pwd, data, sizeof(pwd)-1);
  return RSAHelper::EncryptPassword(depubkey, depubLen, pwd);
}

// void 

void Luabind_Base_Register(lua_State* L) {
  lua_tinker::def(L, "MD5String", &MD5String2);
  lua_tinker::def(L, "UTF8ToNativeMB", &UTF8ToNativeMB);
  lua_tinker::def(L, "ToString", &ToString);
  lua_tinker::def(L, "EncryptByKey", &EncryptByKey);
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

namespace zengine {

void Base_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(kBaseModuleName, &Luabind_Base_Register);
}

}
