-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--


-- cache服务器消息类型定义
zcache_message_types = {
  SERVER_DATA_REQ = 1,     -- 游戏逻辑服务器启动时请求全局数据
  SERVER_DATA_REP = 2,     -- 全局数据请求返回
  
  ACCOUNT_DATA_REQ = 3,    -- 请求帐号信息
  ACCOUNT_DATA_REP = 4,    -- 返回帐号数据

  PLAYER_INFO_REQ = 5,     -- 请求玩家信息，通常是需要联系人列表或联盟等离线玩家数据需要显示时请求
  PLAYER_INFO_REP = 6,     -- 返回玩家信息
  
  PLAYER_DATA_REQ = 7,     -- 请求玩家游戏数据
  PLAYER_DATA_REP = 8,     -- 返回玩家游戏数据
  
  PLAYER_ATTRS_DATA_REQ = 9,     -- 请求玩家属性数据，通常是查看离线玩家详细信息或竞技场和离线玩家战斗时的数据
  PLAYER_ATTRS_DATA_REP = 10,     -- 返回玩家属性数据
}
