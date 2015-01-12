-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------------------------
if EchoClientMainUtil == nil then
  EchoClientMainUtil = {
    class_name = "EchoClientMainUtil"
  }
end

function EchoClientMainUtil:MainInitialize()
  TRACE("EchoClientMainUtil:MainInitialize()")
  return 0
end

function EchoClientMainUtil:MainDestroy()
  --ctx:term()
  TRACE("EchoClientMainUtil:MainDestroy()")
  return 0
end

if g_main_util == nil or g_main_util.class_name == "MailUtil" then
  g_main_util = MainUtil:CreateInstance(EchoClientMainUtil)
else
  --PrintTable(g_main_util)
  g_main_util = EchoClientMainUtil
end
