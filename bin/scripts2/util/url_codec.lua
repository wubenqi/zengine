local url={parsed={}} --存储decode出来的key-value

--private
local function escape(w)
	pattern="[^%w%d%._%-%* ]"
	s=string.gsub(w,pattern,function(c)
		local c=string.format("%%%02X",string.byte(c))
		return c
	end)
	s=string.gsub(s," ","+")
	return s
end

local function detail_escape(w)
	local t={}
	for i=1,#w do
		c = string.sub(w,i,i)
		b,e = string.find(c,"[%w%d%._%-'%* ]")
		if not b then
			t[#t+1]=string.format("%%%02X",string.byte(c))
		else
			t[#t+1]=c
		end
	end
	s = table.concat(t)
	s = string.gsub(s," ","+")
	return s
end

local function unescape(w)
	s=string.gsub(w,"+"," ")
	s,n = string.gsub(s,"%%(%x%x)",function(c)
		return string.char(tonumber(c,16))
	end)
	return s
end

---[=[
--public function
function url:new()
	local u={}    --对象
	u.old= nil    --上一次decode 字符串的值
	u.de = nil    --decode字符串后的结果值
	u.en = nil    --encode字符串后的结果值
	u.enold = nil --上一次encode 字符串的值
	u.len = 0     --记录parsed map的长度


	--设置metatable
	setmetatable(u,{__index=self})
	return u
end

function url:decode(line)
	assert(line == nil or type(line) == "string")
	if self.old == line then return print("url:decode have cache") ,self.de end
	if not line then return self.de end

	self.parsed=nil
	self.len = 0
	self.parsed={}
	for k,v in string.gmatch(line,"([^&=]+)=([^&=]+)") do
		self.len = self.len + 1
		self.parsed[k] = unescape(v)
	end

	if  self.len == 0 then return nil  end

	local r={}
	for k,v in pairs(self.parsed) do
		r[#r+1] = k.."="..v
	end
	self.de=table.concat(r,"&")
	self.old=line
	return self.de
end

function url:encode(t)
	if(type(t) == "string") then
			if t == self.enold then
				print("url:encode have cache")
				return self.en
			end

			local r={}
			for k,v in string.gmatch(t,"([^&=]+)=([^&=]+)") do
				r[#r+1] = escape(k) .."=".. escape(v)
			end
			self.en = table.concat(r,"&")
			self.enold = t
			return self.en
	elseif type(t) ~= "table" then return nil
	else
		local r = {}
		for k , v in pairs(t) do
			r[#r+1] = escape(k) .."=".. escape(v)

		end
		self.en = table.concat(r,"&")

		return self.en
	end
end

function url:parse(line)
	assert(line == nil or type(line) == "string")
	if not line then
		if self.len == 0 then return nil
		else return self.parsed end
	end
	if url:decode(line) == nil then return nil end
	return self.parsed
end

function url:get(k)
	if self.len > 0 then
		return self.parsed[k]
	else
		return nil
	end
end

--[[
--测试私有函数
test="泰囧<>2a+b=c.*!^()"
test2="tn=baiduhome_pg&ie=utf-8&bs=lua%26%3D+%E5%8C%B9%E9%85%8D%E6%B1%89%E5%AD%97&f=8&rsv_bp=1&rsv_spt=1&wd=%E6%B3%B0%E5%9B%A7%3C%3E2a%2Bb+%3D+c.*%21%5E%28%29&rsv_n=2&rsv_sug3=1&rsv_sug1=1&rsv_sug4=168&inputT=897"
--- [ [
io.write("test private funcitons\n")
io.write("test:",test,"\n")
io.write("结果:",escape(test),"\n")
io.write("结果:",detail_escape(test),"\n")
dec=detail_escape(test)
io.write("结果:",unescape(dec),"\n")
--] ]
--测试公有函数
show = function(m)
	if not m then io.write("empty")
	elseif type(m) == "string" then
		io.write(m)
	elseif type(m) == "table" then
		for k,v in pairs(m) do
			io.write(string.format("k=%-10s  v=%s\n",k,v))
		end
	else
		io.write("unkown")
	end
	io.write("\n")
end

io.write("test public funcitons\n")
local u = url:new()
io.write("encode(test):");show(u:encode(test))
io.write("encode(test2):");show(u:encode(test2))
io.write("decode(encode(test)):");show(u:decode(u:encode(test)))
io.write("decode(test2):");show(u:decode(test2))
io.write("decode(test2):");show(u:decode(test2))
io.write("encode(test2):");show(u:encode(test2))
io.write("parse(encode(test2)):");show(u:parse(u:encode(test2)))
io.write("encode(map):"); show(u:encode({a="x + 3*10 << 1&2",b="q=queryWord!"}))
io.write("decode(encode(map):");show(u:decode(u:encode({a="x + 3*10 << 1&2",b="q=queryWord!"})))
print("decode test2:",test2);
d=u:decode(test2)
print("decode test2 ret:",d);
print("encode d:",d)
e=u:encode(d)
print("encode d ret:",e)
p=u:parse(e)
show(p)
io.write("parse:");show(u:parse(u:encode(u:decode(test2))))

io.write("get f:");show(u:get("f"))
io.write("get a  :");show(u:get("a"))
io.write("get tn :");show(u:get("tn"))
io.write("get bw :");show(u:get("bw"))
io.write("get cda:");show(u:get("cda"))

--]=]
--]]

--return url
