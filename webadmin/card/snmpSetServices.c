/*
*snmpSetServices.c
*设置硬件参数服务入口
*/
#include "snmpSetServices.h"
#include "../base/base.h"
#include "snmpSetUtil.h"
#include <stdlib.h>


/*
*功能:设置设备参数
*@ itemNames	:in	硬件参数名
*@ itemValues	:in	要设置的硬件参数值
*@ parameters	:out设置后的硬件参数值
*if success return true, else return false;
*/
boolean setDeviceParametersService(char *itemNames,char *itemValues,char *parameters){
	
	if(itemNames == NULL || itemValues == NULL ){
		webLog("file=%s;func=%s;msg=itemNames or itemValues is null ;",__FILE__,__FUNCTION__);
		return false;
	}
	//调用硬件参数设置工具进行处理
	return setDeviceParametersUtil(itemNames,itemValues,parameters);
}