-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--


--
-- 数据库和LUA的映射定义
-- 全局数据表

--[[
-- 数据库映射层
g_zcache_db_mgr = {
  ["t_player"] = {
    -- [aid] = {}
  },
  ["t_char"] = {
    -- [aid] = {}
  },
  ["t_item"] = {
    -- [aid] = {}
  },
  --todo
}
--]]

function ZDBLoadPlayerInfos(conn, player_infos)
  --local player_infos = nil
  
  -- 加载所有玩家数据
  local answ = conn:Query("SELECT aid,acct_name,password,player_name FROM t_player WHERE 1")
  if answ ~= nil then
    --player_infos = {}
    repeat
      local player_info = {}
      player_info.aid = answ:GetColumnInt(0)
      player_info.acct_name = answ:GetColumn(1)
      player_info.password = answ:GetColumn(2)
      player_info.player_name = answ:GetColumn(3)
      player_infos[player_info.aid] = player_info
      --LOG(json.encode(player_info))
    until answ:FetchRow() == false  
  end
  
  --return player_infos
end

function ZDBLoadAccountDatas(conn, account_datas)
  --local player_infos = nil
  
  -- 加载所有玩家数据
  local answ = conn:Query("SELECT aid,acct_name,password,player_name FROM t_player WHERE 1")
  if answ ~= nil then
    --player_infos = {}
    repeat
      local account_data = {}
      account_data.aid = answ:GetColumnInt(0)
      account_data.acct_name = answ:GetColumn(1)
      account_data.password = answ:GetColumn(2)
      account_data.player_name = answ:GetColumn(3)
      account_datas[account_data.acct_name] = account_data
      --LOG(json.encode(account_data))
    until answ:FetchRow() == false  
  end
  
  --return player_infos
end

function ZDBLoadPlayerData(conn, player_aid)
  -- 加载所有玩家数据
  local player = nil
  local answ = conn:Query("SELECT aid,acct_name,player_name,gender,face,class,reputation,gold,rmb,xianqi,extra_xianqi,aura,scene,scene_x,scene_y FROM t_player WHERE aid=" .. player_aid)
  if answ ~= nil then
    player = {}
    player.aid = answ:GetColumnInt(0)
    player.acct_name = answ:GetColumn(1)
    player.player_name = answ:GetColumn(2)
    player.gender = answ:GetColumnInt(3)
    player.face = answ:GetColumnInt(4)
    player.class = answ:GetColumnInt(5)
    player.reputation = answ:GetColumnInt(6)
    player.gold = answ:GetColumnInt(7)
    player.rmb = answ:GetColumnInt(8)
    player.xianqi = answ:GetColumnInt(9)
    player.extra_xianqi = answ:GetColumnInt(10)
    player.aura = answ:GetColumnInt(11)
    player.scene = answ:GetColumnInt(12)
    player.scene_x = answ:GetColumnInt(13)
    player.scene_y = answ:GetColumnInt(14)
    --LOG(json.encode(player))
  end
  return player
end

-- 加载玩家角色数据
function ZDBLoadPlayerCharData(conn, player_aid)
  local chars = nil
  local answ = conn:Query("SELECT aid,player_aid,char_name,is_human,gender,level,class,exp,rage,hp_add,skill_active,skill,life_add,att_add,def_add,formation_index FROM t_char WHERE player_aid=" .. player_aid)
  if answ ~= nil then
    chars = {}
    repeat
      local char = {}
      char.aid = answ:GetColumnInt(0)
      char.player_aid = answ:GetColumnInt(1)
      char.char_name = answ:GetColumn(2)
      char.is_human = answ:GetColumnInt(3)
      char.gender = answ:GetColumnInt(4)
      char.level = answ:GetColumnInt(5)
      char.class = answ:GetColumnInt(6)
      char.exp = answ:GetColumnInt(7)
      char.rage = answ:GetColumnInt(8)
      char.hp_add = answ:GetColumnInt(9)
      char.skill_active = answ:GetColumnInt(10)
      char.extra_xianqi = answ:GetColumnInt(11)
      char.skill = answ:GetColumn(12)
      char.life_add = answ:GetColumnInt(13)
      char.att_add = answ:GetColumnInt(14)
      char.def_add = answ:GetColumnInt(15)
      char.formation_index = answ:GetColumnInt(16)
      --LOG(json.encode(char))
      chars[char.aid] = char
    until answ:FetchRow() == false  
  end
  return chars
