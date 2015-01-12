-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------------------------
if EchodMainUtil == nil then
  EchodMainUtil = {
    class_name = "EchodMainUtil"
  }
end

function EchodMainUtil:MainInitialize()
  TRACE("EchodMainUtil:MainInitialize()")
  return 0
end

function EchodMainUtil:MainDestroy()
  --ctx:term()
  TRACE("EchodMainUtil:MainDestroy()")
  return 0
end

if g_main_util == nil or g_main_util.class_name == "MailUtil" then
  g_main_util = MainUtil:CreateInstance(EchodMainUtil)
else
  --PrintTable(g_main_util)
  g_main_util = EchodMainUtil
end
