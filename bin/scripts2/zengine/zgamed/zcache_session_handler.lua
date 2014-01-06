-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------
-- ÎÞ×´Ì¬

--local g_zcache_protos = g_zcache_protos

function ZCacheSessionHandler:Initialize(ih)
  TRACE("ZNetSessionEngine:Initialize()")
  
--[[  
  local zproto_cs_account_req = { account_name = "wubenqi", player_aid = 1 }
  
   -- g_zcache_protos["zproto_cs_account_req"]
  --print(zproto_cs_account_req)
  --zproto_cs_account_req.acctount_name = "wubenqi"
  PrintTable(zproto_cs_account_req)
  local data = json.encode(zproto_cs_account_req)
  LOG(data)
  bbb = json.decode(data)
  PrintTable(bbb)
  
  --LOG(#bb)
  --local data = json.encode(zproto_cs_account_req)
  --SendSessionDataByTable(ih, ih:GetSessionID(), MESSAGE_CS_AUTH_CHALLENGE, zproto_cs_account_req)
  ih:SendSessionStringData(ih:GetSessionID(), MESSAGE_CS_AUTH_CHALLENGE, data)
  --ih:SendSessionStringData(ih:GetSessionID(), MESSAGE_CS_AUTH_CHALLENGE, bb)
--]]

end

function ZCacheSessionHandler:Destroy(ih)
  TRACE("ZNetSessionEngine:Destroy()")
end

function ZCacheSessionHandler:OnSessionDataHandler(ih, message_type, io_buffer)
  LOG("ZCacheSessionHandler:OnSessionDataReceived: recv message_type " .. message_type)
  local r,session_id = io_buffer:ReadUint64()
  
  local json_data = io_buffer:ReadString()
  LOG(json_data)
  
  local message = json.decode(json_data)
  g_znet_session_engine:PostDataToZGateSession(session_id, message_type, message)

end
