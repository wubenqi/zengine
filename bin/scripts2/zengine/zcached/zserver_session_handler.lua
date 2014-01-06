-- Copyright 2010, www.lelewan.com
-- All rights reserved
--
-- Author: wubenqi<wubenqi@caitong.net>, 2013-01-25
--

------------------------------------------------------------------------------------------

function ZServerSessionHandler:Initialize(ih)
  TRACE("ZNetSessionEngine:Initialize()")
end

function ZServerSessionHandler:Destroy(ih)
  TRACE("ZNetSessionEngine:Destroy()")
end

function ZServerSessionHandler:OnSessionDataHandler(ih, message_type, io_buffer)
  LOG("ZServerSessionHandler:OnSessionDataReceived: recv message_type " .. message_type .. " from " .. session.addr)
end

function ZServerSessionHandler:OnNewSession(session)
  LOG("OnSessionNew: from " .. session.addr)
end

function ZServerSessionHandler:OnSessionClosed(session)
  LOG("ZNetHandlerEngine:OnSessionClosed: from " .. session.addr)
end

