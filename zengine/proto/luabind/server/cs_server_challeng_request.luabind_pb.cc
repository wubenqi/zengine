// Generated By protoc-gen-luabind.  DO NOT EDIT!
//

#include "cs_server_challeng_request.pb.h"

#include "luabind/luabind.hpp"
#include "luabind/enum.hpp"
#include "luabind/out_value_policy.hpp"


void LuabindPB_CSServerChallengeRequest_Register(lua_State* L) {
  luabind::module(L) [
    luabind::class_< ::hammer::message::CSServerChallengeRequest, ::google::protobuf::Message, ::google::protobuf::MessageLite>("CSServerChallengeRequest")
      .def(luabind::constructor<>())
      .property("name", (const std::string& (::hammer::message::CSServerChallengeRequest::*)(void) const)&::hammer::message::CSServerChallengeRequest::name, (void (::hammer::message::CSServerChallengeRequest::*)(const char*))&::hammer::message::CSServerChallengeRequest::set_name)
      .property("remote_password", (const std::string& (::hammer::message::CSServerChallengeRequest::*)(void) const)&::hammer::message::CSServerChallengeRequest::remote_password, (void (::hammer::message::CSServerChallengeRequest::*)(const char*))&::hammer::message::CSServerChallengeRequest::set_remote_password)
  ];
}

//////////////////////////////////////////////////
void LuabindPB_cs_server_challeng_request_RegisterAll(lua_State* L) {
  LuabindPB_CSServerChallengeRequest_Register(L);
}

//////////////////////////////////////////////////