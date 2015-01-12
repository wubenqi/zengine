-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------

kNetType_TCPConnector = "TCPConnector"
kNetType_TCPAcceptor = "TCPAcceptor"
------------------------------------------------------------------------------------------
if ZNetHandlerEngine == nil then
  ZNetHandlerEngine = {
    class_name = "ZNetHandlerEngine"
  }
end

function ZNetHandlerEngine:CreateInstance(o)
	o = o or { class_name = "ZNetHandlerEngine" }
	setmetatable(o, {__index = self})
	return o
end


function ZNetHandlerEngine:OnNewConnection(ih)
  TRACE("ZNetHandlerEngine:OnNewConnection()")
  return 0
end

function ZNetHandlerEngine:OnDataReceived(ih, io_buffer)
  TRACE("ZNetHandlerEngine:OnDataReceived()")
  return 0
end

function ZNetHandlerEngine:OnConnectionClosed(ih)
  TRACE("ZNetHandlerEngine:OnConnectionClosed()")
  return 0
end

if g_znet_handler_engine == nil then
  g_znet_handler_engine = ZNetHandlerEngine
end
------------------------------------------------------------------------------------------

function OnNewConnection(ih)
  return g_znet_handler_engine:OnNewConnection(ih)
end

function OnDataReceived(ih, io_buffer)
  return g_znet_handler_engine:OnDataReceived(ih, io_buffer)
end

function OnConnectionClosed(ih)
  return g_znet_handler_engine:OnConnectionClosed(ih)
end
