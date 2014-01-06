-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------

--g_zgate_session_mgr = {}
--
--ZGateSessionHandler = {
--}
--
-- 有状态，维护SESSION列表
kSessionState_SessionCreated = 1
kSessionState_WaitingAuthChallenge = 2
kSessionState_AuthChallenge = 3
kSessionState_WaitingAuthProof = 4
kSessionState_AuthProof = 5
kSessionState_SessionClosed = 6
kSessionState_SessionError = 7


function ZGateSessionHandler:Initialize(ih)
  TRACE("ZNetSessionEngine:Initialize()")
end

function ZGateSessionHandler:Destroy(ih)
  TRACE("ZNetSessionEngine:Destroy()")
end

function ZGateSessionHandler:OnSessionDataHandler(ih, message_type, io_buffer)
  LOG("ZGateSessionHandler:OnSessionDataReceived: recv message_type " .. message_type)
  local r,session_id = io_buffer:ReadUint64()
  
  local session = self.session_mgr[session_id]
  if self.session_mgr[session_id] == nil then
    session = { ih = ih, session_id = session_id, addr = ih:GetAddressString() }
    self.session_mgr[session_id] = session
  end
  
  -- local session = self.session_mgr[session_id] or self.session_mgr[session_id] = { ih = ih, session_id = session_id, addr = ih:GetAddressString() }
  local result = -1
  
  if message_type == MESSAGE_CS_SERVER_SESSION_NEW then
    result = self:OnNewSession(session)
  elseif message_type == MESSAGE_CS_SERVER_SESSION_CLOSE then
      result = self:OnSessionClosed(session)
  else
    if message_type == MESSAGE_CS_AUTH_CHALLENGE then
      result = self:OnCSAuthChallenge(session, message_type, io_buffer)
    elseif message_type == MESSAGE_CS_AUTH_PROOF then
      result = self:OnCSAuthProof(session, message_type, io_buffer)
    else
    ERROR("recv a invalid message_type session_data, message_type = " .. message_type .. ", from " .. session.addr)
    end
  end
  return 0
  
  -- TRACE("ZLoginSessionEngine:OnSessionDataHandler()")
end

function ZGateSessionHandler:OnNewSession(session)
  session.session_state = kSessionState_SessionCreated
  LOG("OnSessionNew: from " .. session.addr)
end

function ZGateSessionHandler:OnSessionClosed(session)
  session.session_state = kSessionState_SessionClosed
  LOG("ZNetHandlerEngine:OnSessionClosed: from " .. session.addr)
end

function ZGateSessionHandler:OnCSAuthChallenge(session, message_type, io_buffer)
  local cs_auth_challenge = CSAuthChallenge()
  if false == cs_auth_challenge:ParseFromIOBuffer(io_buffer) then
    return kErrorCode_ParseMessageError
  end

  cs_auth_challenge:PrintDebugString()
  -- 存储帐号信息  
  session.acct_name = cs_auth_challenge.uname
  --session.hash_key = GenerateHashKey()
  session.session_state = kSessionState_AuthChallenge
  
  -- SendToZCacheSessionData(session.session_id, message_type, tb)
  -- SendToZCacheSessionData()
  
  local account_data_req = { account_name = session.acct_name }
  LOG(json.encode(account_data_req))
  g_znet_session_engine:SendToZCacheSessionData(session.session_id, zcache_message_types.ACCOUNT_DATA_REQ, account_data_req)
  
  return 0
end

function ZGateSessionHandler:OnCSAuthProof(session, message_type, io_buffer)
  LOG("ZGateSessionHandler:OnCSAuthProof()")

  local cs_auth_proof = CSAuthProof()
  if false == cs_auth_proof:ParseFromIOBuffer(io_buffer) then
    return kErrorCode_ParseMessageError
  end

  local digest = session.hash_key .. "@" .. MD5String(session.password)
  digest = MD5String(digest)
  
  local sc_auth_proof = SCAuthProof()
  sc_auth_proof.ret_code = -1

  if digest == cs_auth_proof.hash_key then
    sc_auth_proof.ret_code = 0
    sc_auth_proof.player_id = session.player_id
  end
  
  session.ih:SendSessionMessageData(session.session_id, MESSAGE_SC_AUTH_PROOF, sc_auth_proof)

  return 0
end


function ZGateSessionHandler:OnZCacheSessionDataHandler(session_id, message_type, tb_data)
  local session = self.session_mgr[session_id]
  if self.session_mgr[session_id] ~= nil then
    if message_type == zcache_message_types.ACCOUNT_DATA_REP then
      self:OnAccountDataRep(session, message_type, tb_data)
    end
  end
end


function ZGateSessionHandler:OnAccountDataRep(session, message_type, tb_data)
  if session.session_state == kSessionState_AuthChallenge then
  
    session.hash_key = GenerateHashKey()
    session.player_id = tb_data.message.aid
    session.password = tb_data.message.password
    session.acct_name = tb_data.message.acct_name
    session.player_name = tb_data.message.player_name
    session.session_state = kSessionState_AuthChallenge

    --PrintTable(session)
    LOG(json.encode(tb_data))
    local sc_auth_challenge = SCAuthChallenge()
    sc_auth_challenge.hash_key = session.hash_key
    sc_auth_challenge.ret_code = 0
    
    session.ih:SendSessionMessageData(session.session_id, MESSAGE_SC_AUTH_CHALLENGE, sc_auth_challenge)
  else
    LOG("ZGateSessionHandler:OnAccountDataRep() error!")    
  end
  return 0
end

-- 