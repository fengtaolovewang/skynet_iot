/*创建一个 zigbee 设备模版
coap_cb:表示coap协议的 接受包
id：表示设备ID，唯一识别设备
time:表示时间

function coap_recive(fd,code,token,coap_obj,dev)
*/

zigbee.new_zigbee(coap_recive,id,time)



//-------------------------------------------
--function cb_fun(code,fd,token,pdu_code,pdu,arg)

//------------------------------------------
lua_coap:coap_send(pdu,fd,token,code,cb_fun,arg)
lua_coap:coap_post(pdu,fd,token,code,cb_fun,arg)
lua_coap:coap_replay(pdu,fd,token,code,cb_fun,arg)
//--------------------------------------------------

//ZCL api
zigbee_zcl:set(ep,cluster,cmd_type,cmd,mencode,def_rsp_flag,payload)

ep ,cluster,cmd_type,cmd,payload= zigbee_zcl:get()
//--------------------------------------------------------
dev  api 设备入口

function zigbee_ack_fun(code,rssi,dev,msg,arg)
	
end
//异步
zigbee_send(dev,zcl_msg,zigbee_ack_fun,arg)
//同步
zigbee_post(dev,zcl_msg,zigbee_ack_fun,arg)
//异步
zigbee_send_fifo(dev,zigbee_ack_fun,arg)
//同步
zigbee_post_fifo(dev,zigbee_ack_fun,arg)

----------------------------------------------------
zigbee_recive(dev_obj,mac,rssi,data)
coap_recive(fd,token,pdu)
dev_notify(dev_obj,mac,code)
//-----------------------------------------------------
