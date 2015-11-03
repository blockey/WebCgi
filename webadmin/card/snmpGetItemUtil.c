/*
*snmpGetItemUtil.c
*硬件参数转换工具
*/
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include "../base/base.h"
#include "../util/adstring.h"
#include "snmpGetItemUtil.h"
#include "snmpGetItemMeta.h"
#include "snmpvar.h"
///////////////////////////////////
//全局变量
//卡类型
char g_cardType[10];
/*
*功能:对ucDeviceLinkStatus数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void deviceLinkStatus_DTF(char *itemName,char *data){
	char outData[300] = "";		//输出数据
	//设置结果
	sprintf(outData,"%s=%s;",itemName,data);
	//写到返回数据中
	strcpy(data,outData);
}
/*
*功能: 获取系统重启状态
*@ itemName :in
*@ data :in out
*return 
*/
void refleshStatus_DTF(char *itemName,char *data){
	char outData[50] = "";		//输出数据
	//设置结果
	if(data[0] == '1'){//恢复状态正在进行中 
		sprintf(outData,"refleshing=true;",itemName,data);
	}else{
		sprintf(outData,"refleshing=false;",itemName,data);
	}
	//写到返回数据中
	strcpy(data,outData);
}

/*
*功能:对trapServer类数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void trapServer_DTF(char *itemName,char *data){
	char tempstr[70];			//临时数据
	char outData[300] = "";		//输出结果
	NETWORK_TRAP_ADDR addr;		//Trap服务器
	int i ;
	struct sockaddr_in in;		//网络地址结构
	//将十六进行数据转换成普通字符
	hextochar(data);
	memcpy(&addr,data,36);	
	/*TRAP服务器IP地址*/
	for(i = 0;i<4; i++){
		in.sin_addr.s_addr=addr.ulIpAddress[i];
		sprintf(tempstr,"%ld",addr.ulIpAddress[i]);
		sprintf(tempstr,"trapIp%d=%s;",i+1,inet_ntoa(in.sin_addr));
		strcat(outData,tempstr);		
	}
	/*TRAP服务器端口*/
	for(i = 0 ;i<4 ;i++){
		sprintf(tempstr,"trapPort%d=%ld;",i+1,addr.ulPort[i]);
		strcat(outData,tempstr);	
	}
	/*该记录是否有效标记*/
	for(i = 0 ;i<4 ;i++){
		addr.ucIsFlag[i]=addr.ucIsFlag[i]+'0';
		sprintf(tempstr,"isValid%d=%c;",i+1,addr.ucIsFlag[i]);
		strcat(outData,tempstr);	
	}
	strcpy(data,outData);
}
/*
*功能:对snmpUpdateDate数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void snmpUpdateDate_DTF(char *itemName,char *data){
	char tempstr[70];			//临时变量 
	char outData[300] = "";		//输出结果
	//将十六进制数据转换成普通字符串
	hextochar(data);
	//将数据转换成标准时间字符
	dtFormat(data);
	//设置输出
	sprintf(tempstr,"%s=%s;",itemName,data);
	strcat(outData,tempstr);
	strcpy(data,outData);
}
/*
*功能:对getDeviceTime数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void getDeviceTime_DTF(char *itemName,char *data){
	char tempstr[70];			//临时变量 
	char outData[300] = "";		//输出结果
	//将数据转换成标准时间字符
	dtFormat(data);
	//设置输出
	sprintf(tempstr,"%s=%s;",itemName,data);
	strcat(outData,tempstr);
	strcpy(data,outData);
}
/*
*功能:对readSingleDeviceCardType数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void readSingleDeviceCardType_DTF(char *itemName,char *data){
	char tempstr[70];			//临时变量 
	int cardId ;				//卡编号
	unsigned char chData[100];	//无符号字符串
	CARD_STATUS status[GLOBAL_MAX_CARDNUM];	//板卡状态
	//将十六进制字符转换成普通字符串
	hextochar(data);
	//设置板卡状态数据结构值
	int i,j;
	int d = 0;
	for(i = 0;i< 16;i++){
		for(j = 0;j< 5;j++){
			status[i].ucCardVersion[j]= data[d++];
		}
		
		for(j = 0;j< 16;j++){
			status[i].ucCardstatus[j]= data[d++];
		}
		//对齐
		d = d+3;
	}
	//获取卡编号
	cardId = getCardNum(itemName,"cardId");
	//获取板卡类型描述
	getCardTypeByCategory(status[cardId-1].ucCardVersion[0],tempstr);
	sprintf(data,"singleDeviceCardType=%s;",tempstr);
	//设置全局变量
	strcpy(g_cardType,tempstr);
}
/*
*功能:对saveSingleDeviceCardType数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void saveSingleDeviceCardType_DTF(char *itemName,char *data){
	char tempstr[70];			//临时变量 
	int cardId ;				//卡编号
	unsigned char chData[100];	//无符号字符串
	//获取卡编号
	cardId = getCardNum(itemName,"cardId");
	//将十六进制数据转换成无符号字符串
	hextochar1(data,chData);
	//获取板卡类型描述
	getCardTypeByCategory(chData[cardId-1],tempstr);
	sprintf(data,"singleDeviceCardType=%s;",tempstr);
	//设置全局变量
	strcpy(g_cardType,tempstr);
}

/*
*功能:对devicesCardType数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void devicesCardType_DTF(char *itemName,char *data){
	char tempstr[70];			//临时变量
	char outData[300];			//输出数据
	int cardId ;				//板卡编号

	unsigned char chData[150];  //无符号字符串
	//将十六进制数据转换成无符号字符串
	hextochar1(data,chData);
	
	int i;
	//设置返回数据
	strcpy(data,"cardType=");
	for(i = 0;i< GLOBAL_MAX_CARDNUM-1 ;i++){
		//获取板卡类型描述
		getCardTypeByCategory(chData[i],tempstr);
		sprintf(outData,"%s,",tempstr);
		strcat(data,outData);
	}
	strcat(data,";");
	
}
/*
*功能:对cardPowerConfig数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void cardPowerConfig_DTF(char *itemName,char *data){
	
	char outData[200];		//输出数据
	int iValue;				//int 中间变量
	char temp[50];			//临时变量
	CARD_CONFIG cardConfig;	//板卡配置数据结构
	//将十六进制字符转换成一般字符
	hextochar(data);
	//将转换后的数据内在拷贝到板卡配置数据结构空间
	memcpy(&cardConfig,data,16);
	strcpy(data,"");
	/*电源电压告警门限----上限*/
	//小数部分
	sprintf(temp,"%x",cardConfig.ucCardConf[1]);
	iValue = atoi(temp);
	if(iValue <10){
		sprintf(temp,"0%x",cardConfig.ucCardConf[1]);
	}
	sprintf(outData,"voltageTopLimit=%X.%s;",cardConfig.ucCardConf[0],temp);
	strcat(data,outData);
	/*电源电压告警门限----下限*/
	sprintf(temp,"%x",cardConfig.ucCardConf[3]);
	iValue = atoi(temp);
	if(iValue <10){
		sprintf(temp,"0%x",cardConfig.ucCardConf[3]);
	}
	sprintf(outData,"voltageLowerLimit=%X.%s;",cardConfig.ucCardConf[2],temp);
	strcat(data,outData);
	/*环境温度告警门限----上限*/
	sprintf(outData,"temperatureTopLimit=%d;",cardConfig.ucCardConf[4]);
	strcat(data,outData);
	/*环境温度告警门限----下限*/
	sprintf(outData,"temperatureLowerLimit=%d;",cardConfig.ucCardConf[5]);
	strcat(data,outData);
	/*环境湿度告警门限----上限*/
	sprintf(outData,"humidityTopLimit=%d;",cardConfig.ucCardConf[6]);
	strcat(data,outData);
	/*环境湿度告警门限----下限**/
	sprintf(outData,"humidityLowerLimit=%d;",cardConfig.ucCardConf[7]);
	strcat(data,outData);
	/*风扇转速告警门限----上限*/
	sprintf(outData,"fanSpeedTopLimit=%d;",cardConfig.ucCardConf[9]*256+cardConfig.ucCardConf[8]);
	strcat(data,outData);
	/*风扇转速告警门限----下限*/
	sprintf(outData,"fanSpeedLowerLimit=%d;",cardConfig.ucCardConf[11]*256+cardConfig.ucCardConf[10]);
	strcat(data,outData);
}
/*
*功能:对singleDeviceCardUse数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void singleDeviceCardUse_DTF(char *itemName,char *data){
	int cardId ;			//板卡编号
	char outData[50];		//输出数据
	//获取板卡编号
	cardId = getCardNum(itemName,"cardId");
	//将十六进制字符转换成一般字符
	hextochar(data);
	sprintf(outData,"singleDeviceCardUse=%d;",data[cardId-1]);
	strcpy(data,outData);
}
/*
*功能:对devicesCardUse数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void devicesCardUse_DTF(char *itemName,char *data){
	int i;
	int	cardId ;			//板卡编号
	char outData[100];		//输出数据
	char temp[5];			//临时变量
	//将十六进制字符转换成一般字符
	hextochar(data);
	//设置输出数据
	strcpy(outData,"cardUse=");
	for(i = 0;i< GLOBAL_MAX_CARDNUM -1 ;i++){
		sprintf(temp,"%d,",data[i]);
		strcat(outData,temp);
	}
	strcat(outData,";");
	strcpy(data,outData);
}
/*
*功能:对singleCardVersion数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void singleCardVersion_DTF(char *itemName,char *data){
	int cardId ;		//板卡编号
	char outData[50];	//输出数据
	char temp[10];		//临时变量
	char version[20];	//版本
	char year[3];		//年
	char month[3];		//月
	char day[3];		//日
	CARD_STATUS status[GLOBAL_MAX_CARDNUM];	//板卡状态
	//将十六进制字符转换成普通字符串
	hextochar(data);
	//内存拷贝
	//memcpy(&status,data,21*16);
	int i,j;
	int d = 0;
	for(i = 0;i< 16;i++){
		for(j = 0;j< 5;j++){
			status[i].ucCardVersion[j]= data[d++];
		}
		
		for(j = 0;j< 16;j++){
			status[i].ucCardstatus[j]= data[d++];
		}
		//对齐
		d = d+3;
	}
	strcpy(data,"");
	//获取板卡编号
	cardId = getCardNum(itemName,"cardId");
	//设置板卡版本数据
	sprintf(version,"%02x;",status[cardId -1 ].ucCardVersion[1]);
	sprintf(outData,"cardVersion=V%c.%c;",version[0],version[1]);
	strcat(data,outData);
	//设置升级日期数据
	strcpy(outData,"");
	//年
	if(status[cardId -1 ].ucCardVersion[2] < 10){
		sprintf(year,"0%d",status[cardId -1 ].ucCardVersion[2]);
	}else {
		sprintf(year,"%d",status[cardId -1 ].ucCardVersion[2]);
	}
	//月
	if(status[cardId -1 ].ucCardVersion[3] < 10){
		sprintf(month,"0%d",status[cardId -1 ].ucCardVersion[3]);
	}else{
		sprintf(month,"%d",status[cardId -1 ].ucCardVersion[3]);
	}
	//日
	if(status[cardId -1 ].ucCardVersion[4] < 10){
		sprintf(day,"0%d",status[cardId -1 ].ucCardVersion[4]);
	}else{
		sprintf(day,"%d",status[cardId -1 ].ucCardVersion[4]);
	}
	sprintf(outData,"updateTime=%s-%s-%s;",year,month,day);
	strcat(data,outData);
}
/*
*功能:对singleDeviceCardExt数据转换,只有IP113S有扩展信息
*@ itemName :in
*@ data :in out
*return 
*/
void singleDeviceCardExt_DTF(char *itemName,char *data){
	int cardId ;			//板卡编号
	char outData[200];		//输出数据
	char binstr[9];			//数据的二进制字符串形式
	CARD_CONFIG cardConfig; //板卡配置信息结构
	//获取板卡编号
	cardId = getCardNum(itemName,"cardId");
	//将十六进制字符转换成普通字符串
	hextochar(data);
	//将数据内存拷贝到配置信息结构中
	memcpy(&cardConfig,data,16);
	//将板卡信息转换成二进制字符串
	int2binstr(cardConfig.ucCardConf[3],binstr);
	strcpy(data,"");		
	//lookbackTest	
	sprintf(outData,"lookbackTest=%c;",binstr[0]);
	strcat(data,outData);		
	//backPressure		
	sprintf(outData,"backPressure=%c;",binstr[2]);
	strcat(data,outData);
	//broadcastStorm
	sprintf(outData,"broadcastStorm=%c;",binstr[5]);
	strcat(data,outData);
	//handlePause
	sprintf(outData,"handlePause=%c;",binstr[7]);
	strcat(data,outData);
	//linkLose
	sprintf(outData,"linkLose=%c;",binstr[1]);
	strcat(data,outData);
	//symmetric
	sprintf(outData,"symmetric=%c;",binstr[4]);
	strcat(data,outData);
	//mode
	sprintf(outData,"mode=%c;",binstr[6]);
	strcat(data,outData);
	
}

