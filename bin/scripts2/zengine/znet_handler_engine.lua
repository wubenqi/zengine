-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------

-- 连接状态定义
kCommHandlerStateInvalid = 0	   -- 非法
kCommHandlerStateConnected = 1	 -- 已经连接
kCommHandlerStateNormal = 2 	   -- 正常工作
kCommHandlerStateClosed = 3	    -- 已经关闭

kNetType_TCPConnector = "TCPConnector"
kNetType_TCPAcceptor = "TCPAcceptor"

------------------------------------------------------------------------------------------
if ZNetSessionEngine == nil then
  ZNetSessionEngine = {
    class_name = "ZNetSessionEngine"
  }
end

function ZNetSessionEngine:CreateInstance(o)
	o = o or { class_name = "ZNetSessionEngine" }
	setmetatable(o, {__index = self})
	return o
end

function ZNetSessionEngine:OnSessionDataHandler(ih, message_type, io_buffer)
  TRACE("ZNetSessionEngine:OnSessionDataHandler()")
end

function ZNetSessionEngine:Initialize(ih)
  TRACE("ZNetSessionEngine:Initialize()")
end

function ZNetSessionEngine:Destroy(ih)
  TRACE("ZNetSessionEngine:Destroy()")
end

if g_znet_session_engine == nil then
  g_znet_session_engine = ZNetSessionEngine
end
------------------------------------------------------------------------------------------

function OnNewConnection(ih)
  TRACE("OnNewConnection(): ")
  local conn_net_type = ih:GetNetType()
  ih:set_conn_state(kCommHandlerStateConnected)
  local instance_name = ih:GetInstanceName()
  --self.znet_instacnes[instance_name] = {}
  
  if conn_net_type == kNetType_TCPConnector then
    -- 1. 如果是TCPConnector创建的
    -- 做身份认证
    local cs_server_challenge_request = CSServerChallengeRequest()
    cs_server_challenge_request.name = instance_name
    cs_server_challenge_request.remote_password = "fj2ksoke#"
    ih:SendSessionMessageData(ih:GetSessionID(), MESSAGE_CS_SERVER_CHALLENGE_REQUEST, cs_server_challenge_request)
  elseif conn_net_type == kNetType_TCPAcceptor then 
    -- 1. 如果是TCPAcceptor创建的，也即服务器
  else
    -- LOG
  end
  return 0
end

function OnDataReceived(ih, message_type, io_buffer)
  TRACE("OnDataReceived()")
  -- 连接状态
  local conn_state = ih:conn_state()
  local conn_net_type = ih:GetNetType()
  --local instance_name = ih:GetInstanceName()
  
  --local session_mgr = self.znet_instacnes[instance_name]
  --local session = session_mgr[session_id]
  
  if conn_net_type == kNetType_TCPConnector then
    -- 1. 如果是TCPConnector创建的
    if conn_state == kCommHandlerStateConnected then
      local r,session_id = io_buffer:ReadUint64()
      -- 连接到服务器
      if message_type == MESSAGE_SC_SERVER_CHALLENGE_ACK then
    	   local sc_server_challenge_ack = SCServerChallengeAck()
	       if false == sc_server_challenge_ack:ParseFromIOBuffer(io_buffer) then
		        return kErrorCode_ParseMessageError
	       end
	       if sc_server_challenge_ack.ret_code == 0 then
          ih:set_conn_state(kCommHandlerStateNormal)
	         LOG("session_id = " .. ih:GetReadableSessionID() .. " sc_server_challenge_ack is ok")
	       else
	         LOG("OnDataReceived - message_type[sc_server_challenge_ack]'s ret_code = " .. sc_server_challenge_ack.ret_code)
	       end
	       
	       -- g_znet_session_engine初始化
	       g_znet_session_engine:Initialize(ih)
      else
        LOG("conn_state is kCommHandlerStateConnected, but receive a state is kCommHandlerStateNormal data, message_type = " .. message_type)
      end
    elseif conn_state == kCommHandlerStateNormal then
      OnSessionDataHandler(ih, message_type, io_buffer)
    else 
      LOG("invalid state, current_state = " .. conn_state .. ", should is kCommHandlerStateConnected or kCommHandlerStateNormal") 
    end
  elseif conn_net_type == kNetType_TCPAcceptor then
    if conn_state == kCommHandlerStateConnected then
      local r,session_id = io_buffer:ReadUint64()
      if message_type == MESSAGE_CS_SERVER_CHALLENGE_REQUEST then
        -- cached服务器连接
        local cs_server_challegne_ack = CSServerChallengeRequest()
        if false == cs_server_challegne_ack:ParseFromIOBuffer(io_buffer) then
          return kErrorCode_ParseMessageError
        end
        LOG("recv cs_server_challegne_ack, name = ".. cs_server_challegne_ack.name .. ", remote_password = " .. cs_server_challegne_ack.remote_password)
        
        ih:set_conn_state(kCommHandlerStateNormal)
        local sc_server_challegne_ack = SCServerChallengeAck()
        sc_server_challegne_ack.ret_code = 0
        ih:SendSessionMessageData(ih:GetSessionID(), MESSAGE_SC_SERVER_CHALLENGE_ACK, sc_server_challegne_ack)
      else
        LOG("conn_state is kCommHandlerStateConnected, but receive a state is kCommHandlerStateNormal data, message_type = " .. message_type)
      end
    elseif conn_state == kCommHandlerStateNormal then
      OnSessionDataHandler(ih, message_type, io_buffer)
    else 
      LOG("invalid state, current_state = " .. conn_state .. ", should is kCommHandlerStateConnected or kCommHandlerStateNormal") 
    end
      
      -- 创建Session
      --
  end

  return 0
end

function OnConnectionClosed(ih)
  TRACE("ZNetHandlerEngine:OnNewConnection()")
  ih:set_conn_state(kCommHandlerStateInvalid)
  
  -- 连接断开，关闭所有Session
  g_znet_session_engine:Destroy(ih)
    
  return 0
end

function OnSessionDataHandler(ih, message_type, io_buffer)
  TRACE("OnSessionDataHandler()")
  g_znet_session_engine:OnSessionDataHandler(ih, message_type, io_buffer)
end

function SendSessionDataByTable(ih, session_id, message_type, json_table)
  local data = json.encode(json_table)
  --LOG("AAAAAAAAAA: " .. data)
  ih:SendSessionStringData(session_id, message_type, data)
end
