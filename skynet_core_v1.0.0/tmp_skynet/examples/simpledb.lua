local skynet = require "skynet"
require "skynet.manager"	-- import skynet.register
local db = {}

local command = {}

function command.GET(key)
	return db[key]
end

function command.SET(key, value)
	local last = db[key]
	db[key] = value
	return last
end

function command.TEST(msg)
--	print "one test"
	
	local function test()
		print(msg)
	end
	test()
end

skynet.info_func(function() return command.TEST("12345") end)
skynet.start(function()
	skynet.dispatch("lua", function(session, address, cmd, ...)
		cmd = cmd:upper()
		if cmd == "PING" then
			assert(session == 0)
			local str = (...)
			if #str > 20 then
				str = str:sub(1,20) .. "...(" .. #str .. ")"
			end
			skynet.error(string.format("%s ping %s", skynet.address(address), str))
			return
		end
		local f = command[cmd]
		if f then
			skynet.ret(skynet.pack(f(...)))
		else
			error(string.format("Unknown command %s", tostring(cmd)))
		end
	end)
--	skynet.traceproto("lua", false)	-- true off tracelog
	skynet.register "SIMPLEDB"
end)
