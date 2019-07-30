local skynet = require "skynet"

local CMD = {}
local SOCKET = {}
local gate
local agent = {}

function SOCKET.open(fd, addr)
	skynet.error("New client from : " .. addr)
	local id= skynet.newservice("zigbeeagent")
	agent[fd] = {
		id=id
	}
	
	skynet.call(id, "lua", "start", { gate = gate, client = fd, watchdog = skynet.self() })
end

local function close_agent(fd)
	local a = agent[fd]
	agent[fd] = nil
	
	if a.id then
		skynet.call(gate, "lua", "kick", fd)
		-- disconnect never return
		skynet.send(a.id, "lua", "disconnect")
	end
end

function SOCKET.close(fd)
	print("socket close",fd)
	close_agent(fd)
end

function SOCKET.error(fd, msg)
	print("socket error",fd, msg)
	close_agent(fd)
end

function SOCKET.warning(fd, size)
	-- size K bytes havn't send out in fd
	print("socket warning", fd, size)
end

function SOCKET.data(fd, msg)
end
-------------------------------------------------------------------------
-------------------------------------------------------------------------
-------------------------------------------------------------------------
-------------------------------------------------------------------------
-------------------------------------------------------------------------
function CMD.start(conf)
	skynet.call(gate, "lua", "open" , conf)
end

function CMD.close(fd)
	close_agent(fd)
end

---mac,ep,cluster,data
function CMD.report(fd,mac,...)
	local a= agent[fd]	
	
end

---mac,ep,type,cluster,cmd,data
function CMD.sent(fd,mac,...)
	local a= agent[fd]	
	
end

---mac,ep,type,cluster,cmd,data
function CMD.rssi(fd,mac,...)
	local a= agent[fd]	
	
end

---mac,ep,type,cluster,cmd,data
function CMD.cmd(fd,mac,...)
	local a= agent[fd]	
	
end

---mac,ep,cluster,data
function CMD.left(fd,mac,...)
	local a= agent[fd]		
	a[mac]=nil
	
end

---ep,deviceId,incluster,outcluster
function CMD.online(fd,mac,ep,deviceId,incluster,outcluster)
	print ("CMD.online",mac)

	local a= agent[fd]
	local e={}
	e[ep]={
			id=deviceId,
			inc=incluster,
			outc=outcluster
		}

	a[mac]=e
	
	print("dev",a[mac][ep].id)
end


skynet.start(function()
	skynet.dispatch("lua", function(session, source, cmd, subcmd, ...)
		if cmd == "socket" then
			local f = SOCKET[subcmd]
			f(...)
			-- socket api don't need return
		else
			local f = assert(CMD[cmd])
			skynet.ret(skynet.pack(f(subcmd, ...)))
		end
	end)

	gate = skynet.newservice("gate")
end)
