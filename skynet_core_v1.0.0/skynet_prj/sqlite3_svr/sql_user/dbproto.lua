local sprotoparser = require "sprotoparser"

local proto = {}

------------------------------------ skynet agent--->db client
proto.c2s = sprotoparser.parse [[
.package {
	type 0 : integer
	session 1 : integer
}

db_creat 1 {
	request {
		sql 0 : string
	}
}

db_insert 2 {
		request {
		name 0 : string
		num 1 : integer
		data 2 : string
	}
}

db_get 3 {
		request {
		name 0 : string
		num 1 : integer
		sql 2 : string
	}       
		response {
                result 0  : string
        }

	
}

db_del 4 {
		request {
			sql 0 : string
	}
}

db_updata 5 {
	request {
		sql 0 : string
	}
}

heartbeat 6 {}

]]

-------------------------- skynet agent api---db modle--->skynet agent
proto.s2c = sprotoparser.parse [[
.package {
	type 0 : integer
	session 1 : integer
}

handshake 1 {}

quit 2 {}
]]

return proto
