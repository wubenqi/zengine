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

-- 定义ZLoginSessionEngine
if ZLoginSessionEngine == nil then
  ZLoginSessionEngine = {
   class_name = "ZLoginSessionEngine",
   ["zlogind"] = ZGateSessionHandler,
   ["zcached"] = ZCacheSessionHandler
  }
end

------------------------------------------------------------------------------------------

function ZLoginSessionEngine:CreateInstance(o)
	o = o or { class_name = "ZLoginSessionEngine" }
	setmetatable(o, {__index = self})
	return o
end

if g_znet_session_engine == nil or g_znet_session_engine.class_name == "ZNetSessionEngine" then
  g_znet_session_engine = ZNetSessionEngine:CreateInstance(ZLoginSessionEngine)
else
  --PrintTable(g_znet_hangler_engine)
  g_znet_session_engine = ZNetSessionEngine
end

------------------------------------------------------------------------------------------
function ZLoginSessionEngine:Initialize(ih)
  TRACE("ZLoginSessionEngine:Initialize()")
  local zsession_engine = self[ih:GetInstanceName()]
  if zsession_engine ~= nil then
    zsession_engine.ih = ih
    zsession_engine:Initialize(ih)
  else
    LOG("ZLoginSessionEngine:Initialize() -- not find session_engine by instance_name " .. ih:GetInstanceName())
  end
end

function ZLoginSessionEngine:Destroy(ih)
  TRACE("ZLoginSessionEngine:Destroy()")
  local zsession_engine = self[ih:GetInstanceName()]
  if zsession_engine ~= nil then
    zsession_engine:Destroy(ih)
    zsession_engine.ih = nil
  else
    LOG("ZLoginSessionEngine:Destroy() -- not find session_engine by instance_name " .. ih:GetInstanceName())
  end
end

function ZLoginSessionEngine:OnSessionDataHandler(ih, message_type, io_buffer)
  TRACE("ZLoginSessionEngine:OnSessionDataHandler()")
  local zsession_engine = self[ih:GetInstanceName()]
  if zsession_engine ~= nil then
    zsession_engine:OnSessionDataHandler(ih, message_type, io_buffer)
  else
    LOG("ZLoginSessionEngine:OnSessionDataHandler() -- not find session_engine by instance_name " .. ih:GetInstanceName())
  end
end

function ZLoginSessionEngine:SendToZGateSessionData(session_id, message_type, tb)
  local ih = self["zlogind"].ih
  if ih ~= nil then
    SendSessionDataByTable(ih, session_id, message_type, tb)
    --ih:SendSessionIOBufferData(session_id, message_type, io_buffer)
  end
end

function ZLoginSessionEngine:SendToZCacheSessionData(session_id, message_type, tb)
  local ih = self["zcached"].ih
  if ih ~= nil then
    SendSessionDataByTable(ih, session_id, message_type, tb)
    --ih:SendSessionIOBufferData(session_id, message_type, io_buffer)
  end
end

function ZLoginSessionEngine:PostDataToZGateSession(session_id, message_type, tb)
  self["zlogind"]:OnZCacheSessionDataHandler(session_id, message_type, tb)
end
