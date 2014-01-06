-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------------------------
if ZLoginMainUtil == nil then
  ZLoginMainUtil = {
    class_name = "ZLoginMainUtil"
  }
end

function ZLoginMainUtil:MainInitialize()
  TRACE("ZLoginMainUtil:MainInitialize()")
  --LOG(GenerateHashKey())
  return 0
end

function ZLoginMainUtil:MainDestroy()
  TRACE("ZLoginMainUtil:MainDestroy()")
  return 0
end

if g_main_util == nil or g_main_util.class_name == "MailUtil" then
  g_main_util = MainUtil:CreateInstance(ZLoginMainUtil)
else
  --PrintTable(g_main_util)
  g_main_util = ZLoginMainUtil
end
