/*
*snmpSetMeta.c
*设置硬件参数的扩展工具
*/
#include <string.h>
#include "snmpSetMeta.h"
#include "snmpUtil.h"
#include "../util/adstring.h"
#include "../base/base.h"
#include <stdlib.h>
#include <stdio.h>
/*
*功能:根据机架和设备卡位置获取卡类型
*@devId		:in		机架编号 从0开始
*@cardId	:in		板卡编号 从1开始
*@cardType	:out	板卡类型 输出
*return
*/
void getCardTypeByPosition(int devId,int cardId,char *cardType){
	char cardTypeStr[50];		//板卡类型的字符串形式
	char itemNames[100];		//硬件参数名称
	//设置硬件参数名
	sprintf(itemNames,"readSingleDeviceCardType__devId%d_cardId%d_,",devId,cardId);
	//执行SNMP命令,提取命令结果
	snmpGetFinalData(itemNames,cardTypeStr);
	//提取板卡类型
	getValueByName(cardTypeStr,"singleDeviceCardType",cardType);
	return ;
}

/*
*功能:获取启用参数状态 
*正在启用,返回true,启用完成,返回false
*/
int getRefleshStatus(){
	char statusRet[100];
	char refleshStatus[50];		//板卡类型的字符串形式
	char itemNames[100];		//硬件参数名称
	//设置硬件参数名
	sprintf(itemNames,"refleshStatus,");
	//执行SNMP命令,提取命令结果
	snmpGetFinalData(itemNames,statusRet);
	//提取板卡类型
	getValueByName(statusRet,"refleshing",refleshStatus);
	if(!strcmp(refleshStatus,"true")){
		return true;
	}else{
		return false;
	}
}