/*
*功能:对singleDeviceCardExt数据转换,只有IP113S有扩展信息
*@ itemName :in
*@ data :in out
*return 
*/
void singleDeviceCardStatusExt_DTF(char *itemName,char *data){
	int cardId ;			//板卡编号
	char outData[200];		//输出数据
	
	CARD_STATUS status[GLOBAL_MAX_CARDNUM];	//板卡状态
	//将十六进制字符转换成普通字符串
	hextochar(data);
	//内存拷贝
	//memcpy(&status,data,21*16);
	int i,j;
	int d = 0;
	for(i = 0;i< 16;i++){
		for(j = 0;j< 5;j++){
			status[i].ucCardVersion[j]= data[d++];
		}
		
		for(j = 0;j< 16;j++){
			status[i].ucCardstatus[j]= data[d++];
		}
		//对齐
		d = d+3;
	}
	strcpy(data,"");
	//获取板卡编号
	cardId = getCardNum(itemName,"cardId");
	
	//lookbackTest	
	if(status[cardId].ucCardstatus[5] & 0x80){
		sprintf(outData,"lookbackTest=1;");
	}else{
		sprintf(outData,"lookbackTest=0;");
	}
	strcat(data,outData);
		
	//backPressure	
	if(status[cardId].ucCardstatus[5] & 0x20){
		sprintf(outData,"backPressure=1;");
	}else{
		sprintf(outData,"backPressure=0;");
	}
	strcat(data,outData);
	//broadcastStorm
	if(status[cardId].ucCardstatus[5] & 0x04){
		sprintf(outData,"broadcastStorm=1;");
	}else{
		sprintf(outData,"broadcastStorm=0;");
	}
	strcat(data,outData);
	//handlePause
	if(status[cardId].ucCardstatus[5] & 0x01){
		sprintf(outData,"handlePause=1;");
	}else{
		sprintf(outData,"handlePause=0;");
	}
	strcat(data,outData);
	//linkLose
	if(status[cardId].ucCardstatus[5] & 0x40){
		sprintf(outData,"linkLose=1;");
	}else{
		sprintf(outData,"linkLose=0;");
	}
	strcat(data,outData);
	//symmetric
	if(status[cardId].ucCardstatus[5] & 0x08){
		sprintf(outData,"symmetric=1;");
	}else{
		sprintf(outData,"symmetric=0;");
	}
	strcat(data,outData);
	//mode
	if(status[cardId].ucCardstatus[5] & 0x02){
		sprintf(outData,"mode=1;");
	}else{
		sprintf(outData,"mode=0;");
	}
	strcat(data,outData);
	
}

