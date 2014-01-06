-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------------------------
if ZGameMainUtil == nil then
  ZGameMainUtil = {
    class_name = "ZGameMainUtil"
  }
end

function ZGameMainUtil:MainInitialize()
  TRACE("ZLoginMainUtil:MainInitialize()")
  --LOG(GenerateHashKey())
  return 0
end

function ZGameMainUtil:MainDestroy()
  TRACE("ZLoginMainUtil:MainDestroy()")
  return 0
end

if g_main_util == nil or g_main_util.class_name == "MailUtil" then
  g_main_util = MainUtil:CreateInstance(ZGameMainUtil)
else
  --PrintTable(g_main_util)
  g_main_util = ZGameMainUtil
end
