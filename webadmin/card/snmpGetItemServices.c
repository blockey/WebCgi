#include "snmpGetItemUtil.h"
#include "snmpGetItemServices.h"
#include <string.h>
/*
*功能:对字符串进行特定处理,以适合输出到界面
*@ itemName :in
*@ itemType :in
*@ data :in out
*return 转换成功 return true,else return false
*/
void dataConveter(char *itemName,char *itemType ,char *data){
	/*机架信息*/
	if(!strcmp(itemName,"deviceLinkStatus")){
		deviceLinkStatus_DTF(itemName,data);
	}
	/*扩展Trap地址和端口*/
	else if(!strcmp(itemName,"trapServer")){
		trapServer_DTF(itemName,data);
	}
	/*软件升级日期*/
	else if(!strcmp(itemName,"snmpUpdateDate")){
		snmpUpdateDate_DTF(itemName,data);
	}
	/*设备系统时间*/
	else if(!strcmp(itemName,"getDeviceTime")||!strcmp(itemName,"setDeviceTime")){
		getDeviceTime_DTF(itemName,data);
	}
	/*16块卡状态信息*/
	else if(strstr(itemName,"cardStatusInfo") != NULL){
		cardStatusInfo_DTF(itemName,data);
	}
	/*单个卡状态信息--从状态中取*/
	else if(strstr(itemName,"cardStatusCfg") != NULL){
		cardStatusCfg_DTF(itemName,data);
	}
	/*远端单个卡状态信息--从状态中取*/
	else if(strstr(itemName,"remoteCardStatus") != NULL){
		remoteCardStatus_DTF(itemName,data);
	}
	/*级联设备的16块卡的远端描述信息*/
	else if(strstr(itemName,"devicesCardRemote") != NULL){
		devicesCardRemote_DTF(itemName,data);
	}
	/*读取单个设备卡类型*/
	else if(strstr(itemName,"readSingleDeviceCardType") != NULL){
		readSingleDeviceCardType_DTF(itemName,data);
	}
	/*设置单个设备卡类型*/
	else if(strstr(itemName,"saveSingleDeviceCardType") != NULL){
		saveSingleDeviceCardType_DTF(itemName,data);
	}
	/*单个设备卡是否使用*/
	else if(strstr(itemName,"singleDeviceCardUse") != NULL){
		singleDeviceCardUse_DTF(itemName,data);
	}
	/*单个设备卡配置信息*/
	else if(strstr(itemName,"cardConfInfo") != NULL){
		cardConfInfo_DTF(itemName,data);
	}
	/*单个级联设备卡的远端描述信息*/
	else if(strstr(itemName,"singleDeviceCardRemote") != NULL){
		singleDeviceCardRemote_DTF(itemName,data);
	}
	/*单个级联设备卡的远端配置信息*/
	else if(strstr(itemName,"remoteCardConfigInfo") != NULL){
		remoteCardConfigInfo_DTF(itemName,data);
	}
	/*单个级联设备卡的版本信息*/
	else if(strstr(itemName,"singleCardVersion") != NULL){
		singleCardVersion_DTF(itemName,data);
	}
	/*单个级联设备卡的扩展信息*/
	else if(strstr(itemName,"singleDeviceCardExt") != NULL){
		singleDeviceCardExt_DTF(itemName,data);
	}
	/*单个级联设备卡的扩展信息*/
	else if(strstr(itemName,"singleDeviceCardStatusExt") != NULL){
		singleDeviceCardStatusExt_DTF(itemName,data);
	}
	/*单个级联设备卡远端连接状态*/
	else if(strstr(itemName,"singleDeviceCardRmtConnStatus") != NULL){
		singleDeviceCardRemoteConnStatus_DTF(itemName,data);
	}
	/*16个设备卡类型*/
	else if(strstr(itemName,"devicesCardType") != NULL){
		devicesCardType_DTF(itemName,data);
	}
	/*16个设备卡是否使用*/
	else if(strstr(itemName,"devicesCardUse") != NULL){
		devicesCardUse_DTF(itemName,data);
	}
	/*环境参数告警门限配置信息*/
	else if(strstr(itemName,"cardPowerConfig") != NULL){
		cardPowerConfig_DTF(itemName,data);
	}
	/*板卡类型统计信息*/
	else if(strstr(itemName,"statistics_cardTypes") != NULL){
		statistics_cardTypes_DTF(itemName,data);
	}
	/* 获取系统重启状态 */
	else if(strstr(itemName,"refleshStatus") != NULL){
		refleshStatus_DTF(itemName,data);
	}
	
	/* 通用数据转换*/
	else{
		Genal_DTF(itemName,itemType,data);
	}	
}