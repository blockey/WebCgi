/*
*snmpSetItemMeta.c
*硬件参数设置工具扩展
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../base/base.h"
#include "snmpSetServices.h"
#include "snmpGetItemMeta.h"
#include "snmpvar.h"
#include "../util/adstring.h"
/*
*功能:根据itemName和itemValue处理出SingleCardType请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean getSingleCardTypeSetData(char *itemName,char *itemValue,char *saveData){
	char srcData[500];			//执行snmp命令的原始结果
	char itemNames[50];			//硬件参数名称
	unsigned char chData[100];	//无符号十六进行临时数据
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	//获取板卡编号
	int cardNum = getCardNum(itemNames,"cardId");
	//设置相关参数的值
	chData[cardNum-1] = getCardTypeId(itemValue);
	//重新转换成硬件识别的十六进制数据以便下一步保存
	str2hexstr(chData,saveData,16);
	return true;
}

/*
*功能:根据itemName和itemValue处理出SingleCardUse请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean getSingleCardUseSetData(char *itemName,char *itemValue,char *saveData){
	
	char itemNames[50];			//硬件参数名称
	char srcData[500];			//执行snmp命令的原始结果
	unsigned char chData[100];	//无符号十六进行临时数据
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	//获取板卡编号
	int cardNum = getCardNum(itemNames,"cardId");
	//设置相关参数的值
	chData[cardNum-1] = atoi(itemValue);
	//重新转换成硬件识别的十六进制数据以便下一步保存
	str2hexstr(chData,saveData,16);
	return true;
}


/*
*功能:根据itemName和itemValue处理出singleDeviceCardRemote请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean singleDeviceCardRemoteSetData(char *itemName,char *itemValue,char *saveData){
	char itemNames[50];				//硬件参数名称
	char srcData[2500];				//执行snmp命令的原始结果
	unsigned char chData[1000];		//无符号十六进行临时变量
	//unsigned char chValue[1000];	//无符号十六进行临时变量
	CARD_REMOTEINFO remoteInfo[GLOBAL_MAX_CARDNUM];		//板卡远端地址结构数组
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	//将数据内存拷贝到板卡远端地址结构数组空间
	memcpy(&remoteInfo,&chData,40*GLOBAL_MAX_CARDNUM);
	//获取板卡编号
	int cardNum = getCardNum(itemName,"cardId");
	//对要设置的远端地址值进行转换,因为原来是编码字符编码形式"12/45/78/"
	tranRemoteAddr(itemValue);
	//转换为无符号类型 
	unsigned char *remoteAddr = (unsigned char *)itemValue;
	//将要设置的值设置到相应位置
	memcpy(remoteInfo[cardNum-1].ucCardRemote,remoteAddr,40);
	//重新转换成硬件识别的十六进制数据以便下一步保存
	memcpy(chData,&remoteInfo,40*GLOBAL_MAX_CARDNUM);
	str2hexstr(chData,saveData,40*GLOBAL_MAX_CARDNUM);
	return true;
}
/*
*功能:根据itemName和itemValue处理出devicesCardRemote请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean devicesCardRemoteSetData(char *itemName,char *itemValue,char *saveData){
	int i;
	char itemNames[50];			//硬件参数名称
	char srcData[2500];			//执行snmp命令的原始结果
	//char value[40];				//远端地址变量
	char value[256];				//远端地址变量
	unsigned char chData[1000];	//无符号十六进行临时变量
	char tempItemName[50];		//临时硬件参数名
	unsigned char *remoteAddr ;//远端地址 无符号类型指针,用于转换 
	CARD_REMOTEINFO remoteInfo[GLOBAL_MAX_CARDNUM];		//板卡远端地址结构数组
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	//将数据内存拷贝到板卡远端地址结构数组空间
	memcpy(&remoteInfo,&chData,40*GLOBAL_MAX_CARDNUM);
	//对该机架的每一块板卡的远端地址进行设置
	for(i = 0;i< GLOBAL_MAX_CARDNUM -1;i++){
		sprintf(tempItemName,"remote%d",i+1);
		getValueByName(itemValue,tempItemName,value);
		tranRemoteAddr(value);
		//转换为无符号类型 
		remoteAddr = (unsigned char *)value;
		//将要设置的值设置到相应位置
		memcpy(remoteInfo[i].ucCardRemote,remoteAddr,40);
		//strcpy(remoteInfo[i].ucCardRemote,value);
	}
	/* 重新转换成硬件识别的十六进制数据以便下一步保存 */
	memcpy(chData,&remoteInfo,40*GLOBAL_MAX_CARDNUM);
	str2hexstr(chData,saveData,40*GLOBAL_MAX_CARDNUM);
	return true;
}

