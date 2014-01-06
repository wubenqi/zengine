-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------

function ZLoginSessionHandler:Initialize(ih)
  TRACE("ZNetSessionEngine:Initialize()")
end

function ZLoginSessionHandler:Destroy(ih)
  TRACE("ZNetSessionEngine:Destroy()")
end

function ZLoginSessionHandler:OnSessionDataHandler(ih, message_type, io_buffer)
  LOG("ZLoginSessionHandler:OnSessionDataReceived: recv message_type " .. message_type .. " from " .. session.addr)
  local buffer = IOBuffer()
  buffer:WriteUint32(1)
  buffer:WriteString("123456")
  ih:SendSessionIOBufferData(ih:GetSessionID(), message_typem, buffer)
end

function ZLoginSessionHandler:OnNewSession(session)
  LOG("OnSessionNew: from " .. session.addr)
end

function ZLoginSessionHandler:OnSessionClosed(session)
  LOG("ZNetHandlerEngine:OnSessionClosed: from " .. session.addr)
end
