-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------

-- ∂®“ÂEchodHandlerEngine
if EchodHandlerEngine == nil then
  EchodHandlerEngine = {
   class_name = "EchodHandlerEngine",
    ih = nil,
    session_id = nil,
  }
end

------------------------------------------------------------------------------------------

function EchodHandlerEngine:CreateInstance(o)
	o = o or { class_name = "EchodHandlerEngine" }
	setmetatable(o, {__index = self})
	return o
end

if g_znet_handler_engine == nil or g_znet_handler_engine.class_name == "ZNetHandlerEngine" then
  g_znet_handler_engine = ZNetHandlerEngine:CreateInstance(EchodHandlerEngine)
else
  --PrintTable(g_znet_hangler_engine)
  g_znet_handler_engine = ZNetHandlerEngine
end

------------------------------------------------------------------------------------------
function EchodHandlerEngine:OnNewConnection(ih)
  TRACE("EchodHandlerEngine:OnNewConnection()")
  return 0
end

function EchodHandlerEngine:OnDataReceived(ih, io_buffer)
  TRACE("EchodHandlerEngine:OnDataReceived()")
  ih:SendString(io_buffer)
  return 0
end

function EchodHandlerEngine:OnConnectionClosed(ih)
  TRACE("EchodHandlerEngine:OnConnectionClosed()")
  return 0
end
