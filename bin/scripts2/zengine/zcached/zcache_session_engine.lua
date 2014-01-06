-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------

-- ∂®“ÂZCacheSessionEngine
if ZCacheSessionEngine == nil then
  ZCacheSessionEngine = {
   class_name = "ZCacheSessionEngine",
   --["zlogin"] = ZLoginSessionHandler,
   --["zserver"] = ZServerSessionHandler
  }
end

------------------------------------------------------------------------------------------

function ZCacheSessionEngine:CreateInstance(o)
	o = o or { class_name = "ZCacheSessionEngine" }
	setmetatable(o, {__index = self})
	return o
end

if g_znet_session_engine == nil or g_znet_session_engine.class_name == "ZNetSessionEngine" then
  g_znet_session_engine = ZNetSessionEngine:CreateInstance(ZCacheSessionEngine)
else
  PrintTable(g_znet_hangler_engine)
  g_znet_session_engine = ZNetSessionEngine
end

------------------------------------------------------------------------------------------
function ZCacheSessionEngine:Initialize(ih)
  TRACE("ZCacheSessionEngine:Initialize()")
  
end

function ZCacheSessionEngine:Destroy(ih)
  TRACE("ZCacheSessionEngine:Destroy()")
end

function ZCacheSessionEngine:OnSessionDataHandler(ih, message_type, io_buffer)
  TRACE("ZCacheSessionEngine:OnSessionDataHandler()")
  local r,session_id = io_buffer:ReadUint64()
  
  local json_data = io_buffer:ReadString()
  LOG(json_data)
  
  local message = json.decode(json_data)
  PrintTable(message)
  if self[message_type] ~= nil then
    return self[message_type](self, ih, session_id, message)
  else
    ERROR("recv a invalid message_type: " .. message_type)
  end
end

ZCacheSessionEngine[zcache_message_types.SERVER_DATA_REQ] = function(self, ih, session_id, message)
  
  return 0
end

ZCacheSessionEngine[zcache_message_types.ACCOUNT_DATA_REQ] = function(self, ih, session_id, message)
  TRACE("ZCacheSessionEngine:OnAccountDataReqHandler()")
  local account_datas = g_zcache_data_mgr.g_server_data.account_datas
  
  local rep_data = {
    retult = 1,
    message = nil
  }
  
  --LOG(message.acctount_name)
  
  local account_data = account_datas[message.account_name]
  --PrintTable(account_datas)
  --PrintTable(account_data)
  
  if account_data ~= nil then
    rep_data.retult = 0
    rep_data.message = account_data
  else
    LOG("Notfound ")
  end

  LOG(json.encode(rep_data))
  SendSessionDataByTable(ih, session_id, zcache_message_types.ACCOUNT_DATA_REP, rep_data)
  return 0
end

ZCacheSessionEngine[zcache_message_types.PLAYER_INFO_REQ] = function(self, ih, session_id, message)
  local account_datas = g_zcache_data_mgr.g_server_data.account_datas
  
  local rep_data = {
    retult = 1,
    message = nil
  }
  
  if account_datas[message.acctount_name] ~= nil then
    rep_data.retult = 0
    rep_data.message = account_datas[message.acctount_name]
    SendSessionDataByTable(ih, session_id, zcache_message_types.ACCOUNT_DATA_REP, rep_data)
  end

  return 0
end

ZCacheSessionEngine[zcache_message_types.PLAYER_DATA_REQ] = function(self, ih, session_id, message)
  local player_data_req = message
  local cache_player_datas = g_zcache_data_mgr.z_player_data_mgr
  
  local rep_data = {
    retult = 1,
    message = nil
  }
  
  if cache_player_datas[player_data_req.player_aid] ~= nil then
    rep_data.retult = 0
    rep_data.message = cache_player_datas[player_data_req.player_aid]
  else
    local cache_data = ZLoadPlayerGameData(player_data_req.player_aid)
    if cache_data ~= nil then
      rep_data.retult = 0
      rep_data.message = cache_data
      cache_player_datas[player_data_req.player_aid] = cache_data
    end
  end

  PrintTable(rep_data)
  SendSessionDataByTable(ih, session_id, zcache_message_types.PLAYER_DATA_REP, rep_data)

  return 0
end

ZCacheSessionEngine[zcache_message_types.PLAYER_ATTRS_DATA_REQ] = function(self, ih, session_id, message)

  return 0
end