/*
*功能:对联级设备卡人其他属性进行数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void cardStatusCfg_DTF(char *itemName,char *data){

	char outData[200];		//输出数据
	char binstr[9];			//数据的二进制字符串形式
	char temp[50];			//临时变量
	CARD_STATUS status[GLOBAL_MAX_CARDNUM];	//板卡状态
	//将十六进制字符转换成普通字符串
	hextochar(data);
	//内存拷贝
	//memcpy(&status,data,21*16);
	int i,j;
	int d = 0;
	for(i = 0;i< 16;i++){
		for(j = 0;j< 5;j++){
			status[i].ucCardVersion[j]= data[d++];
		}
		
		for(j = 0;j< 16;j++){
			status[i].ucCardstatus[j]= data[d++];
		}
		//对齐
		d = d+3;
	}
	//获取板卡编号
	int cardId = getCardNum(itemName,"cardId") -1;
	//将板卡信息转换成二进制字符串
	strcpy(data,"");
	if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_B){ //8993
		
		//通断 
		if(status[cardId].ucCardstatus[0] & 0x08){
			sprintf(outData,"connect=1;");
		}else{
			sprintf(outData,"connect=0;");
		}
		
		strcat(data,outData);
		
		//复位 并型没有不复位
		//sprintf(outData,"reset=%c;",binstr[5]);
		strcpy(outData,"reset=1;");
		strcat(data,outData);
		//速率
		if(status[cardId].ucCardstatus[0] & 0x02){
			sprintf(outData,"speed=100M;");
		}else{
			sprintf(outData,"speed=10M;");
		}
		strcat(data,outData);
		//双工
		if(status[cardId].ucCardstatus[0] & 0x40){
			sprintf(outData,"duplex=1;");
		}else{
			sprintf(outData,"duplex=0;");
		}
		strcat(data,outData);
	}else if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_C){ //9301
		//通断 
		if(status[cardId].ucCardstatus[0] & 0x08){
			sprintf(outData,"connect=1;");
		}else{
			sprintf(outData,"connect=0;");
		}
		strcat(data,outData);
		//速率 只支持100M
		strcpy(outData,"speed=100M;");
		strcat(data,outData);
		
		//复位 并型没有不复位
		//sprintf(outData,"reset=%c;",binstr[5]);
		strcpy(outData,"reset=1;");
		strcat(data,outData);
	}else if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_F){ //9331
		//通断 
		if(status[cardId].ucCardstatus[0] & 0x08){
			sprintf(outData,"connect=1;");
		}else{
			sprintf(outData,"connect=0;");
		}
		strcat(data,outData);
		//速率 只支持100M
		strcpy(outData,"speed=100M;");
		strcat(data,outData);
		//复位 并型没有不复位
		//sprintf(outData,"reset=%c;",binstr[5]);
		strcpy(outData,"reset=1;");
		strcat(data,outData);
		//双工
		if(status[cardId].ucCardstatus[0] & 0x20){
			sprintf(outData,"duplex=1;");
		}else{
			sprintf(outData,"duplex=0;");
		}
		strcat(data,outData);
	}else if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_G){ //6652
		//通断 
		if(status[cardId].ucCardstatus[0] & 0x08){
			sprintf(outData,"connect=1;");
		}else{
			sprintf(outData,"connect=0;");
		}
		strcat(data,outData);
		//复位 并型没有不复位
		//sprintf(outData,"reset=%c;",binstr[5]);
		strcpy(outData,"reset=1;");
		strcat(data,outData);
		//自适应卡,不再提供速率和双工设置 
		//速率
//		if(binstr[6] == '1'){
//			strcpy(temp,"10M");
//		}else{
//			strcpy(temp,"100M");
//		}
//		sprintf(outData,"speed=%s;",temp);
//		strcat(data,outData);
//		//双工
//		sprintf(outData,"duplex=%c;",binstr[7]);
//		strcat(data,outData);
	}else if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_H){ //IP113
		
		//复位 并型没有不复位
		strcpy(outData,"reset=1;");
		strcat(data,outData);
		//速率
		
		if(status[cardId].ucCardstatus[0] & 0x01){
			sprintf(outData,"speed=100M;");
		}else{
			sprintf(outData,"speed=10M;");
		}
		strcat(data,outData);
		//双工
		if(status[cardId].ucCardstatus[0] & 0x40){
			sprintf(outData,"duplex=0;");
		}else{
			sprintf(outData,"duplex=1;");
		}
		strcat(data,outData);
	}else if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_I){ //1000M 
		//自适应
		if(status[cardId].ucCardstatus[1] & 0x10){
			sprintf(outData,"auto=1;");
		}else{
			sprintf(outData,"auto=0;");
		}
		strcat(data,outData);
		//复位 
		sprintf(outData,"reset=1;");
		strcat(data,outData);
		//速率
		if((status[cardId].ucCardstatus[1] & 0x60 ) == 0x40){
			sprintf(outData,"speed=1000M;");
		}else if((status[cardId].ucCardstatus[1] & 0x60 ) == 0x20){
			sprintf(outData,"speed=100M;");
		}else {
			sprintf(outData,"speed=10M;");
		}
		strcat(data,outData);
		//双工
		if(status[cardId].ucCardstatus[1] & 0x01){
			sprintf(outData,"duplex=1;");
		}else{if(status[cardId].ucCardstatus[0] & 0x04){
			sprintf(outData,"speed=100M;");
		}else{
			sprintf(outData,"speed=10M;");
		}
			sprintf(outData,"duplex=0;");
		}
		strcat(data,outData);
	}else if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_J){ //F3
		//自适应
		if(status[cardId].ucCardstatus[0] & 0x04){
			sprintf(outData,"auto=1;");
		}else{
			sprintf(outData,"auto=0;");
		}
		strcat(data,outData);
		//速率
		if(status[cardId].ucCardstatus[0] & 0x02){
			sprintf(outData,"speed=100M;");
		}else{
			sprintf(outData,"speed=10M;");
		}
		strcat(data,outData);
		//双工
		if(status[cardId].ucCardstatus[0] & 0x01){
			sprintf(outData,"duplex=1;");
		}else{
			sprintf(outData,"duplex=0;");
		}
		strcat(data,outData);	
		//复位 
		sprintf(outData,"reset=1;");
		strcat(data,outData);
		//模式
		sprintf(outData,"mode=1;");//只能是 center mode 
		strcat(data,outData);
		//////////////////////////////////////////////////////////////////
		//带宽还在配置信息里计算 
		//下行
		//下行带宽M

//		char binstr2[9];
//		int rateM ;
//		int rateK;
//		int2binstr(cardConfig.ucCardConf[2],binstr);
//		int2binstr(cardConfig.ucCardConf[3],binstr2);
//		rateM = getRunRateM(binstr,binstr2);
//		sprintf(outData,"downRunRateM=%d;",rateM);
//		strcat(data,outData);
//  
		//下行带宽K
		/*
		int2binstr(cardConfig.ucCardConf[3],binstr);
		rateK = getRunRateK(binstr);
		sprintf(outData,"downRunRateK=%d;",rateK);
		strcat(data,outData);*/ 
		////////////////////////////////////////////////////////
		//上行
		//上行带宽M
		/*
		int2binstr(cardConfig.ucCardConf[4],binstr);
		int2binstr(cardConfig.ucCardConf[5],binstr2);
		rateM = getRunRateM(binstr,binstr2);
		sprintf(outData,"upRunRateM=%d;",rateM);
		strcat(data,outData);*/
		//上行带宽K
		/*
		int2binstr(cardConfig.ucCardConf[5],binstr);
		rateK = getRunRateK(binstr);
		sprintf(outData,"upRunRateK=%d;",rateK);
		strcat(data,outData);*/

	}else if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_K){ //IP113S
		int upSample;
		int downSample;
		//复位 
		
		sprintf(outData,"reset=1;");
		strcat(data,outData);
		// 修正V3.0 卡单片机之前错误,V3.0之前用配置,V3.0只后,用状态信息
		if(status[cardId].ucCardVersion[1] >= 0x30){   
			//速率
			if(status[cardId].ucCardstatus[2] & 0x10){
				sprintf(outData,"speed=10M;");
			}else{
				sprintf(outData,"speed=100M;");
			}
			strcat(data,outData);
			//双工
			if(status[cardId].ucCardstatus[2] & 0x08){
				sprintf(outData,"duplex=1;");
			}else{
				sprintf(outData,"duplex=0;");
			}
			strcat(data,outData);
			//自适应
			if(status[cardId].ucCardstatus[2] & 0x20){
				sprintf(outData,"auto=0;");
			}else{
				sprintf(outData,"auto=1;");
			}
			strcat(data,outData);

		}
		//上行粒度
		if(status[cardId].ucCardstatus[2] & 0x04){
			sprintf(outData,"upRunSample=32;");
			upSample = 32;
		}else{
			sprintf(outData,"upRunSample=512;");
			upSample = 512; 
		}
		strcat(data,outData);	
	
		//下行粒度
		if(status[cardId].ucCardstatus[2] & 0x02){
			sprintf(outData,"downRunSample=32;");
			downSample = 32;
		}else{
			sprintf(outData,"downRunSample=512;");
			downSample = 512; 
		}
		strcat(data,outData);	
		////////////////////////////////////////////////////
		//上行带宽		
		int rate =  status[cardId].ucCardstatus[3];
		int totalRate = rate * upSample;
		if(rate == 0xFF){
			sprintf(outData,"upRunRateM=100;"); 
			strcat(data,outData);
			sprintf(outData,"upRunRateK=0;");
			strcat(data,outData);
		}else if(totalRate < 8 * 1024){
			sprintf(outData,"upRunRateM=%d;",totalRate/1024); 
			strcat(data,outData);
			sprintf(outData,"upRunRateK=%d;",(totalRate%1024)/32);
			strcat(data,outData);
		}else {
			sprintf(outData,"upRunRateM=%d;",totalRate/1024); 
			strcat(data,outData);
			strcpy(outData,"upRunRateK=0;");
			strcat(data,outData);
		}
		//下行带宽
		rate =  status[cardId].ucCardstatus[4];
		totalRate = rate * downSample;
		if(rate == 0xFF){
			sprintf(outData,"downRunRateM=100;"); 
			strcat(data,outData);
			sprintf(outData,"downRunRateK=0;");
			strcat(data,outData);
		}else if(totalRate < 8 * 1024){
			sprintf(outData,"downRunRateM=%d;",totalRate/1024); 
			strcat(data,outData);
			sprintf(outData,"downRunRateK=%d;",(totalRate%1024)/32);
			strcat(data,outData);
		}else {
			sprintf(outData,"downRunRateM=%d;",totalRate/1024); 
			strcat(data,outData);
			strcpy(outData,"downRunRateK=0;");
			strcat(data,outData);
		}
	}			
	
}


