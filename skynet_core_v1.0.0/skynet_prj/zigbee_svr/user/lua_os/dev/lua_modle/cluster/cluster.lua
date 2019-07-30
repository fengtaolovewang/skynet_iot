-- cluster list 

local cluster = {}
cluster[0x0000]={name="basic.lua"}

cluster[0x0006]={name="onoff.lua"}

cluster[0x0008]={name="levelcontrol.lua"}
cluster[0x0020]={name="pollcontrol.lua"}
cluster[0x0402]={name="temperature.lua"}
cluster[0x0405]={name="humidity.lua"}
cluster[0x0300]={name="color.lua"}
cluster[0x0500]={name="iaszone.lua"}
cluster[0x0502]={name="warningdevice.lua"}


cluster.load=function(cluster_id)
	local c=cluster[cluster_id]
	if(c) then
		return dofile(c.name)
	end
end

return cluster