end

-- 加载玩家道具数据
function ZDBLoadPlayerItemData(conn, player_aid)
  local items = {}
  local answ = conn:Query("SELECT aid,player_aid,char_aid,item_name,item_class,pos,item_count,intensify_level FROM t_item WHERE player_aid=" .. player_aid)
  if answ ~= nil then
    --items = {}
    repeat
      local item = {}
      item.aid = answ:GetColumnInt(0)
      item.player_aid = answ:GetColumnInt(1)
      item.char_aid = answ:GetColumnInt(2)
      item.item_name = answ:GetColumn(3)
      item.item_class = answ:GetColumnInt(4)
      item.pos = answ:GetColumnInt(5)
      item.item_count = answ:GetColumnInt(6)
      item.intensify_level = answ:GetColumnInt(7)
      --LOG(json.encode(item))
      items[item.aid] = item
    until answ:FetchRow() == false  
  end
  return items
end

-- 加载所有玩家任务数据
function ZDBLoadPlayerQuestData(conn, player_aid)
  local quests = {}
  local answ = conn:Query("SELECT aid,player_aid,quest_id,targets,state FROM t_quest WHERE player_aid=" .. player_aid)
  if answ ~= nil then
    repeat
      local quest = {}
      quest.aid = answ:GetColumnInt(0)
      quest.player_aid = answ:GetColumnInt(1)
      quest.quest_id = answ:GetColumnInt(2)
      quest.targets = answ:GetColumn(3)
      quest.state = answ:GetColumnInt(4)
      --LOG(json.encode(quest))
      quests[quest.aid] = quest
    until answ:FetchRow() == false  
  end
  return quests
end

-- 加载所有玩家功能开启数据
function ZDBLoadPlayerFunctionalityFlagData(conn, player_aid)
  local functionality_flags = {}
  local answ = conn:Query("SELECT * FROM t_functionality_flag WHERE player_aid=" .. player_aid)
  if answ ~= nil then
    functionality_flags = {}
    functionality_flag.aid = answ:GetColumnInt(0)
    functionality_flag.player_aid = answ:GetColumnInt(1)
    --LOG(json.encode(functionality_flag))
  end
  return functionality_flags
end

