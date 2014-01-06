-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--


------------------------------------------------------------------------------------------
if MainUtil == nil then
  MainUtil = { class_name = "MainUtil" }
end

function MainUtil:CreateInstance(o)
	o = o or { class_name = "MainUtil" }
	setmetatable(o, {__index = self})
	return o
end

function MainUtil:MainInitialize()
  TRACE("MainUtil:MainInitialize()")
  return 0
end

function MainUtil:MainDestroy()
  TRACE("MainUtil:MainDestroy()")
  return 0
end

if g_main_util == nil then
  g_main_util = MainUtil
end

------------------------------------------------------------------------------------------
-- 主程序初始化
function MainInitialize()
  return g_main_util:MainInitialize()
end

-- 主程序结束
function MainDestroy()
  return g_main_util:MainDestroy()
end

