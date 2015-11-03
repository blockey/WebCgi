/*
*snmpServices.c
*存取硬件参数接口
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../base/base.h"
#include "snmpServices.h"
#include "../card/snmpSetServices.h"
#include "../card/snmpGetServices.h"
#include "../util/adstring.h"
/*
*功能:读取设备参数
*@itemNames :in 要读取的参数项集
*@parameters :out 从设备读出的参数
*return ,if success ,return true,else return false
*/
boolean getDeviceParameters(char *itemNames,char *parameters){
	if(itemNames == NULL){
		webLog("file=%s;func=%s;msg=itemNames is null ;",__FILE__,__FUNCTION__);
		return false;
	}
	
	char itemName[50];				//每个请求参数
	char snmpItemNames[500]="";		//SNMP类请求参数集
	char sysInfoItemNames[500]="";	//系统信息请求参数集
	
	//分解参数中的各类请求		
	int i = 1;
	
	while(getItem(itemNames,itemName,',',i++)){
		
		//获取电口光口告警信息
		if(strstr(itemName,"FTAlerts")){
			//添加到系统信息请求中
			strcat(sysInfoItemNames,itemName);
			strcat(sysInfoItemNames,",");
			
		}
		//板卡统计
		else if(strstr(itemName,"cardStatistics")){
			//添加到系统信息请求中
			strcat(sysInfoItemNames,itemName);
			strcat(sysInfoItemNames,",");
			
		}
		//WEB版本信息
		else if(strstr(itemName,"webVersion")){
			//添加到系统信息请求中
			strcat(sysInfoItemNames,itemName);
			strcat(sysInfoItemNames,",");
			
		}
		//SNMP类请求
		else{
			strcat(snmpItemNames,itemName);
			strcat(snmpItemNames,",");
		}
	}
	
	/*  系统信息请求  */	
	if(strcmp(sysInfoItemNames,"")){
		
		if(!getSysInfoService(sysInfoItemNames,parameters)){
			return false;
		}
	}	
	/*  snmp请求   */
	//if(strcmp(snmpItemNames,"")){
	if(strlen(snmpItemNames) > 1){
		
		if(!getDeviceParametersService(snmpItemNames,parameters)){
			return false;
		}
	}
	return true;
}
/*
*功能:设置设备参数
*@itemNames :in 设置的参数名集
*@itemValues :in 设置的参数值集
*@parameters :out 设置后从设备读出的参数
*return ,if success ,return true,else return false
*/
boolean setDeviceParameters(char *itemNames,char *itemValues,char *parameters){
	
	if(itemNames == NULL ||  itemValues == NULL){
		webLog("file=%s;func=%s;msg=itemNames or itemValues is null ;",__FILE__,__FUNCTION__);
		return false;
	}
	//调用硬件参数设置接口进行设置
	return setDeviceParametersService(itemNames,itemValues,parameters);
}



