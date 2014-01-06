-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--


-- 对象数据库管理层

g_zcache_data_mgr = nil

function CreateZCacheDataManager()
  -- 定义
  local zcache_data_mgr = {
    g_server_data = {   -- 全局游戏数据
      player_infos = {}, -- { [aid] = { t_player } }                -- 玩家简单信息
      account_datas = {}, -- { [account_name] = { t_player } },
      guild = {}, -- 联盟
      arena = {}, -- 竞技场
    },
    
    z_player_data_mgr = {      -- 玩家游戏数据
      --[[
      [player_aid] = {
        player = {
          player_data = t_player
          chars = { { char_data = t_char, equips = { { t_item ... } } } }
          bag = { items }
          storage = { items }
          battle_container = { chars }
          contacts = { contacts }
          quest = {}
          -- todo
          --   ...
        }
      }
      --]]
    },
  }
  
  
  ZLoadServerData(zcache_data_mgr.g_server_data)
  
  
  g_zcache_data_mgr = zcache_data_mgr
end

--[[
--]]

--  加载全局服务器数据
function ZLoadServerData(server_data)
  local db_conn_pool_mgr = GetDBConnPoolMgr()
  local conn = db_conn_pool_mgr:GetFreeConnection()
  
  ZDBLoadAccountDatas(conn, server_data.account_datas)
  
  db_conn_pool_mgr:SetFreeConnection(conn)
end

function ZLoadPlayerGameData(player_aid)
  local db_conn_pool_mgr = GetDBConnPoolMgr()
  local conn = db_conn_pool_mgr:GetFreeConnection()
  
  local cache_player_data = nil
  
  repeat
    -- 加载t_player
    local t_player = ZDBLoadPlayerData(conn, player_aid)
    if t_player == nil then
      break
    end
    
    -- 加载t_char
    local t_chars = ZDBLoadPlayerCharData(conn, player_aid)
    if t_chars == nil then
      break
    end
    
    -- 加载t_item
    local t_items = ZDBLoadPlayerItemData(conn, player_aid)
    
    cache_player_data = {
      player_data = t_player,
      chars = {},
      bag = {},
      storage = {},
      battle_container = {},
      contacts = {},
      quest = {},
      -- todo
      --   ...
    }
    
    for i,v in pairs(t_chars) do
      local char_data = { char_data = v, equips = {} }
      for i2, v2 in pairs(t_items) do
        if v2.char_aid == i then
          char_data.equips[v2.pos] = v2
        end
      end
      table.insert(cache_player_data.chars, char_data)
    end
    
    for i,v in pairs(t_items) do
      if v.char_aid == -1 then
        cache_player_data.bag[v.pos] = v
      end
    end
  until true
  
  db_conn_pool_mgr:SetFreeConnection(conn)
  return cache_player_data
end
