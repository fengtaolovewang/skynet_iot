package.cpath = "user/lua_os/zigbee/luaclib/?.so"
package.path = "user/lua_os/zigbee/?.lua"

if _VERSION ~= "Lua 5.3" then
	error "Use lua 5.3"
end

local socket = require "client.socket"
local proto = require "zigbeeproto"
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
-----------------------------------------------------------
local REQUEST = {}

function REQUEST:network_start()
	print("network_start",self.power)
	zbcall.network_start(self.power)
end

function REQUEST:network_open()
	print("network_open")
	zbcall.network_open()
end

function REQUEST:network_close()
	print("network_close")
	zbcall.network_close()
end

function REQUEST:network_leave()
	print("network_leave")
	zbcall.network_leave();
end

function REQUEST:network_del()
	print("network_del", self.mac)
	zbcall.network_del(self.mac)
end

--	print("network_status")
function REQUEST:network_status()
	zbcall.network_status()
end

--	print("network_setpower", self.power)
function REQUEST:network_setpower()
	zbcall.network_setpower(self.power)
end

--	print("zcl_control", self.mac, self.ep,self.cluster,self.type,self.cmd,self.data)
function REQUEST:zcl_control()
	zbcall.zcl_control(self.mac, self.ep,self.cluster,self.type,self.cmd,self.data)
end

--	print("zcl_onoff", self.mac, self.ep,self.onoff)
function REQUEST:zcl_onoff()
	zbcall.zcl_onoff(self.mac, self.ep,self.onoff)
end

--	print("zcl_onoff", self.mac, self.ep,self.cluster,self.attrid)
function REQUEST:zcl_read()
	zbcall.zcl_read(self.mac, self.ep,self.cluster,self.attrid)
end

--	print("zcl_onoff", self.mac, self.ep)
function REQUEST:zcl_factory()
	zbcall.zcl_factory(self.mac, self.ep)
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
local function zigbee_handshake()
	send_request("handshake")
end

zigbee_networkstate =function(mac,power,pid,channel,version)
	print("zigbee_networkstate")
	send_request("network_state",{mac = mac,power = power,pid = pid,channel = channel,version=version})
end

zigbee_zclreport =function(mac,ep,cluster,data)
	print("zigbee_zclreport",ep,cluster)
	send_request("zcl_report",{mac = mac,ep = ep,cluster = cluster,data = data})
end

zigbee_zclread =function(mac,ep,cluster,data)
print("zigbee_zclread",ep,cluster)
	send_request("zcl_read",{mac = mac,ep = ep,cluster = cluster,data = data})
end

zigbee_zclcmd =function(mac,ep,type,cluster,cmd,data)
print("zigbee_zclcmd",ep,cluster,type,cmd)
	send_request("zcl_cmd",{mac = mac,ep = ep,type=type,cluster = cluster,cmd=cmd,data = data})
end

zigbee_devleft =function(mac)
print("zigbee_devleft")
	send_request("dev_left",{mac = mac})
end

zigbee_devonline =function(mac,ep,deviceId,incluster,outcluster)
print("zigbee_devonline")
	send_request("dev_online",{mac = mac,ep = ep,deviceId = deviceId,incluster = incluster,outcluster = outcluster})
end

zigbee_rssireport =function(mac,rssi,lqi)
print("zigbee_rssireport")
	send_request("rssi_report",{mac = mac,rssi = rssi,lqi = lqi})
end

zigbee_apssent =function(mac,status,cluster,cmd)
print("zigbee_apssent")
	send_request("aps_sent",{mac = mac,status = status,cluster = cluster,cmd = cmd})
end

zigbee_eplist =function(mac,cont,list)
print("zigbee_eplist")
	send_request("ep_list",{mac = mac,cont = cont,list = list})
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
fd = assert(socket.connect("127.0.0.1", 8888))
zigbee_handshake()
function zigbee_core()
		dispatch_package()
end
