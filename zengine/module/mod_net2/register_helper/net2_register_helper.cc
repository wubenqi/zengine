// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_net2/register_helper/net2_register_helper.h"

#include "base/basictypes.h"

#include "luabind/luabind.hpp"
#include "luabind/out_value_policy.hpp"
#include "luabind/raw_policy.hpp"

#include "zcommon/io_handler_util.h"
#include "zengine/zengine_context.h"
#include "mod_net/znet_comm_handler.h"

#include "mod_net2/module_net2_dll.h"

bool CreateTCPConnector2(const std::string& addr, const std::string& port, int reconnector_timeout, const std::string& instance_name);
bool CreateTCPAcceptor2(const std::string& addr, const std::string& port, const std::string& instance_name);

void SendMessageData2(ZNetCommHandler* ih, uint16 message_type, const google::protobuf::Message* message) {
  return SendMessageData(ih, message_type, message);
}

void SendNullMessageData2(ZNetCommHandler* ih, uint16 message_type) {
  return SendNullMessageData(ih, message_type);
}

void SendSessionMessageData2(ZNetCommHandler* ih, uint64 session_id, uint16 message_type, const google::protobuf::Message* message) {
  return SendSessionMessageData(ih, session_id, message_type, message);
}

void SendSessionNullMessageData2(ZNetCommHandler* ih, uint64 session_id, uint16 message_type) {
  return SendSessionNullMessageData(ih, session_id, message_type);
}

void SendIOBufferData2(ZNetCommHandler* ih, uint16 message_type, const IOBuffer2* io_buffer2) {
  return SendIOBufferData(ih, message_type, io_buffer2);
}

void SendSessionIOBufferData2(ZNetCommHandler* ih, uint64 session_id, uint16 message_type, const IOBuffer2* io_buffer2) {
  return SendSessionIOBufferData(ih, session_id, message_type, io_buffer2);
}

void SendSessionStringData2(ZNetCommHandler* ih, uint64 session_id, uint16 message_type, const std::string& data) {
  return SendSessionStringData(ih, session_id, message_type, data);

}

//void TestSetSessionID(uint64 session_id) {
//  LOG(INFO) << "TestSetSessionID - " << session_id;
//}

namespace {

//void GetSessionID(ZNetCommHandler* znet, lua_State* L) {
//  net::SessionID session_id = znet->GetSessionID();
//  lua_pushlstring(L, reinterpret_cast<const char*>(&(session_id)), sizeof(session_id));
//}

void Luabind_Net2_Register(lua_State* L) {
  luabind::module(L) [
    //luabind::class_<int64>("int64")
    //  .def(luabind::constructor<int64>()),

    luabind::def("CreateTCPConnector", &CreateTCPConnector2),
    luabind::def("CreateTCPAcceptor", &CreateTCPAcceptor2),

    //luabind::def("TestSetSessionID", &TestSetSessionID),

    luabind::class_<ZNetCommHandler>("ZNetCommHandler")
      .def("GetAddressString", &ZNetCommHandler::GetAddressString)
      .def("GetReactorID", &ZNetCommHandler::GetReactorID)
      .def("GetSessionID", &ZNetCommHandler::GetSessionID)
      .def("GetReadableSessionID", &ZNetCommHandler::GetReadableSessionID)
      .def("GetContext", &ZNetCommHandler::GetContext)
      .def("GetInstanceName", &ZNetCommHandler::GetInstanceName)
      .def("GetNetType", &ZNetCommHandler::GetNetType)
      .def("set_conn_state", &ZNetCommHandler::set_conn_state)
      .def("conn_state", &ZNetCommHandler::conn_state)
      .def("SendMessageData", &SendMessageData2)
      .def("SendNullMessageData", &SendNullMessageData2)
      .def("SendSessionMessageData", &SendSessionMessageData2)
      .def("SendSessionNullMessageData", &SendSessionNullMessageData2)
      .def("SendIOBufferData", &SendIOBufferData2)
      .def("SendSessionIOBufferData", &SendSessionIOBufferData2)
      .def("SendSessionStringData", &SendSessionStringData2)
  ];
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

namespace zengine {

void Net2_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_Net2(), &Luabind_Net2_Register);
}

}
