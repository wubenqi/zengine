// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_protobuf/register_helper/protobuf_register_helper.h"

#include "base/memory/scoped_ptr.h"

#include "luabind/luabind.hpp"
#include "luabind/out_value_policy.hpp"
#include "luabind/raw_policy.hpp"

#include "base2/io_buffer2.h"
#include <google/protobuf/message.h>

#include "mod_protobuf/module_protobuf_dll.h"

namespace {

bool ParseFromIOBuffer(google::protobuf::MessageLite* message_lite, const IOBuffer2& data) {
  //if (!data.Check()) {
  //  return false;
  //}
	return message_lite->ParseFromArray(data.read_ptr(), data.size()-data.read_pos());
}

IOBuffer2* SerializeToIOBuffer(const google::protobuf::MessageLite* message_lite) {
	scoped_ptr<IOBuffer2> output(new IOBuffer2(message_lite->ByteSize()));
	return message_lite->SerializeToArray(output->data(), output->size()) ? output.release() : NULL;
}

bool ParseFromString(google::protobuf::MessageLite* message_lite, const std::string& data) {
  return message_lite->ParseFromString(data);
}

std::string SerializeToString(const google::protobuf::MessageLite* message_lite) {
  std::string data;
  message_lite->SerializeToString(&data);
  return data;
}
//
//void ToString(IOBuffer2* io_buffer, lua_State* L) {
//	lua_pushlstring(L, io_buffer->GetData(), io_buffer->GetDataLen());
//}

}

namespace {


void Luabind_Protobuf_Register(lua_State* L) {
  luabind::module(L) [
    luabind::class_<google::protobuf::MessageLite>("MessageLite")
      .def("ParseFromIOBuffer", &ParseFromIOBuffer)
      .def("SerializeToIOBuffer", &SerializeToIOBuffer)
      .def("ParseFromString", &ParseFromString)
      .def("SerializeToString", &SerializeToString)
      .def("ByteSize", &google::protobuf::MessageLite::ByteSize)
      .def("Clear", &google::protobuf::MessageLite::Clear),

      luabind::class_<google::protobuf::Message, google::protobuf::MessageLite>("Message")
      .def("PrintDebugString", &google::protobuf::Message::PrintDebugString)
      .def("Utf8DebugString", &google::protobuf::Message::Utf8DebugString)
      .def("CopyFrom", &google::protobuf::Message::CopyFrom)
  ];
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

//void LuabindPB_RegisterAll(lua_State* L);

namespace zengine {

void Protobuf_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_Protobuf(), &Luabind_Protobuf_Register);
  //LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_Protobuf(), &LuabindPB_RegisterAll);
}

}
