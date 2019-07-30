
local skynet = require "skynet"
local socket = require "skynet.socket"
local sproto = require "sproto"
local sprotoloader = require "sprotoloader"

local WATCHDOG
local host
local send_request

local CMD = {}
local REQUEST = {}
local zigbee_gate={}
local client_fd


local function send_package(pack)
	local package = string.pack(">s2", pack)
	socket.write(client_fd, package)
end
-----------------------------------------------------------
function CMD.start(conf)
	local fd = conf.client
	local gate = conf.gate
	WATCHDOG = conf.watchdog
	-- slot 1,2 set at main.lua
	host = sprotoloader.load(2):host "package"
	send_request = host:attach(sprotoloader.load(1))
	skynet.fork(function()
		while true do
			send_package(send_request "heartbeat")
			skynet.sleep(500)
		end
	end)

	client_fd = fd
	skynet.call(gate, "lua", "forward", fd)
end

function CMD.disconnect()
	-- todo: do something before exit
	zigbee_gate=nil
	skynet.exit()
end

--network_start
function CMD.network_start(power)
	send_package(send_request("network_start",{power=power}))
end

--network_open
function CMD.network_open()
	send_package(send_request("network_open"))
end

--network_close
function CMD.network_close()
	send_package(send_request("network_close"))
end

--leave zigbee network
function CMD.network_leave()
	print("network_leave!!")
	send_package(send_request("network_leave"))
end

--network_del dev
function CMD.network_del(mac)
	send_package(send_request("network_del",{mac=mac}))
end

--network_status
function CMD.network_status()
	print "get network_status!!!"
	send_package(send_request("network_status"))
end

--network_setpower
function CMD.network_setpower(power)
	send_package(send_request("network_setpower",{power=power}))
end

--zcl_control {mac,ep,cluster,cmd,data}
function CMD.zcl_control(pack)
	send_package(send_request("zcl_control",pack))
end

--zcl_onoff {mac,ep,onoff}
function CMD.zcl_onoff(pack)
	send_package(send_request("zcl_onoff",pack))
end

--zcl_read {mac,ep,cluster,attrid}
function CMD.zcl_read(pack)
	send_package(send_request("zcl_read",pack))
end

--zcl_factory
function CMD.zcl_factory(mac,ep)
	send_package(send_request("zcl_factory",{mac=mac,ep=ep}))
end
--------------------------------------------------------------------------------------
-----------------------------------PROCESS--------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function REQUEST:handshake()
	print("handshake!!!")
	CMD.network_status()
--	return { msg = "Welcome to skynet, I will send heartbeat every 5 sec." }
end

function REQUEST:network_state()
	print("network_state",self.version,self.power,self.channel)

		zigbee_gate["mac"]=self.mac
		zigbee_gate["power"]=self.power
		zigbee_gate["channel"]=self.channel
		zigbee_gate["pid"]=self.pid
		zigbee_gate["version"]=self.version

	print ("stat",zigbee_gate["power"],zigbee_gate["channel"])
	if(zigbee_gate["channel"]==0) then
		CMD.network_start(13)
		skynet.timeout(500,CMD.network_status)
		print("time",skynet.time())
	else
		CMD.network_open()
	end
	
end

function REQUEST:rssi_report()
	--{mac,rssi,lqi}
	print("rssi_report!!!", self.rssi,self.lqi)
	skynet.send(WATCHDOG, "lua", "report",client_fd,self.mac, self.rssi,self.lqi)
end

function REQUEST:aps_sent()
	--{mac,status,cluster,cmd}
	print("aps_sent!!!", self.status,self.cluster,self.cmd)
	skynet.send(WATCHDOG, "lua", "report",client_fd,self.mac, self.status,self.cluster,self.cmd)
end

function REQUEST:zcl_report()
	--{mac,ep,cluster,data}
	print("zcl_report!!!", self.ep,self.cluster)
	skynet.send(WATCHDOG, "lua", "report",client_fd,self.mac, self.ep,self.cluster,self.data)
end

function REQUEST:zcl_read()
	print("zcl_read!!!",self.ep,self.cluster)
	--{mac,ep,cluster,data}
	skynet.send(WATCHDOG, "lua", "report",client_fd,self.mac, self.ep,self.cluster,self.data)
end

function REQUEST:zcl_cmd()
	print("zcl_cmd!!!",self.ep,self.cluster,self.type,self.cmd)
	--{mac,ep,type,cluster,cmd,data}
	skynet.send(WATCHDOG, "lua", "cmd",client_fd,self.mac, self.ep,self.type,self.cluster,self.cmd,self.data)
end

function REQUEST:dev_left()
	--{mac}
	print("dev_left!!!")
	--skynet.send(WATCHDOG, "lua", "left",client_fd,self.mac)
end

function REQUEST:dev_online()
	print("dev_online!!!",self.deviceId,self.ep)
	--{mac,ep,deviceId,incluster,outcluster}
	skynet.send(WATCHDOG, "lua", "online",client_fd,self.mac,self.ep,self.deviceId,self.incluster,self.outcluster)
	CMD.zcl_factory(self.mac,self.ep)	
end

function REQUEST:quit()
	skynet.call(WATCHDOG, "lua", "close", client_fd)
end

-------------------------------------------------------------------------------
local function request(name, args, response)
	local f = assert(REQUEST[name])
	local r = f(args)
	
	if response then
		return response(r)
	end
end

-------------------------------------------------------------------------------
skynet.register_protocol {
	name = "client",
	id = skynet.PTYPE_CLIENT,
	unpack = function (msg, sz)
		return host:dispatch(msg, sz)
	end,
	dispatch = function (fd, _, type, ...)
		assert(fd == client_fd)	-- You can use fd to reply message
		skynet.ignoreret()	-- session is fd, don't call skynet.ret
		skynet.trace()
		if type == "REQUEST" then
			local ok, result  = pcall(request, ...)
			if ok then
				if result then
					send_package(result)
				end
			else
				skynet.error(result)
			end
		else
			assert(type == "RESPONSE")
			error "This zigbee agent doesn't support request client"
		end
	end
}
---------------------------------------------------------------

skynet.start(function()
	skynet.dispatch("lua", function(_,_, command, ...)
		skynet.trace()
		local f = CMD[command]
		skynet.ret(skynet.pack(f(...)))
	end)
end)
