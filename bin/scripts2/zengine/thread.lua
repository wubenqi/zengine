-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------
if ThreadUtil == nil then
  ThreadUtil = {}
end

function ThreadUtil:CreateInstance(o)
	o = o or {}
	setmetatable(o, {__index = self})
	return o
end

function ThreadUtil:OnThreadInitialize(zcontext)
  TRACE("ThreadUtil:OnThreadInitialize()")
  math.randomseed(os.time())
  
  return 0
end

function ThreadUtil:OnThreadDestroy(zcontext)
  TRACE("ThreadUtil:OnThreadDestroy()")
  return 0
end

function ThreadUtil:OnTimer(zcontext, timer_id, tm)
  TRACE("ThreadUtil:OnTimer()")
  return 0
end

function ThreadUtil:OnTaskDataReceived(zcontext, task_data)
  TRACE("ThreadUtil:OnTaskDataReceived()")
  return 0
end

------------------------------------------------------------------------------------------
if g_thread_util == nil then
  g_thread_util = ThreadUtil
end

------------------------------------------------------------------------------------------
-- 线程创建和销毁
function OnThreadInitialize(zcontext)
  -- 初始化随机数
  math.randomseed(os.time())
  return g_thread_util:OnThreadInitialize(zcontext)
end

function OnThreadDestroy(zcontext)
  return g_thread_util:OnThreadDestroy(zcontext)
end

------------------------------------------------------------------------------------------
-- 定时器
function OnTimer(zcontext, timer_id, tm)
  return g_thread_util:OnTimer(zcontext, timer_id, tm)
end

------------------------------------------------------------------------------------------
-- 异步消息
function OnTaskDataReceived(zcontext, task_data)
  return g_thread_util:OnTaskDataReceived(zcontext, task_data)
end

------------------------------------------------------------------------------------------

