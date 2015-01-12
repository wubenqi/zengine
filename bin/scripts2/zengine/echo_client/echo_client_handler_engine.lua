-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------

-- ∂®“ÂEchoClientHandlerEngine
if EchoClientHandlerEngine == nil then
  EchoClientHandlerEngine = {
   class_name = "EchoClientHandlerEngine"
  }
end

------------------------------------------------------------------------------------------

function EchoClientHandlerEngine:CreateInstance(o)
	o = o or { class_name = "EchoClientHandlerEngine" }
	setmetatable(o, {__index = self})
	return o
end

--print(g_znet_handler_engine)

if g_znet_handler_engine == nil or g_znet_handler_engine.class_name == "ZNetHandlerEngine" then
  g_znet_handler_engine = ZNetHandlerEngine:CreateInstance(EchoClientHandlerEngine)
else
  print(g_znet_hangler_engine)
  g_znet_handler_engine = ZNetHandlerEngine
end

------------------------------------------------------------------------------------------
function EchoClientHandlerEngine:OnNewConnection(ih)
  TRACE("EchoClientHandlerEngine:OnNewConnection()")
  ih:SendString("Hello zengine")
  return 0
end

function EchoClientHandlerEngine:OnDataReceived(ih, io_buffer)
  TRACE("EchoClientHandlerEngine:OnDataReceived()")
  print(io_buffer)
  --print(io_buffer:RetrieveAllAsString())
  return 0
end

function EchoClientHandlerEngine:OnConnectionClosed(ih)
  TRACE("EchoClientHandlerEngine:OnConnectionClosed()")
  return 0
end


function BB(s)
 print(s)
end