/*
*功能:根据itemName和itemValue处理出devicesCardType请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean devicesCardTypeSetData(char *itemName,char *itemValue,char *saveData){
	int i;
	char tempItemName[50];			//临时硬件参数名
	char itemNames[50];				//硬件参数名称
	char srcData[500];				//执行snmp命令的原始结果
	char value[40];					//中间变量
	unsigned char chData[500];		//无符号十六进行临时变量
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	/* 对该机架的每一块板卡的类型进行设置 */
	for(i = 0;i< GLOBAL_MAX_CARDNUM -1;i++){
		sprintf(tempItemName,"cardType%d",i+1);
		getValueByName(itemValue,tempItemName,value);
		chData[i] = getCardTypeId(value);
	}
	/* 重新转换成硬件识别的十六进制数据以便下一步保存 */
	str2hexstr(chData,saveData,16);
	return true;
}

/*
*功能:根据itemName和itemValue处理出devicesCardUse请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean devicesCardUseSetData(char *itemName,char *itemValue,char *saveData){
	int i;
	char tempItemName[50];			//临时硬件参数名
	char itemNames[50];				//硬件参数名称
	char srcData[500];				//执行snmp命令的原始结果
	char value[40];					//中间变量
	unsigned char chData[500];		//无符号十六进行临时变量
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	/* 对该机架的每一块板卡的使用状态进行设置 */
	for(i = 0;i< GLOBAL_MAX_CARDNUM -1;i++){
		sprintf(tempItemName,"cardUse%d",i+1);
		getValueByName(itemValue,tempItemName,value);
		chData[i] = atoi(value);
	}
	/* 重新转换成硬件识别的十六进制数据以便下一步保存 */
	str2hexstr(chData,saveData,16);
	return true;
	
}
/*
*功能:根据itemName和itemValue处理出cardPowerConfig请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean cardPowerConfigSetData(char *itemName,char *itemValue,char *saveData){
	char itemNames[50];				//硬件参数名称
	char srcData[500];				//执行snmp命令的原始结果
	char value[40];					//中间变量
	unsigned char chData[500];		//无符号十六进行临时变量
	char chIntValue[5];				//电源电压整数部分
	char chDecValue[5];				//电源电压小数部分
	CARD_CONFIG cardConfig;			//板卡配置信息
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	//将数据内存拷贝到板卡配置信息空间
	memcpy(&cardConfig,chData,16);
	/*电源电压告警门限----上限*/
	getValueByName(itemValue,"voltageTopLimit",value);
	getIntAndDecOfDoubleStr(value,chIntValue,chDecValue);
	cardConfig.ucCardConf[0] = strtol(chIntValue,NULL,16);
	cardConfig.ucCardConf[1] = strtol(chDecValue,NULL,16);
	/*电源电压告警门限----下限*/
	getValueByName(itemValue,"voltageLowerLimit",value);
	getIntAndDecOfDoubleStr(value,chIntValue,chDecValue);
	cardConfig.ucCardConf[2] = strtol(chIntValue,NULL,16);
	cardConfig.ucCardConf[3] = strtol(chDecValue,NULL,16);
	/*环境温度告警门限----上限*/
	getValueByName(itemValue,"temperatureTopLimit",value);
	cardConfig.ucCardConf[4] = atoi(value);
	/*环境温度告警门限----下限*/
	getValueByName(itemValue,"temperatureLowerLimit",value);
	cardConfig.ucCardConf[5] = atoi(value);
	/*环境湿度告警门限----上限*/
	getValueByName(itemValue,"humidityTopLimit",value);
	cardConfig.ucCardConf[6] = atoi(value);
	/*环境湿度告警门限----下限**/		
	getValueByName(itemValue,"humidityLowerLimit",value);
	cardConfig.ucCardConf[7] = atoi(value);
	/*风扇转速告警门限----上限*/
	getValueByName(itemValue,"fanSpeedTopLimit",value);
	cardConfig.ucCardConf[9] = atoi(value)/256;
	cardConfig.ucCardConf[8] = atoi(value)%256;
	/*风扇转速告警门限----下限*/
	getValueByName(itemValue,"fanSpeedLowerLimit",value);
	cardConfig.ucCardConf[11] = atoi(value)/256;
	cardConfig.ucCardConf[10] = atoi(value)%256;
	/* 重新转换成硬件识别的十六进制数据以便下一步保存 */
	memcpy(&chData,&cardConfig,16);
	str2hexstr(chData,saveData,16);
	return true;
}
/*
*功能:根据itemName和itemValue处理出remoteCardConfigInfo请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false
*说明:只有F3和IP113S两种板卡有远端配置信息
*/
boolean remoteCardConfigInfoSetData(char *itemName,char *itemValue,char *saveData){
	char itemNames[50];				//硬件参数名称
	char srcData[2500];				//执行snmp命令的原始结果
	unsigned char chData[1000];		//无符号十六进行临时变量
	char outData[200];				//输出结果
	char binstr[9];					//数据的二进制字符串形式
	char g_cardType[20];			//板卡类型
	char value[30];					//中间变量 
	CARD_CONFIG cardConfig;			//板卡配置信息
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	//将数据内存拷贝到板卡配置信息空间
	memcpy(&cardConfig,&chData,16);
	//获取卡类型
	getCardTypeByPosition(getCardNum(itemName,"devId"),getCardNum(itemName,"cardId"),g_cardType);
	/*   根据不同的板卡类型作不同的处理  */
	if(!strcmp(g_cardType,"F3")){
		
		//自适应
		getValueByName(itemValue,"auto",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[8] |= 0x04;
		}else{
			
			cardConfig.ucCardConf[8] &= ~0x04;
		}
		
		//速率
		getValueByName(itemValue,"speed",value);
		if(!strcmp(value,"100M")){
			cardConfig.ucCardConf[8] |= 0x02;
		}else{
			cardConfig.ucCardConf[8] &= ~0x02;
		}
		
		//双工
		getValueByName(itemValue,"duplex",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[8] |= 0x01;
		}else{
			
			cardConfig.ucCardConf[8] &= ~0x01;
		}
		
		//复位
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[9] |= 0x01;
		}else{
			
			cardConfig.ucCardConf[9] &= ~0x01;
		}
		
		//模式
		//int2binstr(cardConfig.ucCardConf[10],binstr);
		getValueByName(itemValue,"mode",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[10] |= 0x01;
		}else{
			
			cardConfig.ucCardConf[10] &= ~0x01;
		}
	}else if(!strcmp(g_cardType,"IP113S")){
		//int2binstr(cardConfig.ucCardConf[4],binstr);
		
		//网管设置
		cardConfig.ucCardConf[4] |= 0x40;
		//复位 
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[4] |= 0x80;
		}else{
			
			cardConfig.ucCardConf[4] &= ~0x80;
		}
		
		
		//自适应 1 强制 0 
		getValueByName(itemValue,"auto",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[4] &= ~0x20;
			
		}else{
			cardConfig.ucCardConf[4] |= 0x20;
			
		}
		
		//速率
		getValueByName(itemValue,"speed",value);
		if(!strcmp(value,"10M")){
			cardConfig.ucCardConf[4] |= 0x10;
		}else{
			cardConfig.ucCardConf[4] &= ~0x10;
		}
		
		//双工
		getValueByName(itemValue,"duplex",value);
		if(!strcmp(value,"1")){
			
			cardConfig.ucCardConf[4] &= ~0x08;
		}else{
			cardConfig.ucCardConf[4] |= 0x08;
			
		}
		
	}
	//写控制位
	cardConfig.ucCardConf[15] |= 0x02;
	memcpy(chData,&cardConfig,16);
	str2hexstr(chData,saveData,16);		
	return true;
}
/*
*功能:根据itemName和itemValue处理出singleDeviceCardExt请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false
*说明:只有IP113S板卡有近端扩展信息
*/
boolean singleDeviceCardExtSetData(char *itemName,char *itemValue,char *saveData){
	char itemNames[50];				//硬件参数名称
	char srcData[2500];				//执行snmp命令的原始结果
	unsigned char chData[1000];		//无符号十六进行临时变量
	char binstr[9];					//数据的二进制字符串形式
	char g_cardType[20];			//板卡类型
	char value[30];					//中间变量 
	CARD_CONFIG cardConfig;			//板卡配置信息
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	//将数据内存拷贝到板卡配置信息空间
	memcpy(&cardConfig,&chData,16);
	//获取卡类型
	getCardTypeByPosition(getCardNum(itemName,"devId"),getCardNum(itemName,"cardId"),g_cardType);
	//将数据转换成二进制形式
	int2binstr(cardConfig.ucCardConf[3],binstr);
	/*   对各个数据项作分别处理   */
	//lookbackTest	
	getValueByName(itemValue,"lookbackTest",value);
	binstr[0] = atoi(value)+'0';
	//backPressure		
	getValueByName(itemValue,"backPressure",value);
	binstr[2] = atoi(value)+'0';
	//broadcastStorm
	getValueByName(itemValue,"broadcastStorm",value);
	binstr[5] = atoi(value)+'0';
	//handlePause
	getValueByName(itemValue,"handlePause",value);
	binstr[7] = atoi(value)+'0';
	//handlePause
	getValueByName(itemValue,"linkLose",value);
	binstr[1] = atoi(value)+'0';
	//handlePause
	getValueByName(itemValue,"symmetric",value);
	binstr[4] = atoi(value)+'0';
	//handlePause
	getValueByName(itemValue,"mode",value);
	binstr[6] = atoi(value)+'0';	
	//保存数据
	cardConfig.ucCardConf[3] = strtol(binstr,NULL,2);
	cardConfig.ucCardConf[15] = 0x01;  
	memcpy(&chData,&cardConfig,16);
	//转码到输出
	str2hexstr(chData,saveData,16);		
	return true;
}
/*
*功能:根据itemName和itemValue处理出getCardConfInfo请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false
*/
boolean getCardConfInfoSetData(char *itemName,char *itemValue,char *saveData){
	
	char itemNames[50];			//硬件参数名称
	char outData[100];			//输出数据
	char temp[100];				//临时变量
	char data[100] = "";		//临时变量
	char srcData[500];			//执行snmp命令的原始结果
	char g_cardType[20];		//板卡类型
	unsigned char chData[100];	//无符号十六进行临时变量
	char binstr[9];				//数据的二进制字符串形式
	char value[20];				//中间变量 
	CARD_CONFIG cardConfig;		//板卡配置信息
	/* 执行命令提取该命令的原始数据  */
	sprintf(itemNames,"%s,",itemName);
	snmpGetSrcData(itemNames,srcData);
	//获取卡类型
	getCardTypeByPosition(getCardNum(itemName,"devId"),getCardNum(itemName,"cardId"),g_cardType);
	//将十六进制数据转换为一般字符
	hextochar1(srcData,chData);
	//将数据内存拷贝到板卡配置信息空间
	memcpy(&cardConfig,chData,16);
	//根据不同类型的板卡作不同处理
	if(!strcmp(g_cardType,"8993")){
		
		
		//通断 
		getValueByName(itemValue,"connect",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x08;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x08;
		}
		//复位 
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x04;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x04;
		}
		//速率
		getValueByName(itemValue,"speed",value);
		if(!strcmp(value,"10M")){
			cardConfig.ucCardConf[0] |= 0x02;
		}else{
			cardConfig.ucCardConf[0] &= ~0x02;
		}
		//双工
		getValueByName(itemValue,"duplex",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x01;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x01;
		}
		
	}else if(!strcmp(g_cardType,"9301")){
		
		//通断 
		getValueByName(itemValue,"connect",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x08;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x08;
		}
		//速率 只支持100M
		
		//复位 
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x04;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x04;
		}
		
	}else if(!strcmp(g_cardType,"9331")){
		
		//通断 
		getValueByName(itemValue,"connect",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x02;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x02;
		}

		//速率 只支持100M

		//复位 
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x04;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x04;
		}

		//双工
		getValueByName(itemValue,"duplex",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x01;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x01;
		}

	}else if(!strcmp(g_cardType,"6652")){
		int2binstr(cardConfig.ucCardConf[0],binstr);
		//通断 
		getValueByName(itemValue,"connect",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x08;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x08;
		}
		
		//复位
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x04;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x04;
		}
		//自适应卡,不再提供速率和双工设置 ====================== 
		//速率
