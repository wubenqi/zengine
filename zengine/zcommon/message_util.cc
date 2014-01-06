// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zcommon/message_util.h"

#include "base/logging.h"
#include "base2/io_buffer2.h"

#include <google/protobuf/message.h>

#include "net/base/io_buffer.h"
#include "net/hammer/hammer_packet_header.h"

//static IOBufferPtr kEmptyIOBufferPtr;

net::IOBufferPtr MakeIOBuffer(uint16 message_type, const google::protobuf::Message* message) {
  net::IOBufferPtr io_buffer;// = kEmptyIOBufferPtr;
  if (message!=NULL) {
    uint32 message_size = message->ByteSize();
    io_buffer = new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize + message_size);
    if(!message->SerializeToArray(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize, message_size)) {
      LOG(ERROR) << "MessageType[" 
        << message_type 
        << "] SerializeToArray error.";
      io_buffer = NULL;
    } else {
      net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), message_type, message_size);
    }
  }
  return io_buffer;
}

net::IOBufferPtr MakeIOBuffer(uint16 message_type, const google::protobuf::Message& message) {
  return MakeIOBuffer(message_type, &message);
}

net::IOBufferPtr MakeIOBuffer(uint16 message_type) {
  net::IOBufferPtr io_buffer(new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize));
  net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), message_type, 0);
  return io_buffer;
}

net::IOBufferPtr MakeIOBuffer(uint16 message_type, const char* data, uint32 data_len) {
  net::IOBufferPtr io_buffer(new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize + data_len));
  net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), message_type, data_len);
  memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize, data, data_len);
  return io_buffer;
}

net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const google::protobuf::Message* message) {
  net::IOBufferPtr io_buffer;// = kEmptyIOBufferPtr;
  if (message!=NULL) {
    uint32 message_size = message->ByteSize();
    io_buffer = new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize + message_size+sizeof(uint64));
    if(!message->SerializeToArray(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize+sizeof(uint64), message_size)) {
      LOG(ERROR) << "MessageType[" 
        << message_type 
        << "] SerializeToArray error.";
      io_buffer = NULL;
    } else {
      net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), message_type, message_size+sizeof(uint64));
      memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize, &session_id, sizeof(uint64));
    }
  }
  return io_buffer;
}

net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const google::protobuf::Message& message) {
  return MakeIOBuffer(session_id, message_type, &message);
}

net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type) {
  net::IOBufferPtr io_buffer(new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize)+sizeof(uint64));
  net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), message_type, sizeof(uint64));
  memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize, &session_id, sizeof(uint64));
  return io_buffer;
}

net::IOBufferPtr MakeIOBuffer(uint16 message_type, const IOBuffer2* io_buffer2) {
  net::IOBufferPtr io_buffer;// = kEmptyIOBufferPtr;
  if (io_buffer2!=NULL) {
    io_buffer = new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize+io_buffer2->writable_size());
    net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), message_type, io_buffer2->writable_size());
    memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize, io_buffer2->data(), io_buffer2->writable_size());
  }
  return io_buffer;
}

net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const IOBuffer2* io_buffer2) {
  net::IOBufferPtr io_buffer;// = kEmptyIOBufferPtr;
  if (io_buffer2!=NULL) {
    io_buffer = new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize + io_buffer2->writable_size()+sizeof(uint64));
    memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize+sizeof(session_id), io_buffer2->data(), io_buffer2->writable_size());
    net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), message_type, io_buffer2->writable_size()+sizeof(uint64));
    memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize, &session_id, sizeof(uint64));
  }
  return io_buffer;
}

net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const std::string& data) {
  net::IOBufferPtr io_buffer;// = kEmptyIOBufferPtr;
  uint32 body_len = data.length()+1+sizeof(uint64)+sizeof(uint32);
  io_buffer = new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize + body_len);
  net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), message_type, body_len);

  IOBuffer2 io_buffer2(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize, body_len);
  io_buffer2.WriteUint64(session_id);
  io_buffer2.WriteString(data);
  return io_buffer;
}

net::IOBufferPtr MakeIOBuffer(uint64 session_id, uint16 message_type, const char* data, uint32 data_len) {
  net::IOBufferPtr io_buffer;// = kEmptyIOBufferPtr;
  io_buffer = new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize + data_len+sizeof(uint64));
  memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize+sizeof(session_id), data, data_len);
  net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), message_type, data_len+sizeof(uint64));
  memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize, &session_id, sizeof(uint64));
  return io_buffer;
}