/*
*功能:对联级设备卡人其他属性进行数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void remoteCardStatus_DTF(char *itemName,char *data){

	char outData[200];		//输出数据
	char binstr[9];			//数据的二进制字符串形式
	char temp[50];			//临时变量
	CARD_STATUS status[GLOBAL_MAX_CARDNUM];	//板卡状态
	//将十六进制字符转换成普通字符串
	hextochar(data);
	//内存拷贝
	//memcpy(&status,data,21*16);
	int i,j;
	int d = 0;
	for(i = 0;i< 16;i++){
		for(j = 0;j< 5;j++){
			status[i].ucCardVersion[j]= data[d++];
		}
		
		for(j = 0;j< 16;j++){
			status[i].ucCardstatus[j]= data[d++];
		}
		//对齐
		d = d+3;
	}
	//获取板卡编号
	int cardId = getCardNum(itemName,"cardId") -1;
	//将板卡信息转换成二进制字符串
	strcpy(data,"");
	if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_J){ //F3 
		//自适应 
		if(status[cardId].ucCardstatus[1] & 0x08){
			sprintf(outData,"auto=1;");
		}else{
			sprintf(outData,"auto=0;");
		}
		strcat(data,outData);
		//速率
		if(status[cardId].ucCardstatus[1]& 0x04){
			sprintf(outData,"speed=100M;");
		}else{
			sprintf(outData,"speed=10M;");
		}
		strcat(data,outData);
		//双工
		if(status[cardId].ucCardstatus[1] & 0x01){
			sprintf(outData,"duplex=1;");
		}else{
			sprintf(outData,"duplex=0;");
		}
		strcat(data,outData); 
		//复位 //恒为复位 
		sprintf(outData,"reset=1;");
		strcat(data,outData);
		//模式 恒为终端模式  
		sprintf(outData,"mode=0;");
		strcat(data,outData);
	}else if(status[cardId].ucCardVersion[0] == BD_CARD_TYPE_K){ //IP113S 
		
		if(status[cardId].ucCardVersion[1] > 0x30){ //远端版本大于V3.0时用状态显示  
			//复位 恒为复位
			strcpy(outData,"reset=1;");
			strcat(data,outData);
			//自适应 
			if(status[cardId].ucCardstatus[7] & 0x20){
				sprintf(outData,"auto=0;");
			}else{
				sprintf(outData,"auto=1;");
			}
			strcat(data,outData);
			
			//速率
			if(status[cardId].ucCardstatus[7] & 0x10){
				sprintf(outData,"speed=10M;");
			}else{
				sprintf(outData,"speed=100M;");
			}
			strcat(data,outData);
			//双工
			if(status[cardId].ucCardstatus[7] & 0x08){
				sprintf(outData,"duplex=0;");
			}else{
				sprintf(outData,"duplex=1;");
			}
			strcat(data,outData);
		}
	}	
}


void cardConfInfo_DTF(char *itemName,char *data){
	char outData[200];		//输出数据
	char binstr[9];			//数据的二进制字符串形式
	char temp[50];			//临时变量
	CARD_CONFIG cardConfig; //板卡配置信息结构体
	//将十六进制字符转换成普通字符串
	hextochar(data);
	//将数据内存拷贝到配置信息结构中
	memcpy(&cardConfig,data,16);
	//将板卡信息转换成二进制字符串
	int2binstr(cardConfig.ucCardConf[0],binstr);
	strcpy(data,"");
	if(!strcmp(g_cardType,"F3")){ 
		//只从配置中读取带宽 
		//////////////////////////////////////////////////////////////////
		//下行
		//下行带宽M 
		char binstr2[9];
		int rateM ;
		int rateK;
		int2binstr(cardConfig.ucCardConf[2],binstr);
		int2binstr(cardConfig.ucCardConf[3],binstr2);
		rateM = getRunRateM(binstr,binstr2);
		sprintf(outData,"downRunRateM=%d;",rateM);
		strcat(data,outData);
		//下行带宽K
		int2binstr(cardConfig.ucCardConf[3],binstr);
		rateK = getRunRateK(binstr);
		sprintf(outData,"downRunRateK=%d;",rateK);
		strcat(data,outData);
		////////////////////////////////////////////////////////
		//上行
		//上行带宽M 
		int2binstr(cardConfig.ucCardConf[4],binstr);
		int2binstr(cardConfig.ucCardConf[5],binstr2);
		rateM = getRunRateM(binstr,binstr2);
		sprintf(outData,"upRunRateM=%d;",rateM);
		strcat(data,outData);
		//上行带宽K
		int2binstr(cardConfig.ucCardConf[5],binstr);
		rateK = getRunRateK(binstr);
		sprintf(outData,"upRunRateK=%d;",rateK);
		strcat(data,outData);
 
	}else if(!strcmp(g_cardType,"IP113S")){
		int2binstr(cardConfig.ucCardConf[0],binstr);
		///////////////////只从配置中读取自适应 
		//速率
		if(cardConfig.ucCardConf[0] & 0x10){
			sprintf(outData,"speed=10M;");
		}else{
			sprintf(outData,"speed=100M;");
		}
		strcat(data,outData);
		//双工
		if(cardConfig.ucCardConf[0] & 0x08){
			sprintf(outData,"duplex=1;");
		}else{
			sprintf(outData,"duplex=0;");
		}
		strcat(data,outData);
		//自适应
		if(cardConfig.ucCardConf[0] & 0x20){
			sprintf(outData,"auto=0;");
		}else{
			sprintf(outData,"auto=1;");
		}
		strcat(data,outData);
		
	}	
}

/*
*功能:对remoteCardConfigInfo数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void remoteCardConfigInfo_DTF(char *itemName,char *data){
	int cardId ;			//板卡编号
	char outData[200];		//输出数据
	char binstr[9];			//数据的二进制形式字符串
	char temp[50];			//临时变量
	CARD_CONFIG cardConfig; //板卡配置信息结构体
	//获取板卡编号
	cardId = getCardNum(itemName,"cardId");
	//将十六进制数据转换是普通字符串
	hextochar(data);
	//将数据内存拷贝到板卡配置信息结构体
	memcpy(&cardConfig,data,16);
	strcpy(data,"");
	if(!strcmp(g_cardType,"F3")){
		//自适应
		if(cardConfig.ucCardConf[8] & 0x04){
			sprintf(outData,"auto=1;");
		}else{
			sprintf(outData,"auto=0;");
		}
		strcat(data,outData);
		//速率
		if(cardConfig.ucCardConf[8] & 0x02){
			sprintf(outData,"speed=100M;");
		}else{
			sprintf(outData,"speed=10M;");
		}
		strcat(data,outData);
		//双工
		if(cardConfig.ucCardConf[8] & 0x01){
			sprintf(outData,"duplex=1;");
		}else{
			sprintf(outData,"duplex=0;");
		}
		strcat(data,outData);
		//复位
		if(cardConfig.ucCardConf[9] & 0x01){
			sprintf(outData,"reset=1;");
		}else{
			sprintf(outData,"reset=0;");
		}
		strcat(data,outData);
		//模式
		if(cardConfig.ucCardConf[10] & 0x01){
			sprintf(outData,"mode=1;");
		}else{
			sprintf(outData,"mode=0;");
		}
		strcat(data,outData);
	}else if(!strcmp(g_cardType,"IP113S")){
		//int2binstr(cardConfig.ucCardConf[4],binstr);
		//复位
		if(cardConfig.ucCardConf[4] & 0x80){
			sprintf(outData,"reset=1;");
		}else{
			sprintf(outData,"reset=0;");
		}
		strcat(data,outData);
		//自适应
		if(cardConfig.ucCardConf[4] & 0x20){
			sprintf(outData,"auto=0;");
		}else{
			sprintf(outData,"auto=1;");
		}
		strcat(data,outData);
		//速率
		if(cardConfig.ucCardConf[4] & 0x10){
			sprintf(outData,"speed=10M;");
		}else{
			sprintf(outData,"speed=100M;");
		}
		strcat(data,outData);
		//双工
		if(cardConfig.ucCardConf[4] & 0x08){
			sprintf(outData,"duplex=0;");
		}else{
			sprintf(outData,"duplex=1;");
		}
		strcat(data,outData);
	}			
	
}
/*
*功能:对singleDeviceCardRemoteConnStatus数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void singleDeviceCardRemoteConnStatus_DTF(char *itemName,char *data){
	char outData[128] = "";						//输出数据
	CARD_STATUS status[GLOBAL_MAX_CARDNUM]; //板卡状态结构体
	//将十六进制数据转换成普通字符串
	hextochar(data);
	//将数据内存拷贝到板卡状态结构体中
	//todebug
	//memcpy(&status,data,21*16);
	int i,j;
	int d = 0;
	for(i = 0;i< 16;i++){
		for(j = 0;j< 5;j++){
			status[i].ucCardVersion[j]= data[d++];
		}
		
		for(j = 0;j< 16;j++){
			status[i].ucCardstatus[j]= data[d++];
		}
		d=d+3;
	}
	int cardId = getCardNum(itemName,"cardId");
	getRemoteConnStatus(status[cardId - 1],outData);
	strcpy(data,outData);
}
/*
*功能:对cardStatusInfo数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void cardStatusInfo_DTF(char *itemName,char *data){
	char outData[1024] = "";						//输出数据
	char tempData[1024] = "";					//临时变量
	CARD_STATUS status[GLOBAL_MAX_CARDNUM]; //板卡状态结构体
	//将十六进制数据转换成普通字符串
	hextochar(data);
	//将数据内存拷贝到板卡状态结构体中
	//todebug
	//memcpy(&status,data,21*16);
	int i,j;
	int d = 0;
	for(i = 0;i< 16;i++){
		for(j = 0;j< 5;j++){
			status[i].ucCardVersion[j]= data[d++];
		}
		
		for(j = 0;j< 16;j++){
			status[i].ucCardstatus[j]= data[d++];
		}
		d=d+3;
	}
	
	//end
	//卡类型
	getCardType(status,tempData);
	strcat(outData,tempData);
	//灯状态
	getLightStatus(status,tempData);
	strcat(outData,tempData);
	//电源信息
	getPowerInfo(status,tempData);
	strcat(outData,tempData);
	//告警信息
	getAlertInfo(status,tempData);
	strcat(outData,tempData);
	//远端信息
	getRemoteInfo(status,tempData);
	strcat(outData,tempData);
	//覆盖原数据输出
	strcpy(data,outData);
}
/*
*功能:对devicesCardRemote数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void devicesCardRemote_DTF(char *itemName,char *data){
	int i ;
	char outData[1000];		//输出数据
	char temp[50];			//临时数据
	CARD_REMOTEINFO remoteInfo[GLOBAL_MAX_CARDNUM];	//远端信息
	//将十六进制数据转换为普通字符串形式
	hextochar(data);
	//将数据内存拷贝到远端信息结构体中
	memcpy(&remoteInfo,data,40*GLOBAL_MAX_CARDNUM);
	//设置输出信息
	strcpy(outData,"remoteInfo=");
	for(i = 0;i<GLOBAL_MAX_CARDNUM -1 ;i++){
		//copyStrs(temp,remoteInfo[i].ucCardRemote);
		//对于远端描述为40的情况
		memcpy(temp,remoteInfo[i].ucCardRemote,40);
		temp[40]='\0';
		strcat(outData,temp);
		strcat(outData,",");
	}
	strcat(outData,";");
	
	strcpy(data,outData);
}

/*
*功能:对singleDeviceCardRemote数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void singleDeviceCardRemote_DTF(char *itemName,char *data){
	unsigned char retData[1000];	//无符号字符串
	char temp[50]="";					//临时变量
	//获取板卡信息
	int cardId = getCardNum(itemName,"cardId");
	CARD_REMOTEINFO remoteInfo[GLOBAL_MAX_CARDNUM]; //远端信息
	//将十六进制数据转换为普通字符串形式
	hextochar1(data,retData);
	//将数据内存拷贝到远端信息结构体中
	memcpy(&remoteInfo,&retData,40*GLOBAL_MAX_CARDNUM);
	//设置输出信息
	strcpy(data,"singleDeviceCardRemote=");
	
	//copyStrs(temp,remoteInfo[cardId-1].ucCardRemote);
	memcpy(temp,remoteInfo[cardId-1].ucCardRemote,40);
	//对于远端描述为40的情况
	temp[40]='\0';
	
	strcat(data,temp);
	strcat(data,";");
	
}
/*
*功能:板卡统计
*@itemName :in
*@finalData :out
*return ,success return true,else return false
*/
boolean cardStatistics_DTF(char *itemName,char *finalData){
	
	char devString[100] = "";				//机架信息字符串 
	char itemNames[600] = "";				//硬件参数名称集
	char itemName[50];						//硬件参数名称
	boolean hasDev[GLOBAL_MAX_CARDNUM];		//机架存在标识数组
	char typeDatas[4096]= "";				//类型数据
	char subTypeDatas[768] = "";			//分批获取的卡类型 
	int i ;
	int countDevices = 0;							//统计基架,每三个机架,发一次命令 
	//获取机架连接状态信息
	if(!snmpGetFinalData("deviceLinkStatus,",devString)){
		return false;
	}
	//设置机架标识数组
	getDevStatusByStr(hasDev,devString);
	for(i = 0;i< GLOBAL_MAX_CARDNUM ;i++){
		if(hasDev[i]){
			++countDevices ;
			sprintf(itemName,"statistics_cardTypes__devId%d_,",i+1);
			strcat(itemNames,itemName);
			//每三个机架,发一次命令 
			if(countDevices == 3){
				
				//获取SNMP 结果 
				if(!snmpGetFinalData(itemNames,subTypeDatas)){
					return false;
				}
				//将批获取的类型结果加到总结果里 
				strcat(typeDatas,subTypeDatas);
				//清零 
				countDevices = 0;
				memset(subTypeDatas,0,sizeof(subTypeDatas));
				memset(itemNames,0,sizeof(itemNames));
			}
		}
	}
	//还未统计的基架不到三个 
	if(countDevices != 0){
		if(!snmpGetFinalData(itemNames,subTypeDatas)){
			return false;
		}
		//将批获取的类型结果加到总结果里 
		strcat(typeDatas,subTypeDatas);
		//清零 
		countDevices = 0;
		memset(subTypeDatas,0,sizeof(subTypeDatas));
		memset(itemNames,0,sizeof(itemNames));
	}
	/*
	*进行对板卡类型进行统计
	*
	*/
	char sub[20];				//每板卡类型
	char beCounted[512] = "";	//已被统计的板卡类型

	char result[50] = "";		//每一板卡统计结果
	char outData[500] = "";		//输出数据
	i = 0;
	while(getSubItem(typeDatas,sub,i++)){
		//对于无卡,电源卡,未定义卡不进行统计
		if(!strcmp(sub,"none") || !strcmp(sub,"power") || !strcmp(sub,"undefine")){
			continue;
		}
		//如果该类型未被统计,则进行统计
		if(!strstr(beCounted,sub)){
			//获取该类型板卡数量
			getAmountOfCardType(typeDatas,sub,result);
			//添加到输出结果中
			strcat(outData,result);
			strcat(beCounted,sub);
			strcat(beCounted," ");
		}
	}
	if(strlen(outData) == 0){
		strcpy(outData,"noCard;");
	}
	strcpy(finalData,outData);
	return true;
}

