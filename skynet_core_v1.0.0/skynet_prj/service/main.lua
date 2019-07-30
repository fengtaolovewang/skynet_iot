
local skynet = require "skynet"
local sprotoloader = require "sprotoloader"

local max_client = 64
skynet.start(function()
	skynet.error("Server start")
	skynet.uniqueservice("protoloader")
	if not skynet.getenv "daemon" then
		local console = skynet.newservice("console")
	end
	skynet.newservice("debug_console",8000)
--	skynet.newservice("simpledb")
--	local watchdog = skynet.newservice("zigbeewatchdog")
--	skynet.call(watchdog, "lua", "start", {
--		port = 8888,
--		maxclient = max_client,
--		nodelay = true,
--	})
--	skynet.error("Watchdog listen on", 8888)
	local watchdog = skynet.newservice("dbwatchdog")
	skynet.call(watchdog, "lua", "start", {
		port = 9999,
		maxclient = 1,
		nodelay = true,
	})
	skynet.error("Watchdog listen on", 9999)
	skynet.exit()
end)
