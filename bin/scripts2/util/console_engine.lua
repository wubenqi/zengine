-- Copyright 2010, www.lelewan.com
-- All rights reserved
--
-- Author: wubenqi<wubenqi@caitong.net>, 2012-10-22
--

ConsoleEngine = {}

function ConsoleEngine:CreateInstance(o)
	o = o or { };
	setmetatable(o, self);
	self.__index = self;
	return o;
end

function ConsoleEngine:DoCommand(ih, cmd_line)
  local cmd_data = string.split(cmd_line, ' ')
  if #cmd_data >= 1 then
    self:OnConsoleCommandHandler(ih, cmd_data)
  end
  return 0
end

function ConsoleEngine:OnConsoleCommandHandler(ih, cmd_data)
  return 0
end