/*
*功能:对statistics_cardTypes数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void statistics_cardTypes_DTF(char *itemName,char *data){
	char outData[1024] = "";					//输出数据
	char tempData[1024] = "";					//临时变量
	CARD_STATUS status[GLOBAL_MAX_CARDNUM];		//板卡状态结构数组
	//将十六进制数据转换为一般字符串
	hextochar(data);
	//将数据内存拷贝到结构体中
	//memcpy(&status,data,21*16);
	int i,j;
	int d = 0;
	for(i = 0;i< 16;i++){
		for(j = 0;j< 5;j++){
			status[i].ucCardVersion[j]= data[d++];
		}
		
		for(j = 0;j< 16;j++){
			status[i].ucCardstatus[j]= data[d++];
		}
		d=d+3;
	}
	//卡类型
	getStatistiesCardTypes(status,tempData);
	strcat(outData,tempData);
	//追加
	strcpy(data,outData);
}


/*
*功能:通用数据转换
*@ itemName :in
*@ itemType :in
*@ data :in out
*return 
*/
void Genal_DTF(char *itemName,char *itemType ,char *data){
	char value[70];				//中间变量
	char outData[1000] = "";	//输出数据
	//如果是十六进制数据,则将其转换成一般字符串
	if(!strcmp(itemType,"Hex-STRING")){
		hextochar(data);
	}
	//设置输出数据
	sprintf(value,"%s=%s;",itemName,data);
	strcat(outData,value);
	strcpy(data,outData);
}