--[[
function ZDBLoadPlayerGameData(player_aid)
  local db_conn_pool_mgr = GetDBConnPoolMgr()
  local conn = db_conn_pool_mgr:GetFreeConnection()
  
  local cache_player_data = nil
  
  repeat
    -- 加载t_player
    local player = ZDBLoadPlayerData(conn, player_aid)
    if player == nil then
      break
    end
    
    -- 加载t_char
    local chars = ZDBLoadPlayerCharData(conn, player.aid)
    if chars == nil then
      break
    end
    
    cache_player_data = {
      ["t_player"] = { [player.aid] = player },
      ["t_char"] = chars,
    }
    
    -- 加载t_item
    cache_player_data["t_item"] = ZDBLoadPlayerItemData(conn, player.aid)
  until true
  
  db_conn_pool_mgr:SetFreeConnection(conn)
  return cache_player_data
end

-- PlayerGameData入库
function ZDBSetCachePlayerGameData(player_aid, cache_player_data)
  local cache_mgr = g_zcache_db_mgr
  cache_mgr["t_player"][player_aid] = cache_player_data["t_player"][player_aid]
  for k,v in pairs(cache_player_data["t_char"]) do
    cache_mgr["t_char"][k] = v
  end
  for k,v in pairs(cache_player_data["t_item"]) do
    cache_mgr["t_item"][k] = v
  end
  -- todo
end

-- 
function ZDBCheckCachePlayerGameData(player_aid)
  return g_zcache_db_mgr["t_player"][player_aid] ~= nil
end

function ZDBGetCachePlayerGameData(player_aid)
  local cache_data = nil
  if ZDBCheckCachePlayerGameData(player_aid) == false then
    cache_data = ZDBLoadPlayerGameData(player_aid)
    if cache_data ~= nil then
      ZDBSetCachePlayerGameData(player_aid, cache_data)
    end
  else
    local cache_mgr = g_zcache_db_mgr
    cache_data = { 
      ["t_player"] = { [player_aid] = cache_mgr["t_player"][player_aid] },
      ["t_char"] = {},
      ["t_item"] = {}
    }
    for k,v in pairs(cache_mgr["t_char"]) do
      if v.player_aid == player_aid then
        cache_data["t_char"][k] = v
      end
    end
    for k,v in pairs(cache_mgr["t_item"]) do
      if v.player_aid == player_aid then
        cache_data["t_item"][k] = v
      end
    end
  end
  return cache_data
end
--]]

--[[
g_zengine_db = {
  ["表名"] = {
    
  }
}

t_player_info = {
  player_aid = 0,             -- 玩家ID
  acctount_name = "wubenqi",  -- 请求帐号名
  player_name = "wubenqi",    -- 玩家名
  level = 1,                  -- 玩家主角ID
  -- todo
}

-- 玩家表
t_player = {
	 aid	 = 0,
	 acct_name = "",
	 password	= "",
	 player_name	= "",
	 gm	 = 0,
	 banned	= 0,
	 muted	= 0,
	 new_flag	= 0,
	 gender	= 0,
	 face	= 1,
	 class	= 0,
	 default_title	= 0,
	 titles	= "",
	 reputation	 = 0,
	 reputation_level	= 1,
	 gold = 10000,
	 rmb	= 10000,
	 xianqi	= 0,
	 extra_xianqi	= 0,
	 buff_xianqi	= 0,
	 aura	= 0,
	 ranking	= -1,
	 guild	= -1,
	 combat_power	= 0,
	 scene	= 1,
	 scene_x	= 500,
	 scene_y	= 500,
	 create_time	= "0000-00-00 00:00:00",
	 last_login	= "0000-00-00 00:00:00",
	 last_ip = "",
}

-- 玩家角色表
t_char = {
	 aid	 = 0,
	 player_aid	 = 0,
	 char_name	 = "",
	 is_human	 = 1,
	 gender	= 0,
	 level	= 1,
	 class	= 0,
	 exp	= 1,
	 rage	= 50,
	 hp_add	= 0,
	 skill_active	= 1,
	 skill	= "",
	 life	= 1,
	 life_add	= 0,
	 att	= 1,
	 att_add	= 0,
	 def	= 1,
	 def_add	= 0,
	 base_block	= 0,
	 base_crit	= 0,
	 base_miss	= 0,
	 base_wreck	= 0 ,
	 base_toughness	= 0,
	 base_hit	= 0,
	 formation_index = -1,
}

-- 道具表
t_item = {
  aid	= 0,
  
  player_aid	= 0,
  char_aid	= -1,
  item_index	= 0, -- 道具索引，也就是位置
  
  item_name	= "",
  item_class	= 0,
  --pos	= 0,
  item_count	= 1,
  intensify_level = 0,
}

-- 联系人表
t_contact = {
  aid	 = 0,
  player_aid	= 1,
  contact_player_aid = 1,
  group_type	= 1,
  last_contact_time = 1
}

-- 功能开启定义表
t_functionality_flag = {
  player_aid	= 1,
  bag = 0,
  quest = 0,
  pet = 0,
  --...
}

--]]
