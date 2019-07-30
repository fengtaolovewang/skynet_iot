if not _P then
	print "inject error!!"
	return
end

local function getuv(f)
	local u={}
	if not f then
		return u
	end
	local i=1
	while true do
		local name,value =debug.getupvalue(f,i)
		if name ==nil then
			return u
		end
		print(name .. "I=" ..i)
		u[name]={i,value}
		i=i+1
	end
	return u
end

local function uvjoin(f,old_f,old_uv)
	local i=1
	while true do
		local name,value =debug.getupvalue(f,i)
		if name ==nil then
			break
		end
		print(name)	
--		debug.setupvalue(f,i,old_uv[name])
		if old_uv[name] then
	--		debug.upvaluejoin(f,i,old_f,i)
			debug.upvaluejoin(f,i,old_f,old_uv[name][1])
		end
		i=i+1
	end
end

local command = _P.lua.command

local upvs=getuv(command.TEST)
local func=command.TEST

local str="hello"

command.TEST = function(msg)
	local function f()
--		print("11111")
		print("22222")
		print("new" .. msg)
	end
	f()
end

--uvjoin(command.TEST,func,upvs)

print "inject ok!!"
