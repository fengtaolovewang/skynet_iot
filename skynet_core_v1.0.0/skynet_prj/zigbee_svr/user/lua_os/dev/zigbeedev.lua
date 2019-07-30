local skynet = require "skynet"
require "skynet.manager"	-- import skynet.register
local dev = {}
local host= {}
local CMD = {}

--network_del dev
function CMD.network_del(mac)
	
end

--zcl_control {mac,ep,cluster,cmd,data}
function CMD.zcl_control(pack)
	
end
----------------------------------------------------
------------------------mac,ep,type,cluster,cmd,data
----------------------------------------------------
-- report
--{mac,status,cluster,cmd}
function CMD.sent(mac,...)
	
	
end

---mac,ep,type,cluster,cmd,data
function CMD.rssi(mac,...)
	
	
end

---mac,ep,type,cluster,cmd,data
function CMD.cmd(mac,...)
	
	
end

---mac
function CMD.left(mac)

	
end

---ep,deviceId,incluster,outcluster
function CMD.online(mac,info)
	print ("CMD.online",mac)
	
	
end

---{mac,power,channel,pid,v}
function CMD.host(address,info)
	print ("CMD.host",info.power)
	
	host[info.mac]={
		id=address,
		power=info.power,
		channel=info.channel,
		pid=info.pid,
		v=info.v
	}
end

skynet.start(function()
	skynet.dispatch("lua", function(session, address, cmd, ...)

		local f = CMD[cmd]
		if f then
			skynet.ret(skynet.pack(f(...)))
		else
			error(string.format("Unknown CMD %s", tostring(cmd)))
		end
	end)
	
	skynet.register "zigbeedev"
end)
