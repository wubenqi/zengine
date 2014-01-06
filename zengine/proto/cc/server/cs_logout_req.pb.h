// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: cs_logout_req.proto

#ifndef PROTOBUF_cs_5flogout_5freq_2eproto__INCLUDED
#define PROTOBUF_cs_5flogout_5freq_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace hammer {
namespace message {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_cs_5flogout_5freq_2eproto();
void protobuf_AssignDesc_cs_5flogout_5freq_2eproto();
void protobuf_ShutdownFile_cs_5flogout_5freq_2eproto();

class CSLogoutReq;

// ===================================================================

class CSLogoutReq : public ::google::protobuf::Message {
 public:
  CSLogoutReq();
  virtual ~CSLogoutReq();
  
  CSLogoutReq(const CSLogoutReq& from);
  
  inline CSLogoutReq& operator=(const CSLogoutReq& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const CSLogoutReq& default_instance();
  
  void Swap(CSLogoutReq* other);
  
  // implements Message ----------------------------------------------
  
  CSLogoutReq* New() const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 player_id = 1;
  inline bool has_player_id() const;
  inline void clear_player_id();
  static const int kPlayerIdFieldNumber = 1;
  inline ::google::protobuf::uint32 player_id() const;
  inline void set_player_id(::google::protobuf::uint32 value);
  
  // required uint32 type = 2;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 2;
  inline ::google::protobuf::uint32 type() const;
  inline void set_type(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:hammer.message.CSLogoutReq)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::uint32 player_id_;
  ::google::protobuf::uint32 type_;
  friend void  protobuf_AddDesc_cs_5flogout_5freq_2eproto();
  friend void protobuf_AssignDesc_cs_5flogout_5freq_2eproto();
  friend void protobuf_ShutdownFile_cs_5flogout_5freq_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static CSLogoutReq* default_instance_;
};
// ===================================================================


// ===================================================================

// CSLogoutReq

// required uint32 player_id = 1;
inline bool CSLogoutReq::has_player_id() const {
  return _has_bit(0);
}
inline void CSLogoutReq::clear_player_id() {
  player_id_ = 0u;
  _clear_bit(0);
}
inline ::google::protobuf::uint32 CSLogoutReq::player_id() const {
  return player_id_;
}
inline void CSLogoutReq::set_player_id(::google::protobuf::uint32 value) {
  _set_bit(0);
  player_id_ = value;
}

// required uint32 type = 2;
inline bool CSLogoutReq::has_type() const {
  return _has_bit(1);
}
inline void CSLogoutReq::clear_type() {
  type_ = 0u;
  _clear_bit(1);
}
inline ::google::protobuf::uint32 CSLogoutReq::type() const {
  return type_;
}
inline void CSLogoutReq::set_type(::google::protobuf::uint32 value) {
  _set_bit(1);
  type_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace message
}  // namespace hammer

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_cs_5flogout_5freq_2eproto__INCLUDED
