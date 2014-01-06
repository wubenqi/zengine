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
    if self[message_type] ~= nil then
      return self[message_type](self, session, message_type, io_buffer)
    else
      ERROR("recv a invalid message_type, maybe not register message_type: " .. message_type)
    end
  end
  return 0
end

function ZGateSessionHandler:OnNewSession(session)
  LOG("OnSessionNew: from " .. session.addr)
  session.session_state = kSessionState_SessionCreated
end

function ZGateSessionHandler:OnSessionClosed(session)
  LOG("ZNetHandlerEngine:OnSessionClosed: from " .. session.addr)
  session.session_state = kSessionState_SessionClosed
end
