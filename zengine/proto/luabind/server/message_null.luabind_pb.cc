// Generated By protoc-gen-luabind.  DO NOT EDIT!
//

#include "message_null.pb.h"

#include "luabind/luabind.hpp"
#include "luabind/enum.hpp"
#include "luabind/out_value_policy.hpp"


void LuabindPB_MessageNull_Register(lua_State* L) {
  luabind::module(L) [
    luabind::class_< ::hammer::message::MessageNull, ::google::protobuf::Message, ::google::protobuf::MessageLite>("MessageNull")
      .def(luabind::constructor<>())
  ];
}

//////////////////////////////////////////////////
void LuabindPB_message_null_RegisterAll(lua_State* L) {
  LuabindPB_MessageNull_Register(L);
}

//////////////////////////////////////////////////
