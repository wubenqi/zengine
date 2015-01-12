// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/script/script_manager.h"

#include "base2/io_buffer2.h"

//#include "zengine/script/luabind_register_helper.h"
#include "scriptengine/luabind_register_manager.h"

#include "zengine/script/script_file_manager.h"
//#include "zengine/client_comm_handler.h"

#include "zengine/zengine_context.h"
//#include "zengine/zengine_daemon.h"

ScriptManager::ScriptManager()
: script_file_manager_(NULL) {
}

ScriptManager::~ScriptManager() {
  Destroy();
}

bool ScriptManager::Initialize() {
  //app_name_ = app_name;
  script_engine_.Initialize();
  RegisterAll();

  script_file_manager_ = ScriptFileManager::GetInstance();
  script_file_manager_->RegisterScriptManager(this);

  LoadScripts();
  return true;
}

void ScriptManager::LoadScripts() {
  base::linked_map<base::FilePath, ScriptFileManager::ScriptFileData>::iterator it;
  base::linked_map<base::FilePath, ScriptFileManager::ScriptFileData>& script_datas = script_file_manager_->GetScriptDatas();
  for (it=script_datas.begin(); it!=script_datas.end(); ++it) {
    if (it->second.IsModified()) {
      script_engine_.DoFile(it->first.MaybeAsASCII());
    }
  }
}

void ScriptManager::RegisterAll() {
  LuabindRegisterManager::GetInstance()->RestiterAll(script_engine_.GetLuaState());
  //script_engine_.RegisterDefault();
  //Luabind_Register_Helper(script_engine_.GetLuaState());
}

void ScriptManager::Destroy() {
  script_engine_.Destroy();
  script_file_manager_ = NULL;
}

//////////////////////////////////////////////////////////////////////////
int ScriptManager::ExeScript_MainInitialize() {
  int result = script_engine_.CallFunction<int>("MainInitialize");
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute MainInitialize() error, error_code = " << result;
  }
  return 0;
}

int ScriptManager::ExeScript_MainDestroy() {
  int result = script_engine_.CallFunction<int>("MainDestroy");
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute MainDestroy() error, error_code = " << result;
  }
  return 0;
}

int ScriptManager::ExeScript_OnThreadInitialize(ZEngineContext* context) {
  int result = script_engine_.CallFunction<int, ZEngineContext*>("OnThreadInitialize", context);
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnThreadInitialize() error, error_code = " << result;
  }
  return 0;
}

int ScriptManager::ExeScript_OnThreadDestroy(ZEngineContext* context) {
  int result = script_engine_.CallFunction<int, ZEngineContext*>("OnThreadDestroy", context);
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnThreadDestroy() error, error_code = " << result;
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////
//int ScriptManager::ExeScript_OnNewConnection(ZNetCommHandler* ih) {
//  int result = script_engine_.CallFunction("OnNewConnection", ih);
//  if (result!=0) {
//    LOG(ERROR) << "ERROR: In main.lua, Execute OnNewConnection() error, error_code = " << result;
//  }
//  return 0;
//}
//
//int ScriptManager::ExeScript_OnDataReceived(ZNetCommHandler* ih, uint16 message_type, const char* data, uint32 data_len) {
//  IOBuffer2 io_buffer(data, data_len);
//  int result = script_engine_.CallFunction("OnOnDataReceived", ih, message_type, io_buffer);
//  if (result!=0) {
//    LOG(ERROR) << "ERROR: In main.lua, Execute OnDataReceived() error, error_code = " << result;
//  }
//  return 0;
//}
//
//int ScriptManager::ExeScript_OnConnectionClosed(ZNetCommHandler* ih) {
//  int result = script_engine_.CallFunction("OnConnectionClosed", ih);
//  if (result!=0) {
//    LOG(ERROR) << "ERROR: In main.lua, Execute OnConnectionClosed() error, error_code = " << result;
//  }
//  return 0;
//}

int ScriptManager::ExeScript_OnTaskDataReceived(ZEngineContext* context, const std::string& task_data) {
  int result = script_engine_.CallFunction<int, ZEngineContext*, std::string>("OnTaskDataReceived", context, task_data);
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnTaskDataReceived() error, error_code = " << result;
  }
  return 0;
}

int ScriptManager::ExeScript_OnTimer(ZEngineContext* context, uint32 timer_id, int tm) {
  int result = script_engine_.CallFunction<int, ZEngineContext*, uint32, int>("OnTimer", context, timer_id, tm);
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnTimer() error, error_code = " << result;
  }
  return 0;
}
