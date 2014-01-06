-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------------------------
if ZCacheMainUtil == nil then
  ZCacheMainUtil = {
    class_name = "ZCacheMainUtil"
  }
end

function ZCacheMainUtil:MainInitialize()
  TRACE("ZCacheMainUtil:MainInitialize()")
  
  return 0
end

function ZCacheMainUtil:MainDestroy()
  TRACE("ZCacheMainUtil:MainDestroy()")
  return 0
end

if g_main_util == nil or g_main_util.class_name == "MailUtil" then
  g_main_util = MainUtil:CreateInstance(ZCacheMainUtil)
else
  --PrintTable(g_main_util)
  g_main_util = ZCacheMainUtil
end



------------------------------------------------------------------------------------------------------------
if ZCacheThreadUtil == nil then
  ZCacheThreadUtil = {
    class_name = "ZCacheThreadUtil"
  }
end

function ZCacheThreadUtil:OnThreadInitialize(zcontext)
  TRACE("ZCacheThreadUtil:OnThreadInitialize()")
  
  if zcontext:instance_name() == "io" then
    CreateZCacheDataManager()
  end
  
  return 0
end

function ZCacheThreadUtil:OnThreadDestroy(zcontext)
  TRACE("ZCacheThreadUtil:OnThreadDestroy()")
  return 0
end

if g_thread_util == nil or g_thread_util.class_name == "ThreadUtil" then
  g_thread_util = ThreadUtil:CreateInstance(ZCacheThreadUtil)
else
  g_thread_util = ZCacheThreadUtil
end
