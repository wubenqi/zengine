-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------
-- 主程序初始化
function MainInitialize()
  LOG("MainInitialize")  
  return 0
end

-- 主程序结束
function MainDestroy()
  LOG("MainDestroy")
  return 0
end

------------------------------------------------------------------------------------------
-- 线程创建和销毁
function OnThreadInitialize(zcontext)
  LOG("OnThreadInitialize ")
    
  zcontext:SetTimer(10 ,10)
  return 0
end

function OnThreadDestroy(zcontext)
  LOG("OnThreadDestroy ")
  return 0
end

------------------------------------------------------------------------------------------
-- 定时器
function OnTimer(zcontext, timer_id, tm)
  LOG("OnTimer: ".. timer_id)
  return 0
end

------------------------------------------------------------------------------------------
-- 异步消息
function OnTaskDataReceived(zcontext, task_data)
  LOG("OnTaskDataReceived: " .. task_data)
  return 0
end

------------------------------------------------------------------------------------------
-- 网络连接建立
function OnNewConnection(ih)
  --zcontext = ih:GetContext()
  LOG("OnNewConnection")
  --zcontext:PostTaskToMainThread("1111")
  return 0
end

-- 接收到网络数据
function OnDataReceived(ih, message_type, io_buffer)
  LOG("OnTCPPacketReceived")

  return 0
end

-- 网络连接关闭
function OnConnectionClosed(ih)
  LOG("OnConnectionClosed")  
  return 0
end
