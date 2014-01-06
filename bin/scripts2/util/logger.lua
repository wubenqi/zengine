-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--
-- 日志库
--

local debug   = _G.debug
local string  = _G.string
local os      = _G.os
local io      = _G.io

local function GetFileName(str)
  --local _, _, name = string.find(str,"(%w*%.lua)")
  local _, _, name = string.find(str,"([_%w]*%.lua)")
  return  name
end

local function GetCurrentFileLine(level)
  level = level or 0
  debug_info = debug.getinfo(level + 2)
  
  file_name    = GetFileName(debug_info.short_src)
  current_line = debug_info.currentline 
  return file_name, current_line
end

local function MakeLogFunction(log_type) 
  return function(content)
      local file_name, current_line = GetCurrentFileLine(1)
      content = string.format("%s %s  %s:%d] %s\n",log_type, os.date("%Y-%m-%d %H:%M:%S"), file_name, current_line, content)  
	     io.stderr:write(content)
      io.stderr:flush()
    end
end

TRACE = MakeLogFunction("TRACE")
INFO  = MakeLogFunction("INFO ")
LOG   = MakeLogFunction("LOG  ")
WARN  = MakeLogFunction("WARN ")
ERROR = MakeLogFunction("ERROR")
FATAL = MakeLogFunction("FATAL")

