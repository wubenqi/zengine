// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_SCRIPT_SCRIPT_MANAGER_H_
#define ZENGINE_SCRIPT_SCRIPT_MANAGER_H_
#pragma once

#include "scriptengine/script_engine.h"

//class ZNetCommHandler;
class ScriptFileManager;
class ZEngineContext;
class ScriptManager {
public:
  ScriptManager();
  ~ScriptManager();

  bool Initialize();
  void Destroy();

  ScriptEngine& script_engine() {
    return script_engine_;
  }

  //const std::string& GetAppName() const {
  //  return app_name_;
  //}

  void DoWatchSignal();

  //////////////////////////////////////////////////////////////////////////
  int ExeScript_MainInitialize();
  int ExeScript_MainDestroy();

  int ExeScript_OnThreadInitialize(ZEngineContext* context);
  int ExeScript_OnThreadDestroy(ZEngineContext* context);

  //int ExeScript_OnNewConnection(ZNetCommHandler* ih);
  //int ExeScript_OnDataReceived(ZNetCommHandler* ih, uint16 message_type, const char* data, uint32 data_len);
  //int ExeScript_OnConnectionClosed(ZNetCommHandler* ih);

  int ExeScript_OnTaskDataReceived(ZEngineContext* context, const std::string& task_data);
  int ExeScript_OnTimer(ZEngineContext* context, uint32 timer_id, int tm);

private:
  void LoadScripts();
  void RegisterAll();

  void DoRefresh();

private:
  ScriptEngine script_engine_;
  std::vector<std::string> data_scripts_; 
  //std::string app_name_;
  ScriptFileManager* script_file_manager_;
};


#if 0
#include "base/singleton.h"

#include "base/dyn_lib.h"
#include "scriptengine/script_engine.h"

#include "tgame/player.h"

namespace boost { namespace asio {
  class io_service;
}}

class Room;
class Player;
class IOHandler;
class ScriptFileManager;
class ScriptManager : public Singleton<ScriptManager> {
public:
  bool Initialize(boost::asio::io_service* io_service, const std::string& app_name);
  void LoadScripts();

  // bool Resfesh();
  void Destroy();
  const std::string& GetAppName() const {
    return app_name_;
  }

  void RegisterAll();

  int ExeScript_MainInitialize();
  int ExeScript_MainDestroy();

  int ExeScript_OnRoomCreate(Room* room);
  int ExeScript_OnRoomDestroy(Room* room);
  int ExeScript_OnRoomTimer(Room* room, uint32 timer_id, int tm);
 
  int ExeScript_OnRoomManagerInitialize(IOHandler* ih);
  int ExeScript_OnRoomManagerDestroy(IOHandler* ih);
  int ExeScript_OnRoomStateNotify(IOHandler* ih, uint32 room_id, int room_state);

  int ExeScript_OnNewSession(Session* session);
  int ExeScript_OnSessionDataReceived(Session* session, uint16 message_type, const void* data, uint32 data_len);
  int ExeScript_OnSessionClosed(Session* session);

  int ExeScript_OnSessionLogin(Session* session);
  int ExeScript_OnSessionLogout(Session* session);

  int ExeScript_OnSessionTimer(Session* session, uint32 timer_id, int tm);

  int ExeScript_OnNewConnection(IOHandler* ih);
  int ExeScript_OnTCPDataReceived(IOHandler* ih, int client, uint16 message_type, const char* data, uint32 data_len);
  int ExeScript_OnConnectionClosed(IOHandler* ih);

  void DoWatchSignal();

private:
  void DoRefresh();

  ScriptEngine script_engine_;
  std::vector<std::string> data_scripts_; 
  std::string app_name_;

  int refresh_type_;

  base::DynLib* dyn_lib_;

  friend class Singleton<ScriptManager>;

  ScriptManager();
  virtual ~ScriptManager();

  ScriptFileManager* script_file_manager_;
};

void SetScriptRefreshType(int refresh_type);
#endif


#endif

