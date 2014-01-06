// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zcommon/io_handler_util.h"

#include "base/logging.h"

#include "net/engine/io_handler.h"
#include "zcommon/message_util.h"

//////////////////////////////////////////////////////////////////////////
void SendMessageData(net::IOHandler* ih, uint16 message_type, const google::protobuf::Message* message) {
  DCHECK(ih);
  DCHECK(message);

  if (!ih || message==NULL) {
    LOG(ERROR) << "Send message[" << message_type <<"] data, znet_comm_handler or message is null.";
    return;
  }

  net::IOBufferPtr io_buffer = MakeIOBuffer(message_type, message);
  ih->AsyncSendIOBuffer(io_buffer);
}

void SendMessageData(net::IOHandler* ih, uint16 message_type, const google::protobuf::Message& message) {
  SendMessageData(ih, message_type, &message);
}

void SendNullMessageData(net::IOHandler* ih, uint16 message_type) {
  DCHECK(ih);
  if (!ih) {
    LOG(ERROR) << "Send message[" << message_type <<"] data, znet_comm_handler is null.";
    return;
  }

  net::IOBufferPtr io_buffer = MakeIOBuffer(message_type);
  return ih->AsyncSendIOBuffer(io_buffer);
}

//////////////////////////////////////////////////////////////////////////
void SendSessionMessageData(net::IOHandler* ih, uint64 session_id, uint16 message_type, const google::protobuf::Message* message) {
  DCHECK(ih);
  DCHECK(message);

  if (!ih || message==NULL) {
    LOG(ERROR) << "Send message[" << message_type <<"] data, znet_comm_handler or message is null.";
    return;
  }

  net::IOBufferPtr io_buffer = MakeIOBuffer(session_id, message_type, message);
  ih->AsyncSendIOBuffer(io_buffer);
}

void SendSessionMessageData(net::IOHandler* ih, uint64 session_id, uint16 message_type, const google::protobuf::Message& message) {
  SendSessionMessageData(ih, session_id, message_type, &message);
}

void SendSessionNullMessageData(net::IOHandler* ih, uint64 session_id, uint16 message_type) {
  DCHECK(ih);
  if (!ih) {
    LOG(ERROR) << "Send message[" << message_type <<"] data, znet_comm_handler is null.";
    return;
  }

  net::IOBufferPtr io_buffer = MakeIOBuffer(session_id, message_type);
  return ih->AsyncSendIOBuffer(io_buffer);
}

void SendIOBufferData(net::IOHandler* ih, uint16 message_type, const IOBuffer2* io_buffer2) {
  DCHECK(ih);
  if (!ih) {
    LOG(ERROR) << "Send message[" << message_type <<"] data, znet_comm_handler is null.";
    return;
  }
  net::IOBufferPtr io_buffer = MakeIOBuffer(message_type, io_buffer2);
  return ih->AsyncSendIOBuffer(io_buffer);
}

void SendSessionIOBufferData(net::IOHandler* ih, uint64 session_id, uint16 message_type, const IOBuffer2* io_buffer2) {
  DCHECK(ih);
  if (!ih) {
    LOG(ERROR) << "Send message[" << message_type <<"] data, znet_comm_handler is null.";
    return;
  }
  net::IOBufferPtr io_buffer = MakeIOBuffer(session_id, message_type, io_buffer2);
  return ih->AsyncSendIOBuffer(io_buffer);
}

void SendSessionStringData(net::IOHandler* ih, uint64 session_id, uint16 message_type, const std::string& data) {
  DCHECK(ih);
  if (!ih) {
    LOG(ERROR) << "Send message[" << message_type <<"] data, znet_comm_handler is null.";
    return;
  }
  net::IOBufferPtr io_buffer = MakeIOBuffer(session_id, message_type, data);
  return ih->AsyncSendIOBuffer(io_buffer);
}
