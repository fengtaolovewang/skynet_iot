package.cpath = "db/luaclib/?.so"
package.path = "db/?.lua"

if _VERSION ~= "Lua 5.3" then
	error "Use lua 5.3"
end

local socket = require "client.socket"
local proto = require "dbproto"
local sproto = require "sproto"
local fd

local host = sproto.new(proto.c2s):host "package"
local request = host:attach(sproto.new(proto.s2c))



local function send_package(fd, pack)
	local package = string.pack(">s2", pack)
	socket.send(fd, package)
end

local function unpack_package(text)
	local size = #text
	if size < 2 then
		return nil, text
	end
	local s = text:byte(1) * 256 + text:byte(2)
	if size < s+2 then
		return nil, text
	end

	return text:sub(3,2+s), text:sub(3+s)
end

local function recv_package(last)
	local result
	result, last = unpack_package(last)
	if result then
		return result, last
	end
	local r = socket.recv(fd)
	if not r then
		return nil, last
	end
	if r == "" then
		error "Server closed"
	end
	return unpack_package(last .. r)
end

local session = 0

local function send_request(name, args)
	session = session + 1
	local str = request(name, args, session)
	send_package(fd, str)
	print("Request:", session)
end

local last = ""
-------------------------------------------------------------------------------
local REQUEST = {}
local D = {}
local RD = {}
-------------------------------------------------------------------------------
db_insert =function(num)
	print("db_insert",num)
	return table.unpack(D)
end

function REQUEST:insert()
	print("insert",self.name,self.num,self.data)
	D=nil

	for v in string.gmatch(self.data, "%w+") do
		if not v then
			table.insert(D, v)
		end
	end	
	--table.concat()
	db.insert(self.name,self.num)

end
---------------------------------------------------------------------------------
db_get =function(...)
    local n= select("#",...)
    local args={...}

    print "db_get"
    for i=1,n,1 do
        table.insert(RD,args[i])
        table.insert(RD,' ')
    end
end
--// sql :: "select * from %s"
--// sql :: "select * from %s where zongID!='ff'"
function REQUEST:get()
	print("get",self.name,self.num)
	RD=nil
	db.get(self.sql,self.num)
	return {result=table.concat(RD)}
end
--------------------------------------------------------------------------------
--sql :: "delete from %s where %s='%s' and %s='%s'"
function REQUEST:del()
	print("db del",self.sql)
	db.del(self.sql)
end
---------------------------------------------------------------------------------
-- sql :"UPDATE   %s       SET  %s='%s',%s='%s' WHERE %s='%s' and %s='%s' "
function REQUEST:updata()
	print("updata",self.sql)
	db.updata(self.sql);
end

--"create table zigbeedevtable(macaddr,nwkaddr,capabilities,devstatus,online,groups,company);"
function REQUEST:creat()
	print("creat", self.sql)
	db.creat(self.sql)
end


local function process_request(name, args,response)
	print("REQUEST", name)
	local f = assert(REQUEST[name])
	local r = f(args)
	
	if response then
		return response(r)
	end	
end
------------------------------------------------------------------------------------
local function db_handshake()
	send_request("handshake")
end


local function process_response(session, args)
	print("RESPONSE", session)
--[[	
	if args then
		for k,v in pairs(args) do
			print(k,v)
		end
	end
	]]--
	
end

local function process_package(t, ...)
	if t == "REQUEST" then
		local ok, result  = pcall(process_request, ...)
			if ok then
				if result then
					send_package(result)
				end
			end
	else
		assert(t == "RESPONSE")
		process_response(...)
	end
end
------------------------------------------------------
local function dispatch_package()
	while true do
		local v
		v, last = recv_package(last)
		if not v then
			break
		end

		process_package(host:dispatch(v))
	end
end
fd = assert(socket.connect("127.0.0.1", 9999))
db_handshake()

function db_core()
		dispatch_package()
end
