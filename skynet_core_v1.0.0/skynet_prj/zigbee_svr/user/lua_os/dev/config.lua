local m=
{
	devstore_path="/dbconf/zigbeedev",
	uart_path	 ="/dev/ttyS0",
	log_path	 ="/tmp/zigbee_run.log",
	dev_path	 ="/dbconf/dbzigbee_lua/lua_dev",
	memsize		 = 64*1024,
	pthread_num  = 3,
	time_num	 = 32,
	udp_port	 = 11111,
}

print("---------------------zigbee.lua")
dofile("dbzigbee_lua/zigbee.lua")

local lua_config_load=function(a)
	print(a.devstore_path)
	print(a.uart_path)
	
	print(a.log_path)
	print(a.dev_path)
	
	print(a.memsize)
	print(a.pthread_num)
		
	print(a.time_num)
	print(a.udp_port)
	return a.devstore_path,a.uart_path,a.log_path,a.dev_path,a.memsize,a.pthread_num,a.time_num,a.udp_port
end

return lua_config_load(m)