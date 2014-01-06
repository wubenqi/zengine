-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------------------------
if ZCacheClientMainUtil == nil then
  ZCacheClientMainUtil = {
    class_name = "ZCacheClientMainUtil"
  }
end

function ZCacheClientMainUtil:MainInitialize()
  TRACE("ZCacheClientMainUtil:MainInitialize()")
  return 0
end

function ZCacheClientMainUtil:MainDestroy()
  --ctx:term()
  TRACE("ZCacheClientMainUtil:MainDestroy()")
  return 0
end

if g_main_util == nil or g_main_util.class_name == "MailUtil" then
  g_main_util = MainUtil:CreateInstance(ZCacheClientMainUtil)
else
  --PrintTable(g_main_util)
  g_main_util = ZCacheClientMainUtil
end



------------------------------------------------------------------------------------------------------------
if ZCacheClientThreadUtil == nil then
  ZCacheClientThreadUtil = {
    class_name = "ZCacheClientThreadUtil"
  }
end

function ZCacheClientThreadUtil:OnThreadInitialize(zcontext)
  TRACE("ZCacheClientThreadUtil:OnThreadInitialize()")  
  return 0
end

function ZCacheClientThreadUtil:OnThreadDestroy(zcontext)
  TRACE("ZCacheClientThreadUtil:OnThreadDestroy()")
  return 0
end

function ZCacheClientThreadUtil:OnTimer(zcontext, timer_id, tm)
  TRACE("ZCacheClientThreadUtil:OnTimer()")
  return 0
end

function ZCacheClientThreadUtil:OnTaskDataReceived(zcontext, task_data)
  --TRACE("ZCacheClientThreadUtil:OnTaskDataReceived()")
  
  -- [zcache_message_types.ACCOUNT_DATA_REQ]
--[[  
  local account_data_req = { account_name = "wubenqi" }
  local d = json.encode(account_data_req)
  LOG(d)
  g_znet_session_engine:SendSessionDataByTable(zcache_message_types.ACCOUNT_DATA_REQ, account_data_req)
--]]  
  
  local player_data_req = { player_aid = 16 }
  local d = json.encode(player_data_req)
  LOG(d)
  g_znet_session_engine:SendSessionDataByTable(zcache_message_types.PLAYER_DATA_REQ, player_data_req)

  --LOG(task_data)
  --print("client$ ")
  return 0
end

if g_thread_util == nil or g_thread_util.class_name == "ThreadUtil" then
  g_thread_util = ThreadUtil:CreateInstance(ZCacheClientThreadUtil)
else
  --PrintTable(g_main_util)
  g_thread_util = ZCacheClientThreadUtil
end
