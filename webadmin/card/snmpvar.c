/*
*snmpvar.c
*硬件参数相关变量,常量
*/
#include <string.h>
#include "snmpvar.h"
/*
*功能:获取MIB变量标识符
*@ itemName :in		硬件参数名称
*@ indents	:out	硬件参数标识符
* return 找到标识符,return true,else return false
*/
boolean getItemIndent(char *itemName,char *indents){
	
	/*主机架IP地址*/
	if(!strcmp(itemName,"netIpAddress")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.2.1");
		return true ;
	}
	/*子网掩码*/
	if(!strcmp(itemName,"netSubMask")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.2.2");
		return true ;
	}
	/*网关*/
	if(!strcmp(itemName,"netGetway")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.2.3");
		return true ;
	}
	/*Trap地址*/
	if(!strcmp(itemName,"netTrapIP")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.2.4");
		return true ;
	}
	/*Get共同体名*/
	if(!strcmp(itemName,"getCommunity")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.1.3");
		return true ;
	}
	/*Set共同体名*/
	if(!strcmp(itemName,"setCommunity")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.1.4");
		return true ;
	}
	/*扩展Trap地址和端口*/
	if(!strcmp(itemName,"trapServer")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.3");
		return true ;
	}
	/*软件版本*/
	if(!strcmp(itemName,"snmpVersion")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.1.1");
		return true ;
	}
	/*软件升级日期*/
	if(!strcmp(itemName,"snmpUpdateDate")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.1.2");
		return true ;
	}
	/*获取设备系统时间*/
	if(!strcmp(itemName,"getDeviceTime")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.7");
		return true ;
	}
	/*设置设备系统时间*/
	if(!strcmp(itemName,"setDeviceTime")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.6");
		return true ;
	}
	/*16块子机架设备连接状态*/
	if(!strcmp(itemName,"deviceLinkStatus")){
		strcpy(indents,"1.3.6.1.4.1.16985.1.5.1.5");
		return true ;
	}
	/*16块卡状态信息*/
	if(strstr(itemName,"cardStatusInfo") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.4",getCardNum(itemName,"devId"));
		//strcpy(indents,"1.3.6.1.4.1.16985.1.5.2.1.4");//temp
		strcpy(indents,buffer);
		return true ;
	}
	/*16块卡类型信息*/
	if(strstr(itemName,"statistics_cardTypes") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.4",getCardNum(itemName,"devId"));
		//strcpy(indents,"1.3.6.1.4.1.16985.1.5.2.1.4");//temp
		strcpy(indents,buffer);
		return true ;
	}
	/*IP113S扩展配置信息*/
	if(strstr(itemName,"singleDeviceCardStatusExt") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.4",getCardNum(itemName,"devId"));
		//strcpy(indents,"1.3.6.1.4.1.16985.1.5.2.1.4");//temp
		strcpy(indents,buffer);
		return true ;
	}
	/*级联设备的16块卡的远端描述信息*/
	if(strstr(itemName,"devicesCardRemote") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.3",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*级联设备的16块卡的远端描述信息*/
	if(strstr(itemName,"singleDeviceCardRemote") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.3",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*设置读取单个设备卡类型*/
	if(strstr(itemName,"saveSingleDeviceCardType") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.1",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		
		return true ;
	}
	/*读取读取单个设备卡类型*/
	if(strstr(itemName,"readSingleDeviceCardType") != NULL){
		char buffer[40];
		//从状态信息中获取
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.4",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		
		return true ;
	}
	/*单个设备卡是否使用*/
	if(strstr(itemName,"singleDeviceCardUse") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.2",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*单个设备卡是否有远端 */
	if(strstr(itemName,"singleDeviceCardRmtConnStatus") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.4",getCardNum(itemName,"devId"));
		//strcpy(indents,"1.3.6.1.4.1.16985.1.5.2.1.4");//temp
		strcpy(indents,buffer);
		return true ;
	}
	/*设置单个设备卡配置信息(近端) ----从配置 */
	if(strstr(itemName,"cardConfInfo") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.7.%d",getCardNum(itemName,"devId"),getCardNum(itemName,"cardId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*读取单个设备卡配置信息(近端) ----从状态*/
	if(strstr(itemName,"cardStatusCfg") != NULL){
		char buffer[40];
		//从状态信息中获取
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.4",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*读取单个设备卡配置信息(远端) ----从状态*/
	if(strstr(itemName,"remoteCardStatus") != NULL){
		char buffer[40];
		//从状态信息中获取
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.4",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*单个设备卡配置信息*/
	if(strstr(itemName,"remoteCardConfigInfo") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.7.%d",getCardNum(itemName,"devId"),getCardNum(itemName,"cardId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*单个设备卡版本信息*/
	if(strstr(itemName,"singleCardVersion") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.4",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*单个设备卡扩展信息*/
	if(strstr(itemName,"singleDeviceCardExt") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.7.%d",getCardNum(itemName,"devId"),getCardNum(itemName,"cardId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*16块设备卡类型*/
	if(strstr(itemName,"devicesCardType") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.1",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		
		return true ;
	}
	/*16块设备卡是否使用*/
	if(strstr(itemName,"devicesCardUse") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.2",getCardNum(itemName,"devId"));
		strcpy(indents,buffer);
		return true ;
	}
	/*环境参数告警门限配置信息*/
	if(strstr(itemName,"cardPowerConfig") != NULL){
		char buffer[40];
		sprintf(buffer,"1.3.6.1.4.1.16985.1.5.2.%d.7.16",getCardNum(itemName,"devId"));
		//strcpy(indents,"1.3.6.1.4.1.16985.1.5.2.1.4");//temp
		strcpy(indents,buffer);
		return true ;
	}
	/* 设置snmp代理配置参数 */
	if(strstr(itemName,"refleshAgentCfg") != NULL){
		sprintf(indents,"1.3.6.1.4.1.16985.1.5.1.8");
		return true ;
	}
	/* 获取系统重启状态 */
	if(strstr(itemName,"refleshStatus") != NULL){
		sprintf(indents,"1.3.6.1.4.1.16985.1.5.1.8");
		return true ;
	}
	
	webLog("file=%s;func=%s;msg=there is no MIB id to the itemName %s;",__FILE__,__FUNCTION__,itemName);
	return false;
}

/*
*功能:硬件参数类型
*@ typeName :in		硬件参数名称
*@ itemType :out	硬件参数类型
* return 找到变量类型,return true,else return false
*/
boolean getItemtype(char *itemName,char *itemType){
		/*主机架IP地址*/
	if(!strcmp(itemName,"netIpAddress")){
		strcpy(itemType,"IpAddress");
		return true ;
	}
	/*子网掩码*/
	if(!strcmp(itemName,"netSubMask")){
		strcpy(itemType,"IpAddress");
		return true ;
	}
	/*网关*/
	if(!strcmp(itemName,"netGetway")){
		strcpy(itemType,"IpAddress");
		return true ;
	}
	/*Trap地址*/
	if(!strcmp(itemName,"netTrapIP")){
		strcpy(itemType,"IpAddress");
		return true ;
	}
	/*Get共同体名*/
	if(!strcmp(itemName,"getCommunity")){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*Set共同体名*/
	if(!strcmp(itemName,"setCommunity")){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*软件版本*/
	if(!strcmp(itemName,"snmpVersion")){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*软件升级日期*/
	if(!strcmp(itemName,"snmpUpdateDate")){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*获取设备系统时间*/
	if(!strcmp(itemName,"getDeviceTime")){
		strcpy(itemType,"STRING");
		return true ;
	}
	/*设置设备系统时间*/
	if(!strcmp(itemName,"setDeviceTime")){
		strcpy(itemType,"STRING");
		return true ;
	}
	/*扩展Trap地址和端口*/
	if(!strcmp(itemName,"trapServer")){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*16块子机架设备连接状态*/
	if(!strcmp(itemName,"deviceLinkStatus")){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*16块卡状态信息*/
	if(strstr(itemName,"cardStatusInfo") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*IP113S扩展配置信息*/
	if(strstr(itemName,"singleDeviceCardStatusExt") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*单个设备卡是否连接远端 */
	if(strstr(itemName,"singleDeviceCardRmtConnStatus") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备的16块卡的远端描述信息*/
	if(strstr(itemName,"devicesCardRemote") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备的16块卡的类型信息*/
	if(strstr(itemName,"saveSingleDeviceCardType") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备的16块卡的类型信息*/
	if(strstr(itemName,"readSingleDeviceCardType") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备的16块卡的类型信息*/
	if(strstr(itemName,"singleDeviceCardUse") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*读取级联设备板卡的其他属性信息--从配置*/
	if(strstr(itemName,"cardConfInfo") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*读取级联设备板卡的其他属性信息--近端--从状态*/
	if(strstr(itemName,"cardStatusCfg") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*读取级联设备板卡的其他属性信息--远端--从状态*/
	if(strstr(itemName,"remoteCardStatus") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备板卡的远端信息*/
	if(strstr(itemName,"singleDeviceCardRemote") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备板卡的远端配置信息*/
	if(strstr(itemName,"remoteCardConfigInfo") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备板卡的近端扩展配置信息*/
	if(strstr(itemName,"singleDeviceCardExt") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备16块板卡的远端地址*/
	if(strstr(itemName,"g_DevicesCardRemote") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备电源配置*/
	if(strstr(itemName,"cardPowerConfig") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备16块板卡的类型*/
	if(strstr(itemName,"devicesCardType") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/*级联设备16块板卡的使用状态*/
	if(strstr(itemName,"devicesCardUse") != NULL){
		strcpy(itemType,"Hex-STRING");
		return true ;
	}
	/* 设置snmp代理配置参数 */
	if(strstr(itemName,"refleshAgentCfg") != NULL){
		strcpy(itemType,"INTEGER");
		return true ;
	}
	/* 获取系统重启状态 */
	if(strstr(itemName,"refleshStatus") != NULL){
		strcpy(itemType,"INTEGER");
		return true ;
	}
	webLog("file=%s;func=%s;msg=can't find the data type for itemName %s;",__FILE__,__FUNCTION__,itemName);
	return false;
}
/*
*功能:获取硬件参数类型标识符
*@ itemType		:in		硬件参数类型
*@ typeIndents	:out	硬件参数类型标识符
* return 找到标识符,return true,else return false
*/
boolean getItemtypeIndents(char *itemType,char *typeIndents){
	if(!strcmp(itemType,"INTEGER")){
		strcpy(typeIndents,"i");
		return true;
	}
	if(!strcmp(itemType,"unsigned INTEGER")){
		strcpy(typeIndents,"u");
		return true;
	}
	if(!strcmp(itemType,"TIMETICKS")){
		strcpy(typeIndents,"t");
		return true;
	}
	if(!strcmp(itemType,"IpAddress")){
		strcpy(typeIndents,"a");
		return true;
	}
	if(!strcmp(itemType,"OBJID")){
		strcpy(typeIndents,"o");
		return true;
	}
	if(!strcmp(itemType,"STRING")){
		strcpy(typeIndents,"s");
		return true;
	}
	if(!strcmp(itemType,"Hex-STRING")){
		strcpy(typeIndents,"x");
		return true;
	}
	if(!strcmp(itemType,"DECIMAL STRING")){
		strcpy(typeIndents,"d");
		return true;
	}
	if(!strcmp(itemType,"BITS")){
		strcpy(typeIndents,"b");
		return true;
	}
	if(!strcmp(itemType,"unsigned int64")){
		strcpy(typeIndents,"U");
		return true;
	}
	if(!strcmp(itemType,"signed int64")){
		strcpy(typeIndents,"I");
		return true;
	}
	if(!strcmp(itemType,"float")){
		strcpy(typeIndents,"F");
		return true;
	}
	if(!strcmp(itemType,"double")){
		strcpy(typeIndents,"D");
		return true;
	}
	webLog("file=%s;func=%s;msg=can't get the MIB id of data type for itmeType %s;",__FILE__,__FUNCTION__,itemType);
	return false;
}
/*
*功能:根据类型名获取该类型标识
*@cardType :in	板卡类型
*return 该类型标识
*/
int getCardTypeId(char *cardType){
	if(!strcmp(cardType,"8993")){
		return BD_CARD_TYPE_B;
	}
	if(!strcmp(cardType,"9301")){
		return BD_CARD_TYPE_C;
	}
	if(!strcmp(cardType,"9331")){
		return BD_CARD_TYPE_F;
	}
	if(!strcmp(cardType,"6652")){
		return BD_CARD_TYPE_G;
	}
	if(!strcmp(cardType,"IP113")){
		return BD_CARD_TYPE_H;
	}
	if(!strcmp(cardType,"1000M")){
		return BD_CARD_TYPE_I;
	}
	if(!strcmp(cardType,"F3")){
		return BD_CARD_TYPE_J;
	}
	if(!strcmp(cardType,"IP113S")){
		return BD_CARD_TYPE_K;
	}
	webLog("file=%s;func=%s;msg=can't the typeid for the card type %s;",__FILE__,__FUNCTION__,cardType);
	return -1;
}
