-- Copyright 2010, www.lelewan.com
-- All rights reserved
--

------------------------------------------------------------------------------------------
-- 主程序初始化
function MainInitialize()
  LOG("MainInitialize")
  
  --zengine:CreateZEngineThread("instance")
  --zengine:Initialize_NetEngineManager()
  --CreateTCPConnector("127.0.0.1", "9900", 10)
  --CreateZEngineThread("instance")
--[[  
  local sections = ConfigFile.GetInstance():GetSectionName()
  LOG(sections:size())
  for section in sections.string_vector do
    --print(name)
    LOG(section)
  end
--]]
--[[  
  local db_conn_pool = GetDBConnPoolMgr()
  local db_conn = db_conn_pool:GetFreeConnection()
  local answ = db_conn:Query("SELECT *	FROM t_player")
  if answ ~= nil then
    repeat
      LOG(UTF8ToNativeMB(answ:GetColumn(1)))
    until answ:FetchRow() == false  
  end
  db_conn_pool:SetFreeConnection(db_conn)
--]]

  local cs_create_room = CSCreateRoom();
  cs_create_room.app = "111"
  cs_create_room.creator_uuid = 1
  cs_create_room.creator_name = "222"
  cs_create_room.room_type = 2
  cs_create_room.room_name = "333"
  
  LOG(cs_create_room:Utf8DebugString())
  
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
  -- AsyncSendIOBuffer(MessageToIOBuffer(hammer::message::MESSAGE_CS_SERVER_CHALLENGE_REQUEST, &cs_server_challenge_request));
  return 0
end