//		getValueByName(itemValue,"speed",value);
//		if(!strcmp(value,"10M")){
//			cardConfig.ucCardConf[0] |= 0x02;
//		}else{
//			cardConfig.ucCardConf[0] &= ~0x02;
//		}
//		//双工
//		getValueByName(itemValue,"duplex",value);
//		if(!strcmp(value,"1")){
//			cardConfig.ucCardConf[0] |= 0x01;
//		}else{
//			
//			cardConfig.ucCardConf[0] &= ~0x01;
//		}

	}else if(!strcmp(g_cardType,"IP113")){
		
		//复位 
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x04;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x04;
		}
		//速率
		getValueByName(itemValue,"speed",value);
		if(!strcmp(value,"10M")){
			cardConfig.ucCardConf[0] |= 0x02;
		}else{
			cardConfig.ucCardConf[0] &= ~0x02;
		}
		//双工
		getValueByName(itemValue,"duplex",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x01;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x01;
		}
		
	}else if(!strcmp(g_cardType,"1000M")){
		//网管设置
		cardConfig.ucCardConf[0] |= 0x80;
		//自适应
		getValueByName(itemValue,"auto",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x40;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x40;
		}
		//复位 
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x04;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x04;
		}
		
		//速率
		if(getValueByName(itemValue,"speed",value)){
			if(!strcmp(value,"1000M")){
				cardConfig.ucCardConf[0] |= 0x20;
			}else{
				cardConfig.ucCardConf[0] &= ~0x20;
				if(!strcmp(value,"100M")){
					cardConfig.ucCardConf[0] |= 0x10;
				}else {
					cardConfig.ucCardConf[0] &= ~0x10;
				}
			}
		}
		//双工
		if(getValueByName(itemValue,"duplex",value)){
			
			if(!strcmp(value,"1")){
				cardConfig.ucCardConf[0] |= 0x08;
			}else{
			
				cardConfig.ucCardConf[0] &= ~0x08;
			}
		}
		
	}else if(!strcmp(g_cardType,"F3")){
		
		//自适应
		getValueByName(itemValue,"auto",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[1] |= 0x04;
		}else{
			
			cardConfig.ucCardConf[1] &= ~0x04;
		}
		
		//速率
		if(getValueByName(itemValue,"speed",value)){
			if(!strcmp(value,"100M")){
				cardConfig.ucCardConf[1] |= 0x02;
			}else{
				cardConfig.ucCardConf[1] &= ~0x02;
			}
		}
		//双工
		if(getValueByName(itemValue,"duplex",value)){
			if(!strcmp(value,"1")){
				cardConfig.ucCardConf[1] |= 0x01;
			}else{
			
				cardConfig.ucCardConf[1] &= ~0x01;
			}	
		}
			
		//复位 
		int2binstr(cardConfig.ucCardConf[6],binstr);
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[6] |= 0x01;
		}else{
			
			cardConfig.ucCardConf[6] &= ~0x01;
		}
		//模式
		getValueByName(itemValue,"mode",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[7] |= 0x01;
		}else{
			
			cardConfig.ucCardConf[7] &= ~0x01;
		}
		//////////////////////////////////////////////////////////////////
		//下行
		//下行带宽M
		getValueByName(itemValue,"downRunRateM",value);
		setRunRateM(atoi(value),&(cardConfig.ucCardConf[2]),&(cardConfig.ucCardConf[3]));
		//init 
		cardConfig.ucCardConf[2] |= 0x80;
		cardConfig.ucCardConf[2] |= 0x40;
		
		//下行带宽K
		getValueByName(itemValue,"downRunRateK",value);
		setRunRateK(atoi(value),&(cardConfig.ucCardConf[3]));
		////////////////////////////////////////////////////////
		//上行
		//上行带宽M
		getValueByName(itemValue,"upRunRateM",value);
		setRunRateM(atoi(value),&(cardConfig.ucCardConf[4]),&(cardConfig.ucCardConf[5]));
		//init 
		cardConfig.ucCardConf[4] |= 0x80;	
		cardConfig.ucCardConf[4] &= ~0x40;
		//上行带宽K
		getValueByName(itemValue,"upRunRateK",value);
		setRunRateK(atoi(value),&(cardConfig.ucCardConf[5]));
	
	}else if(!strcmp(g_cardType,"IP113S")){
		//网管设置
		cardConfig.ucCardConf[0] |= 0x40;
		//本地有效
		cardConfig.ucCardConf[0] |= 0x01;
		int upSample;
		int downSample;
		//复位 
		getValueByName(itemValue,"reset",value);
		if(!strcmp(value,"1")){
			cardConfig.ucCardConf[0] |= 0x80;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x80;
		}
		//速率
		if(getValueByName(itemValue,"speed",value)){
			if(!strcmp(value,"10M")){
				cardConfig.ucCardConf[0] |= 0x10;
			}else{
				cardConfig.ucCardConf[0] &= ~0x10;
			}
		}
		//双工
		if(getValueByName(itemValue,"duplex",value)){
			if(!strcmp(value,"0")){
				cardConfig.ucCardConf[0] &= ~0x08;
			}else{
			
				cardConfig.ucCardConf[0] |= 0x08;
			}
		}
		//自适应
		getValueByName(itemValue,"auto",value);
		if(!strcmp(value,"0")){
			cardConfig.ucCardConf[0] |= 0x20;
		}else{
			
			cardConfig.ucCardConf[0] &= ~0x20;
		}
		//上行粒度
		getValueByName(itemValue,"upRunSample",value);
		if(!strcmp(value,"32")){
			cardConfig.ucCardConf[0] |= 0x04;
			upSample = 32;
		}else{
			cardConfig.ucCardConf[0] &= ~0x04;
			upSample = 512;
		}				
		//下行粒度
		getValueByName(itemValue,"downRunSample",value);
		if(!strcmp(value,"32")){
			cardConfig.ucCardConf[0] |= 0x02;
			downSample = 32;
		}else{
			cardConfig.ucCardConf[0] &= ~0x02;
			downSample = 512;
		}			

		////////////////////////////////////////////////////
		int rateM ;
		int rateK;
		int totalRate ;
		//上行带宽
		getValueByName(itemValue,"upRunRateM",value);
		rateM = atoi(value);
		getValueByName(itemValue,"upRunRateK",value);
		rateK = atoi(value);
		totalRate = rateM * 1024 + rateK *32;
		cardConfig.ucCardConf[1] = totalRate /upSample;
		//下行带宽
		getValueByName(itemValue,"downRunRateM",value);
		rateM = atoi(value);
		getValueByName(itemValue,"downRunRateK",value);
		rateK = atoi(value);
		totalRate = rateM * 1024 + rateK *32;
		cardConfig.ucCardConf[2] = totalRate /downSample;
	
	}

	cardConfig.ucCardConf[15] |= 0x01;
	memcpy(chData,&cardConfig,16);
	str2hexstr(chData,saveData,16);			
	return true;
}
/*
*功能:根据itemName和itemValue处理出getTrapServer请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false
*/
boolean getTrapServerSetData(char *itemName,char *itemValue,char *saveData){
	int i ;
	NETWORK_TRAP_ADDR addr;			//Trap服务器信息
	char value[50];					//中间变量
	char subItemName[20];			//子参数名称
	//设置IP
	for(i = 0;i<4;i++){
		sprintf(subItemName,"trapIp%d",i+1);
		getValueByName(itemValue,subItemName,value);
		addr.ulIpAddress[i]=ip2long(value);
	}		
	//设置端口
	for(i = 0;i<4;i++){
		sprintf(subItemName,"trapPort%d",i+1);
		getValueByName(itemValue,subItemName,value);
		addr.ulPort[i]= atol(value);
	}	
	//设置是否有效
	for(i = 0;i<4;i++){
		sprintf(subItemName,"isValid%d",i+1);
		getValueByName(itemValue,subItemName,value);
		addr.ucIsFlag[i]= value[0]-'0';
	}		
	//将数据结构转换为十六进制
	trap2hexstr(&addr,saveData);
	return true;
}

