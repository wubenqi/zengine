// Generated By protoc-gen-luabind.  DO NOT EDIT!
//

#include "cs_logout_req.pb.h"

#include "luabind/luabind.hpp"
#include "luabind/enum.hpp"
#include "luabind/out_value_policy.hpp"


void LuabindPB_CSLogoutReq_Register(lua_State* L) {
  luabind::module(L) [
    luabind::class_< ::hammer::message::CSLogoutReq, ::google::protobuf::Message, ::google::protobuf::MessageLite>("CSLogoutReq")
      .def(luabind::constructor<>())
      .property("player_id", (::google::protobuf::uint32 (::hammer::message::CSLogoutReq::*)(void) const)&::hammer::message::CSLogoutReq::player_id, (void (::hammer::message::CSLogoutReq::*)(::google::protobuf::uint32))&::hammer::message::CSLogoutReq::set_player_id)
      .property("type", (::google::protobuf::uint32 (::hammer::message::CSLogoutReq::*)(void) const)&::hammer::message::CSLogoutReq::type, (void (::hammer::message::CSLogoutReq::*)(::google::protobuf::uint32))&::hammer::message::CSLogoutReq::set_type)
  ];
}

//////////////////////////////////////////////////
void LuabindPB_cs_logout_req_RegisterAll(lua_State* L) {
  LuabindPB_CSLogoutReq_Register(L);
}

//////////////////////////////////////////////////