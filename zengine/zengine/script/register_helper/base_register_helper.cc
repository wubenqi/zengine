// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/script/register_helper/base_register_helper.h"

#include "luabind/luabind.hpp"
#include "luabind/out_value_policy.hpp"
#include "luabind/raw_policy.hpp"

#include "base/md5.h"
#include "base2/utf8_util.h"
#include "base2/io_buffer2.h"

namespace {

static const std::string kBaseModuleName("mod_base");

std::string UTF8ToNativeMB(const std::string& utf8) {
#if defined(OS_WIN)
  return base::UTF8ToNativeMB(utf8);
#else
  return utf8;
#endif      
}


void io_buffer2_read_ptr_string(IOBuffer2* io_buffer, lua_State* L) {
  lua_pushlstring(L, io_buffer->read_ptr(), io_buffer->readable_size());
}

void io_buffer2_write_ptr_string(IOBuffer2* io_buffer, lua_State* L) {
  lua_pushlstring(L, io_buffer->write_ptr(), io_buffer->writable_size());
}

void io_buffer2_data_string(IOBuffer2* io_buffer, lua_State* L) {
  lua_pushlstring(L, io_buffer->data(), io_buffer->size());
}

void io_buffer2_read_string(IOBuffer2* io_buffer, lua_State* L) {
  std::string val;
  if(io_buffer->ReadString(&val)) {
    lua_pushlstring(L, val.c_str(), val.size());
  } else {
    lua_pushnil(L);
  }
}

void io_buffer2_read_peek_string(IOBuffer2* io_buffer, lua_State* L) {
  std::string val;
  if(io_buffer->ReadPeekString(&val)) {
    lua_pushlstring(L, val.c_str(), val.size());
  } else {
    lua_pushnil(L);
  }
}

void io_buffer2_read_offet_string(IOBuffer2* io_buffer, int offset, lua_State* L) {
  std::string val;
  if(io_buffer->ReadOffsetString(&val, offset)) {
    lua_pushlstring(L, val.c_str(), val.size());
  } else {
    lua_pushnil(L);
  }
}

std::string MD5String2(const std::string& str) {
  base::MD5Digest digest;
  base::MD5Sum(str.c_str(), str.length(), &digest);
  return base::MD5DigestToBase16(digest);
}

void Luabind_Base_Register(lua_State* L) {
  luabind::module(L) [
    luabind::class_<IOBuffer2>("IOBuffer")
      .def(luabind::constructor<>())
      .def(luabind::constructor<int>())
      .def(luabind::constructor<const char*, int>())
      .def("data_string", &io_buffer2_data_string)
      .def("size", &IOBuffer2::size)
      .def("read_ptr_string", &io_buffer2_read_ptr_string)
      .def("write_ptr_string", &io_buffer2_write_ptr_string)

      .def("readable_size", &IOBuffer2::readable_size)
      .def("read_pos", &IOBuffer2::read_pos)
      .def("skip_read_pos", &IOBuffer2::skip_read_pos)
      .def("seek_read_pos", &IOBuffer2::seek_read_pos)

      .def("writable_size", &IOBuffer2::writable_size)
      .def("write_pos", &IOBuffer2::write_pos)
      .def("skip_write_pos", &IOBuffer2::skip_write_pos)
      .def("seek_write_pos", &IOBuffer2::seek_write_pos)

      .def("Resize", &IOBuffer2::Resize)
      .def("Clone", &IOBuffer2::Clone)
      .def("DumpHexEncode", &IOBuffer2::DumpHexEncode)

      .def("ReadUint8", &IOBuffer2::ReadUint8, luabind::pure_out_value(_2))
      .def("ReadUint16", &IOBuffer2::ReadUint16, luabind::pure_out_value(_2))
      .def("ReadUint32", &IOBuffer2::ReadUint32, luabind::pure_out_value(_2))
      .def("ReadUint64", &IOBuffer2::ReadUint64, luabind::pure_out_value(_2))
      .def("ReadString", &io_buffer2_read_string)

      .def("ReadPeekUint8", &IOBuffer2::ReadPeekUint8, luabind::pure_out_value(_2))
      .def("ReadPeekUint16", &IOBuffer2::ReadPeekUint16, luabind::pure_out_value(_2))
      .def("ReadPeekUint32", &IOBuffer2::ReadPeekUint32, luabind::pure_out_value(_2))
      .def("ReadPeekUint64", &IOBuffer2::ReadPeekUint64, luabind::pure_out_value(_2))
      .def("ReadPeekString", &io_buffer2_read_peek_string)

      .def("ReadOffsetUint8", &IOBuffer2::ReadOffsetUint8, luabind::pure_out_value(_2))
      .def("ReadOffsetUint16", &IOBuffer2::ReadOffsetUint16, luabind::pure_out_value(_2))
      .def("ReadOffsetUint32", &IOBuffer2::ReadOffsetUint32, luabind::pure_out_value(_2))
      .def("ReadOffsetUint64", &IOBuffer2::ReadOffsetUint64, luabind::pure_out_value(_2))
      .def("ReadOffsetString", &io_buffer2_read_offet_string)

      .def("WriteUint8", &IOBuffer2::WriteUint8)
      .def("WriteUint16", &IOBuffer2::WriteUint16)
      .def("WriteUint32", &IOBuffer2::WriteUint32)
      .def("WriteUint64", &IOBuffer2::WriteUint64)
      .def("WriteString", (void(IOBuffer2::*)(const std::string&))&IOBuffer2::WriteString)
      //.def("WriteString", (void(IOBuffer2::*)(const char*))&IOBuffer2::WriteString)

      .def("WriteOffsetUint8", &IOBuffer2::WriteOffsetUint8)
      .def("WriteOffsetUint16", &IOBuffer2::WriteOffsetUint16)
      .def("WriteOffsetUint32", &IOBuffer2::WriteOffsetUint32)
      .def("WriteOffsetUint64", &IOBuffer2::WriteOffsetUint64)
      .def("WriteOffsetString", (void(IOBuffer2::*)(const std::string&, int))&IOBuffer2::WriteOffsetString),
      //.def("WriteOffsetString", (void(IOBuffer2::*)(const char*, int))&IOBuffer2::WriteOffsetString),

    luabind::def("MD5String", &MD5String2),
    luabind::def("UTF8ToNativeMB", &UTF8ToNativeMB)
  ];
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

namespace zengine {

void Base_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(kBaseModuleName, &Luabind_Base_Register);
}

}
