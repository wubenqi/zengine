// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZCOMMON_MESSAGE_UTIL_H_
#define ZCOMMON_MESSAGE_UTIL_H_
#pragma once

#include "base/basictypes.h"
#include "net/base/io_buffer.h"

namespace google { namespace protobuf {
class Message;
}}
class IOBuffer2;

net::IOBufferPtr MakeIOBuffer(uint16 message_type, const google::protobuf::Message* message);
net::IOBufferPtr MakeIOBuffer(uint16 message_type, const google::protobuf::Message& message);
net::IOBufferPtr MakeIOBuffer(uint16 message_type);
net::IOBufferPtr MakeIOBuffer(uint16 message_type, const char* data, uint32 data_le);

net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const google::protobuf::Message* message);
net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const google::protobuf::Message& message);
net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type);

net::IOBufferPtr MakeIOBuffer(uint16 message_type, const IOBuffer2* io_buffer2);
net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const IOBuffer2* io_buffer2);

net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const std::string& data);
net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const char* data, uint32 data_len);

#endif
