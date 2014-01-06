// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_ZENGINE_TYPES_H_
#define ZENGINE_ZENGINE_TYPES_H_
#pragma once

#include "base/basictypes.h"

#include <vector>
#include <string>

namespace zengine {

struct CStringVector {
  size_t size() const {
    return cstring_vector.size();
  }
  std::vector<const char*> cstring_vector;
};

struct StringVector {
  size_t size() const {
    return string_vector.size();
  }
  std::vector<std::string> string_vector;
};

}

#endif
