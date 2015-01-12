-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

local print = print
local tconcat = table.concat
local tinsert = table.insert
local srep = string.rep
local type = type
local pairs = pairs
local tostring = tostring
local next = next
local io = io
------------------------------------------------------------------------------------------------------------
function PrintTable(root)
	local cache = {  [root] = "." }
	local function _dump(t,space,name)
		local temp = {}
		for k,v in pairs(t) do
			local key = tostring(k)
			if cache[v] then
				tinsert(temp,"+" .. key .. " {" .. cache[v].."}")
			elseif type(v) == "table" then
				local new_key = name .. "." .. key
				cache[v] = new_key
				tinsert(temp,"+" .. key .. _dump(v,space .. (next(t,k) and "|" or " " ).. srep(" ",#key),new_key))
			else
				tinsert(temp,"+" .. key .. " [" .. tostring(v).."]")
			end
		end
		return tconcat(temp,"\n"..space)
	end
	io.stderr:write(_dump(root, "",""))
	io.stderr:flush()
end

------------------------------------------------------------------------------------------------------------
string.split = function(s, p)
  local rt= {}
  string.gsub(s, '[^'..p..']+', function(w) tinsert(rt, w) end )
  return rt
end

--local kHashKey = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
--const static int kSessionKeyLen = 32;
--生成一个64字节的键
function GenerateHashKey()
  local hash_key = ""
  for i=1,32 do 
    hash_key = hash_key .. string.format("%02X", math.random(0,255))
  end
  
  return hash_key
end
