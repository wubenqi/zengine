// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZCOMMON_IO_HANDLER_UTIL_H_
#define ZCOMMON_IO_HANDLER_UTIL_H_
#pragma once

#include <string>
#include "base/basictypes.h"

namespace google { namespace protobuf {
class Message;
}}

namespace net {
class IOHandler;
}

class IOBuffer2;

void SendMessageData(net::IOHandler* ih, uint16 message_type, const google::protobuf::Message* message);
void SendMessageData(net::IOHandler* ih, uint16 message_type, const google::protobuf::Message& message);
void SendNullMessageData(net::IOHandler* ih, uint16 message_type);

void SendSessionMessageData(net::IOHandler* ih, uint64 session_id, uint16 message_type, const google::protobuf::Message* message);
void SendSessionMessageData(net::IOHandler* ih, uint64 session_id, uint16 message_type, const google::protobuf::Message& message);
void SendSessionNullMessageData(net::IOHandler* ih, uint64 session_id, uint16 message_type);

void SendIOBufferData(net::IOHandler* ih, uint16 message_type, const IOBuffer2* io_buffer2);
void SendSessionIOBufferData(net::IOHandler* ih, uint64 session_id, uint16 message_type, const IOBuffer2* io_buffer2);

void SendSessionStringData(net::IOHandler* ih, uint64 session_id, uint16 message_type, const std::string& data);

#endif
