// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef SCRIPTENGINE_SCRIPT_ENGINE_H_
#define SCRIPTENGINE_SCRIPT_ENGINE_H_
#pragma once

#include <luabind/luabind.hpp>
#include "base/logging.h"

#include <iostream>

class ScriptEngine {
public:
  ScriptEngine() : lua_state_(NULL) {
  }

  ~ScriptEngine();

  void Initialize();
  void Refresh();
  void Destroy();

  void Load(const std::string& lua_file);

  //void RegisterDefault();

  lua_State* GetLuaState() {
    return lua_state_;
  }

  int CallFunction(const char* function_name) {
    int ret = -1;
    try {
      ret = luabind::call_function<int>(lua_state_, function_name);
    } catch (luabind::error& err) {
      LOG(ERROR) << "Call function(" << function_name << ") failed. \"" << err.what() << ":" << lua_tostring(lua_state_, -1) << "\"" << std::endl;
    } catch (luabind::cast_failed& cf) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << cf.what() << "\"" << std::endl;
    } catch (std::runtime_error& re) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << re.what() << "\"" << std::endl;
    } catch (...) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\" invalid error\"." << std::endl;
    }
    return ret;
  }
  template <class A1>
  int CallFunction(const char* function_name, A1& a1) {
    int ret = -1;
    try {
      ret = luabind::call_function<int>(lua_state_, function_name, a1);
    } catch (luabind::error& err) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << err.what() << ":" << lua_tostring(lua_state_, -1) << "\"" << std::endl;
    } catch (luabind::cast_failed& cf) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << cf.what() << "\"" << std::endl;
    } catch (std::runtime_error& re) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << re.what() << "\"" << std::endl;
    } catch (...) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\" invalid error\"." << std::endl;
    }
    return ret;
  }

  template <class A1, class A2>
  int CallFunction(const char* function_name, A1& a1, A2& a2) {
    int ret = -1;
    try {
      ret = luabind::call_function<int>(lua_state_, function_name, a1, a2);
    } catch (luabind::error& err) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << err.what() << ":" << lua_tostring(lua_state_, -1) << "\"" << std::endl;
    } catch (luabind::cast_failed& cf) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << cf.what() << "\"" << std::endl;
    } catch (std::runtime_error& re) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << re.what() << "\"" << std::endl;
    } catch (...) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\" invalid error\"." << std::endl;
    }
    return ret;
  }

  template <class A1, class A2, class A3>
  int CallFunction(const char* function_name, A1& a1, A2& a2, A3& a3) {
    int ret = -1;
    try {
      ret = luabind::call_function<int>(lua_state_, function_name, a1, a2, a3);
    } catch (luabind::error& err) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << err.what() << ":" << lua_tostring(lua_state_, -1) << "\"" << std::endl;
    } catch (luabind::cast_failed& cf) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << cf.what() << "\"" << std::endl;
    } catch (std::runtime_error& re) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << re.what() << "\"" << std::endl;
    } catch (...) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\" invalid error\"." << std::endl;
    }
    return ret;
  }

  template <class A1, class A2, class A3, class A4>
  int CallFunction(const char* function_name, A1& a1, A2& a2, A3& a3, A4& a4) {
    int ret = -1;
    try {
      ret = luabind::call_function<int>(lua_state_, function_name, a1, a2, a3, a4);
    } catch (luabind::error& err) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << err.what() << ":" << lua_tostring(lua_state_, -1) << "\"" << std::endl;
    } catch (luabind::cast_failed& cf) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << cf.what() << "\"" << std::endl;
    } catch (std::runtime_error& re) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << re.what() << "\"" << std::endl;
    } catch (...) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\" invalid error\"." << std::endl;
    }
    return ret;
  }

  template <class A1, class A2, class A3, class A4, class A5>
  int CallFunction(const char* function_name, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) {
    int ret = -1;
    try {
      ret = luabind::call_function<int>(lua_state_, function_name, a1, a2, a3, a4, a5);
    } catch (luabind::error& err) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << err.what() << ":" << lua_tostring(lua_state_, -1) << "\"" << std::endl;
    } catch (luabind::cast_failed& cf) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << cf.what() << "\"" << std::endl;
    } catch (std::runtime_error& re) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\"" << re.what() << "\"" << std::endl;
    } catch (...) {
      LOG(ERROR) << "Call function(" << function_name << ") failed.\" invalid error\"." << std::endl;
    }
    return ret;
  }

  //void RegisterAll();

private:
  lua_State* lua_state_;
};


#endif
