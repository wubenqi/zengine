-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

if g_message_factory_table == nil then
  g_message_factory_table = {}
end


function CreateMessageAndData(message_type, buffer)
  -- 注意，消息有可能带数据也有可能不带数据
  local message = nil
  local result = 1
  if g_message_factory_table[message_type] ~= nil then
    message = g_message_factory_table[message_type].message
    result = 0
    if message ~= nil then
      -- 带数据
      message:Clear()
      if false == message:ParseFromIOBuffer(buffer) then
        result = 1
        ERROR("Parese message " .. g_message_factory_table[message_type].message_type .. " error")
      else
      end
    end
  else
    ERROR("message_type " .. message_type .. " not register!!!")
  end
  return result, message
end

function CreateMessage(message_type)
  local message = nil

  if g_message_factory_table[message_type] ~= nil and g_message_factory_table[message_type].message ~= nil then
    message = g_message_factory_table[message_type].message
    message:Clear()
  else
    ERROR("message_type " .. message_type .. " not register!!!")
  end
  return message
end
