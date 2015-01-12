// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_NET_ZNET_HANDLER_CONTEXT_H_
#define MOD_NET_ZNET_HANDLER_CONTEXT_H_

#include <string>

class ZEngineContext;
namespace zengine {

struct ZNetHandlerContext {
  ZNetHandlerContext(ZEngineContext* _context, const std::string& _net_type, const std::string& _instnce_name)
    : context(_context),
      net_type(_net_type),
      instnce_name(_instnce_name) {}

  ZEngineContext* context;
  std::string net_type;
  std::string instnce_name;
};

}

#endif

