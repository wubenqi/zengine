// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "const_message_types.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace hammer {
namespace message {

namespace {

const ::google::protobuf::EnumDescriptor* MessageTypes_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_const_5fmessage_5ftypes_2eproto() {
  protobuf_AddDesc_const_5fmessage_5ftypes_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "const_message_types.proto");
  GOOGLE_CHECK(file != NULL);
  MessageTypes_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_const_5fmessage_5ftypes_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_const_5fmessage_5ftypes_2eproto() {
}

void protobuf_AddDesc_const_5fmessage_5ftypes_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\031const_message_types.proto\022\016hammer.mess"
    "age*\304\003\n\014MessageTypes\022\020\n\014MESSAGE_NONE\020\000\022\035"
    "\n\031MESSAGE_CS_AUTH_CHALLENGE\020\001\022\035\n\031MESSAGE"
    "_SC_AUTH_CHALLENGE\020\002\022\031\n\025MESSAGE_CS_AUTH_"
    "PROOF\020\003\022\031\n\025MESSAGE_SC_AUTH_PROOF\020\004\022\'\n#ME"
    "SSAGE_CS_SERVER_CHALLENGE_REQUEST\020\005\022#\n\037M"
    "ESSAGE_SC_SERVER_CHALLENGE_ACK\020\006\022!\n\035MESS"
    "AGE_CS_SERVER_SESSION_NEW\020\007\022!\n\035MESSAGE_S"
    "C_SERVER_SESSION_NEW\020\010\022#\n\037MESSAGE_CS_SER"
    "VER_SESSION_CLOSE\020\t\022#\n\037MESSAGE_SC_SERVER"
    "_SESSION_CLOSE\020\n\022\031\n\025MESSAGE_CS_LOGOUT_RE"
    "Q\0204\022\031\n\025MESSAGE_SC_RET_LOGOUT\0205\022\032\n\026MESSAG"
    "E_RET_REQ_RESULT\020dB\022\n\016hammer.messageH\002", 518);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "const_message_types.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_const_5fmessage_5ftypes_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_const_5fmessage_5ftypes_2eproto {
  StaticDescriptorInitializer_const_5fmessage_5ftypes_2eproto() {
    protobuf_AddDesc_const_5fmessage_5ftypes_2eproto();
  }
} static_descriptor_initializer_const_5fmessage_5ftypes_2eproto_;

const ::google::protobuf::EnumDescriptor* MessageTypes_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MessageTypes_descriptor_;
}
bool MessageTypes_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 52:
    case 53:
    case 100:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace message
}  // namespace hammer

// @@protoc_insertion_point(global_scope)
