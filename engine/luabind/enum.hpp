// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

// Copyright 2010, www.lelewan.com
// All rights reserved
//
// Author: wubenqi<wubenqi@caitong.net>, 2012-08-02
// 对于枚举类型，LUA中以枚举类型作为table名，字段名作为键，枚举值作为值
// 例如
// enum EnumType {
//  kEnumType_1 = 1,
//  kEnumType_2 = 2,
// };
// lua中为
// EnumType = {
//  kEnumType_1 = 1,
//  kEnumType_2 = 2
// }

#ifndef LUABIND_ENUM_HPP_INCLUDED
#define LUABIND_ENUM_HPP_INCLUDED

# include <luabind/scope.hpp>
# include <luabind/detail/call_function.hpp>
# include <luabind/detail/enum_maker.hpp>

namespace luabind {

namespace detail
{
  inline void setfield(lua_State* L, const char* index, int value)
  {
		lua_pushnumber(L, value);
		//lua_setfield(L, -2, index);
		lua_setglobal(L, index);
  }

  struct enum_registration : registration
  {
      enum_registration(char const* name)
        : m_name(name)
      {}

      void register_(lua_State* L) const
      {
		  // 创建表
		  //lua_newtable(L);

		  // 设置字段
		  std::map<const char*, int, ltstr>::const_iterator i;
		  for (i = m_static_constants.begin(); i != m_static_constants.end(); ++i)
		  {
			  setfield(L, i->first, i->second);
		  }

		  // 设置表名
		  //lua_setglobal(L, m_name);
	  }

	  void add_static_constant(const char* name, int val)
	  {
		  m_static_constants[name] = val;
	  }

      char const* m_name;
	  mutable std::map<const char*, int, detail::ltstr> m_static_constants;
  };

} // namespace detail

struct enum_class : public scope
{
	enum_class(char const* name)
		: scope(std::auto_ptr<detail::registration>(m_registration = new detail::enum_registration(name)))
	{
	}

	void add_static_constant(const char* name, int val) {
		m_registration->add_static_constant(name, val);
	}

	detail::enum_maker<enum_class> def()
	{
		return detail::enum_maker<enum_class>(*this);
	}

private:
	detail::enum_registration* m_registration;
};

inline enum_class enum_(char const* name) {
	return enum_class(name);
}

} // namespace luabind

#endif // LUABIND_ENUM_HPP_INCLUDED