/*
*功能:Trap地址和端口进行处理,获取16进制字符串
*@ addr			:in
*@ itemValue	:out
* return 
*/
void trap2hexstr(NETWORK_TRAP_ADDR *addr,char *itemValue){
		int i;
		unsigned char midValue[50];		//十六进制中间变量
		char hexstr[50];				//十六进制字符
		//输出数据添加""号
		strcpy(itemValue,"\"");
		//IP地址
		for(i = 0; i<4;i++){
			memcpy(midValue,&addr->ulIpAddress[i],sizeof(unsigned long));
			midValue[sizeof(unsigned long)] = '\0';
			str2hexstr(midValue,hexstr,sizeof(unsigned long));
			trim(hexstr);
			strcat(itemValue,hexstr);
			strcat(itemValue," ");
		}
		//端口
		for(i = 0; i<4;i++){
			memcpy(midValue,&addr->ulPort[i],sizeof(unsigned long));
			midValue[sizeof(unsigned long)] = '\0';
			str2hexstr(midValue,hexstr,sizeof(unsigned long));
			trim(hexstr);
			strcat(itemValue,hexstr);
			strcat(itemValue," ");
		}
		//是否有效
		for(i = 0; i<4;i++){
			memcpy(midValue,&addr->ucIsFlag[i],sizeof(unsigned char));
			midValue[sizeof(unsigned char)] = '\0';
			str2hexstr(midValue,hexstr,sizeof(unsigned char));
			trim(hexstr);
			strcat(itemValue,hexstr);
			strcat(itemValue," ");
		}
		strcat(itemValue,"\"");

}
