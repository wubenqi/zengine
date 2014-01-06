// Copyright 2010, www.lelewan.com
// All rights reserved
//
// 此代码为自动生成，请不要随意改动
//


struct lua_State;

void LuabindPB_const_message_types_RegisterAll(lua_State* L);
void LuabindPB_error_code_RegisterAll(lua_State* L);
void LuabindPB_cs_auth_challenge_RegisterAll(lua_State* L);
void LuabindPB_cs_auth_proof_RegisterAll(lua_State* L);
void LuabindPB_cs_logout_req_RegisterAll(lua_State* L);
void LuabindPB_cs_server_challeng_request_RegisterAll(lua_State* L);
void LuabindPB_message_null_RegisterAll(lua_State* L);
void LuabindPB_sc_auth_challenge_RegisterAll(lua_State* L);
void LuabindPB_sc_auth_proof_RegisterAll(lua_State* L);
void LuabindPB_sc_ret_req_result_RegisterAll(lua_State* L);
void LuabindPB_sc_server_challeng_ack_RegisterAll(lua_State* L);


void LuabindPB_RegisterAll(lua_State* L) {
  LuabindPB_const_message_types_RegisterAll(L);
  LuabindPB_error_code_RegisterAll(L);
  LuabindPB_cs_auth_challenge_RegisterAll(L);
  LuabindPB_cs_auth_proof_RegisterAll(L);
  LuabindPB_cs_logout_req_RegisterAll(L);
  LuabindPB_cs_server_challeng_request_RegisterAll(L);
  LuabindPB_message_null_RegisterAll(L);
  LuabindPB_sc_auth_challenge_RegisterAll(L);
  LuabindPB_sc_auth_proof_RegisterAll(L);
  LuabindPB_sc_ret_req_result_RegisterAll(L);
  LuabindPB_sc_server_challeng_ack_RegisterAll(L);

}

#if defined(WIN32)
#include <windows.h>
#else
#define WINAPI
#endif

extern "C" void WINAPI LuabindPBModule_Register(lua_State* L) {
  LuabindPB_RegisterAll(L);
}
