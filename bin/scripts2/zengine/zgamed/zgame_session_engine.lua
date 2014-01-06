-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------

-- 定义ZGateSessionHandler
if ZGateSessionHandler == nil then
  ZGateSessionHandler = {
    ih = nil,
    session_mgr = {}
  }
end

-- 定义ZCacheSessionHandler
if ZCacheSessionHandler == nil then
  ZCacheSessionHandler = {
    ih = nil,
    session_mgr = {}
  }
end

-- 定义ZGameSessionEngine
if ZGameSessionEngine == nil then
  ZGameSessionEngine = {
   class_name = "ZGameSessionEngine",
   ["zgamed"] = ZGateSessionHandler,
   ["zcached"] = ZCacheSessionHandler
  }
end

------------------------------------------------------------------------------------------

function ZGameSessionEngine:CreateInstance(o)
	o = o or { class_name = "ZGameSessionEngine" }
	setmetatable(o, {__index = self})
	return o
end

if g_znet_session_engine == nil or g_znet_session_engine.class_name == "ZNetSessionEngine" then
  g_znet_session_engine = ZNetSessionEngine:CreateInstance(ZGameSessionEngine)
else
  --PrintTable(g_znet_hangler_engine)
  g_znet_session_engine = ZNetSessionEngine
end

------------------------------------------------------------------------------------------
function ZGameSessionEngine:Initialize(ih)
  TRACE("ZGameSessionEngine:Initialize()")
  local zsession_engine = self[ih:GetInstanceName()]
  if zsession_engine ~= nil then
    zsession_engine.ih = ih
    zsession_engine:Initialize(ih)
  else
    LOG("ZGameSessionEngine:Initialize() -- not find session_engine by instance_name " .. ih:GetInstanceName())
  end
end

function ZGameSessionEngine:Destroy(ih)
  TRACE("ZGameSessionEngine:Destroy()")
  local zsession_engine = self[ih:GetInstanceName()]
  if zsession_engine ~= nil then
    zsession_engine:Destroy(ih)
    zsession_engine.ih = nil
  else
    LOG("ZGameSessionEngine:Destroy() -- not find session_engine by instance_name " .. ih:GetInstanceName())
  end
end

function ZGameSessionEngine:OnSessionDataHandler(ih, message_type, io_buffer)
  TRACE("ZGameSessionEngine:OnSessionDataHandler()")
  local zsession_engine = self[ih:GetInstanceName()]
  if zsession_engine ~= nil then
    zsession_engine:OnSessionDataHandler(ih, message_type, io_buffer)
  else
    LOG("ZGameSessionEngine:OnSessionDataHandler() -- not find session_engine by instance_name " .. ih:GetInstanceName())
  end
end

function ZGameSessionEngine:SendToZGateSessionData(session_id, message_type, tb)
  local ih = self["zgamed"].ih
  if ih ~= nil then
    SendSessionDataByTable(ih, session_id, message_type, tb)
    --ih:SendSessionIOBufferData(session_id, message_type, io_buffer)
  end
end

function ZGameSessionEngine:SendToZCacheSessionData(session_id, message_type, tb)
  local ih = self["zcached"].ih
  if ih ~= nil then
    SendSessionDataByTable(ih, session_id, message_type, tb)
    --ih:SendSessionIOBufferData(session_id, message_type, io_buffer)
  end
end

function ZGameSessionEngine:PostDataToZGateSession(session_id, message_type, tb)
  self["zgamed"]:OnZCacheSessionDataHandler(session_id, message_type, tb)
end
