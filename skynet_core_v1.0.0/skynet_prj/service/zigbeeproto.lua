local sprotoparser = require "sprotoparser"

local proto = {}

------------------------------------ zigbee client api  skynet agent--->zigbee client
proto.c2s = sprotoparser.parse [[
.package {
	type 0 : integer
	session 1 : integer
}

network_start 1 {
	request {
		power 0 : integer
	}
}

network_open 2 {}

network_close 3 {}

network_leave 4 {}

network_del 5 {
	request {
		mac 0 : string
	}
}

network_status 6 {}

network_setpower 7 {
	request {
		power 0 : integer
	}
}

zcl_control 8 {
	request {
		mac 0 : string
		ep 1 : integer
		cluster 2 : integer
		type 3 : integer
		cmd 4 : integer
		data 5 : string
	}
}

zcl_onoff 9 {
	request {
		mac 0 : string
		ep 1 : integer
		onoff 2 : integer
	}
}

zcl_read 10 {
	request {
		mac 0 : string
		ep 1 : integer
		cluster 2 : integer
		attrid 3 : integer
	}
}

zcl_factory 11 {
	request {
		mac 0 : string
		ep 1 : integer
	}
}

heartbeat 12 {}

]]

-------------------------- skynet agent api---zigbee modle--->skynet agent
proto.s2c = sprotoparser.parse [[
.package {
	type 0 : integer
	session 1 : integer
}

handshake 1 {}

network_state 2 {
	request {
		power 0 : integer
		pid 1 : integer
		channel 2 : integer
		mac 3 : string
		version 4 : string
	}
}

zcl_report 3 {
	request {
		mac 0 : string
		ep 1 : integer
		cluster 2 : integer
		data 3 : string
	}
}

zcl_read 4 {
	request {
		mac 0 : string
		ep 1 : integer
		cluster 2 : integer
		data 3 : string
	}
}
	
dev_left 5 {
	request {
		mac 0 : string
	}
}

dev_online 6 {
	request {
		mac 0 : string
		ep 1 : integer
		deviceId 2 : integer
		incluster 3 : string
		outcluster 4 : string
	}
}

zcl_cmd 7 {
	request {
		mac 0 : string
		ep 1 : integer
		type 2: integer
		cluster 3 : integer
		cmd 4 : integer
		data 5 : string
	}
}

aps_sent 8 {
	request {
		mac 0 : string
		status 1 : integer
		cluster 2 : integer
		cmd 3 : integer
	}
}

rssi_report 9 {
	request {
		mac 0 : string
		rssi 1 : integer
		lqi 2 : integer
	}
}

quit 10 {}
]]

return proto
