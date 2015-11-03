#include "../base/base.h"
#include "snmpGetServices.h"
#include "snmpUtil.h"

/*
*功能:读取设备经处理,符合返回到页面的参数
*@itemNames :in 要读取的参数项集
*@parameters :out 从设备读出的参数
*return ,if success ,return true,else return false
*/
boolean getDeviceParametersService(char *itemNames,char *parameters){
	
	if(!snmpGetFinalData(itemNames,parameters)){
		return false;
	}
	return true;
	
}
/*
*功能:读取设备经处理,符合返回到页面的参数
*@itemNames :in 要读取的参数项集
*@parameters :out 从设备读出的参数
*return ,if success ,return true,else return false
*/
boolean getSysInfoService(char *itemNames,char *parameters){
	char itemName[50];
	int i = 1;
	while(getItem(itemNames,itemName,',',i++)){
		//获取电口光口告警信息不用执行snmp命令
		if(strstr(itemName,"FTAlerts")){
			if(!getFTAlertData(itemName,parameters)){
				return false;
			}
		}
		//板卡统计
		else if(strstr(itemName,"cardStatistics")){
			if(!getCardStatisticsData(itemName,parameters)){
				return false;
			}
			
		}
		//WEB版本信息
		else if(strstr(itemName,"webVersion")){
			if(!getWebVersionData(itemName,parameters)){
				return false;
			}
			
		}
	}
	return true;
}
/*
*功能:读取设备经处理,返回从设备读出的原始参数
*@itemNames :in 要读取的参数项集
*@parameters :out 从设备读出的原始参数
*return ,if success ,return true,else return false
*/
boolean getDeviceSrcParameterService(char *itemNames,char *srcParameters){
	return snmpGetSrcData(itemNames,srcParameters);
}