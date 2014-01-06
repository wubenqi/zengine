-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------

--[[
-- 定义ZGateSessionHandler
if ZLoginSessionHandler == nil then
  ZLoginSessionHandler = {
    ih = nil,
    session_mgr = {}
  }
end

-- 定义ZCacheSessionHandler
if ZServerSessionHandler == nil then
  ZServerSessionHandler = {
    ih = nil,
    session_mgr = {}
  }
end
--]]

-- 定义ZClientSessionEngine
if ZClientSessionEngine == nil then
  ZClientSessionEngine = {
   class_name = "ZClientSessionEngine",
    ih = nil,
    session_id = nil,
    --session_mgr = {}
    --["zlogin"] = ZLoginSessionHandler,
    --["zserver"] = ZServerSessionHandler
  }
end

------------------------------------------------------------------------------------------

function ZClientSessionEngine:CreateInstance(o)
	o = o or { class_name = "ZClientSessionEngine" }
	setmetatable(o, {__index = self})
	return o
end

if g_znet_session_engine == nil or g_znet_session_engine.class_name == "ZNetSessionEngine" then
  g_znet_session_engine = ZNetSessionEngine:CreateInstance(ZClientSessionEngine)
else
  PrintTable(g_znet_hangler_engine)
  g_znet_session_engine = ZNetSessionEngine
end

------------------------------------------------------------------------------------------
function ZClientSessionEngine:Initialize(ih)
  TRACE("ZClientSessionEngine:Initialize()")
  
--[[  
  local session = self[ih:GetSessionID()]
  if session == nil then
    session = { ih = ih, session_id = session_id, addr = ih:GetAddressString() }
    --self.session_mgr[session_id] = session
  end
  
  session.ih = ih
  self[ih:GetSessionID()] = session
--]]
  self.ih = ih
  self.session_id = ih:GetSessionID()

  --session:Initialize(ih)
end

function ZClientSessionEngine:Destroy(ih)
  TRACE("ZClientSessionEngine:Destroy()")

--[[  
  local session = self[ih:GetSessionID()]
  if session ~= nil then
    session:Destroy(ih)
    session.ih = nil
    self[ih:GetSessionID()] = nil
  else
    LOG("ZClientSessionEngine:Destroy() -- not find session_engine by instance_name " .. ih:GetReadableSessionID())
  end
--]]
  self.ih = nil  
  self.session_id = nil
end

function ZClientSessionEngine:OnSessionDataHandler(ih, message_type, io_buffer)
  TRACE("ZClientSessionEngine:OnSessionDataHandler()")
  local r,session_id = io_buffer:ReadUint64()
  
  local json_data = io_buffer:ReadString()
  LOG(json_data)
  
--[[  
  local message = json.decode(json_data)
  if self[message_type] ~= nil then
    return self[message_type](self, ih, session_id, message)
  else
    ERROR("recv a invalid message_type: " .. message_type)
  end
--]]

--[[  
  LOG("data_len = " .. #data .. ", data = " .. data)
  local d3 = '{"account_name":"wubenqi","player_aid":1}'
  --local zproto_cs_account_req = json:decode(d3)
  
  local bb = { account_name = "wubenqi", player_aid = 1 }
  --local ee = { account_name = 'wubenqi', player_aid = 1 }  
  
  local cc = json.encode(bb)
  LOG("cc = " .. #cc .. ": " .. cc)
  local t_cc = json.decode(cc)
  PrintTable(t_cc)
  
  zproto_cs_account_req = json.decode(data)
  PrintTable(zproto_cs_account_req)

  local eee = { acctount_name = "222", passwd = "1222", player_aid = 1 }
  --zproto_sc_account_ack.acctount_name = "wubenqi"
  --zproto_sc_account_ack.passwd = "123456"
  --zproto_sc_account_ack.player_aid = 1
  
  local data2 = json.encode(eee)
  LOG("data2 = " .. data2)
  
  ih:SendSessionStringData(session_id, message_type, data2)


  --SendSessionDataByTable(ih, session_id, message_type, zproto_sc_account_ack)
--]]
  
end

function ZClientSessionEngine:SendSessionDataByTable(message_type, json_table)
  if self.ih ~= nil and self.session_id ~= nil then
    local data = json.encode(json_table)
    self.ih:SendSessionStringData(self.session_id, message_type, data)
  end
end

--[[
function ZClientSessionEngine:SendToZLoginSessionData(session_id, message_type, io_buffer)
  local ih = self["zgate"].ih
  if ih ~= nil then
    ih:SendSessionIOBufferData(session_id, message_type, io_buffer)
  end
end

function ZClientSessionEngine:SendToZServerSessionData(session_id, message_type, io_buffer)
  local ih = self["zcache"].ih
  if ih ~= nil then
    ih:SendSessionIOBufferData(session_id, message_type, io_buffer)
  end
end
--]]

ZClientSessionEngine[zcache_message_types.SERVER_DATA_REQ] = function(self, ih, session_id, message)
  
  return 0
end

ZClientSessionEngine[zcache_message_types.ACCOUNT_DATA_REQ] = function(self, ih, session_id, message)
  TRACE("ZClientSessionEngine:ACCOUNT_DATA_REQ")

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

ZClientSessionEngine[zcache_message_types.PLAYER_INFO_REQ] = function(self, ih, session_id, message)
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

ZClientSessionEngine[zcache_message_types.PLAYER_DATA_REQ] = function(self, ih, session_id, message)
  TRACE("ZClientSessionEngine:PLAYER_DATA_REQ")

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

ZClientSessionEngine[zcache_message_types.PLAYER_ATTRS_DATA_REQ] = function(self, ih, session_id, message)
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
