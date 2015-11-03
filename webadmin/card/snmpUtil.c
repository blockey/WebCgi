/*
*snmpUtil.c
*提取参数结果
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../base/base.h"
#include "../util/adstring.h"
#include "snmpUtil.h"
#include "snmpGetItemUtil.h"
#include "snmpMeta.h"
#include "snmpvar.h"
#include "../util/lockFile.h"
#include "../model/alertModel.h"
/*
*功能:执行SNNPGET命令,提取原始数据
*@itemNames		:in 硬件参数名称
*@proRresult	:out 执行命令后结果
*return 成功,return true,else return false
*/
boolean snmpGetSrcData(char *itemNames,char *srcData){
	if(strlen(itemNames)<=0){
		webLog("file=%s;func=%s;msg=itemNames is null;",__FILE__,__FUNCTION__);
		return false;
	}
	char itemIndents[200];	//硬件参数标识符集
	char itemName[50];		//硬件参数名
	char itemIndent[60];	//硬件参数标识符
	char command[200];		//snmp命令变量
	char itemType[15];		//硬件参数类型
	int sysret=-9999;		//调用system的结果
	char tempResult[150];	//临时结果
	int i = 1;				//第几个硬件参数名称
	strcpy(itemIndents,"");
	while(getItem(itemNames,itemName,',',i)){
		//获取MIB标识符
		if(!getItemIndent(itemName,itemIndent)){
			//没找到相应MIB标识符
			webLog("file=%s;func=%s;msg=can't get the MIB id for the itemName %s;",__FILE__,__FUNCTION__,itemName);
			return false;
		}
		strcat(itemIndents,itemIndent);
		strcat(itemIndents," ");
		i++;
	}
	//组装命令
	strcpy(command,snmpGetCom);
	strcat(command,hostIp);
	strcat(command," ");
	strcat(command,itemIndents);
	strcat(command," >");
	strcat(command,commandOutputFile);
	strcat(command," 2>&1");
	//调用系统命令
	sysret = system(command);
	if(sysret == 127){
		webLog("file=%s;func=%s;msg=shell error;",__FILE__,__FUNCTION__);
		return false;
	}
	if(sysret < 0 ){
		webLog("file=%s;func=%s;msg=call snmp command error;",__FILE__,__FUNCTION__);
		return false;
	}
	//提取命令执行结果
	if(getCommandSrcData(itemNames,srcData)){
		return true;
	}else{
		
		return false;
	}
}
/*
*功能:执行SNNPGET命令,提取转换后返回给用户端的数据
*@itemNames		:in		硬件参数名称
*@proRresult	:out	执行命令后结果
*return 成功,return true,else return false
*/
boolean snmpGetFinalData(char *itemNames,char *finalData){
	if(strlen(itemNames)<=0){
		webLog("file=%s;func=%s;msg=itemNames is null ;",__FILE__,__FUNCTION__);
		return false;
	}
	char itemIndents[200] = "";		//硬件参数标识符集
	char itemName[50];			//硬件参数名
	char itemIndent[60];		//硬件参数标识符
	char command[200];			//snmp命令变量
	char itemType[15];			//硬件参数类型
	int sysret=-9999;			//调用system的结果
	char tempResult[150];		//临时结果
	int i = 1;					//第几个硬件参数名称
	while(getItem(itemNames,itemName,',',i++)){
		if(!getItemIndent(itemName,itemIndent)){
			//没找到相应MIB标识符
			webLog("file=%s;func=%s;msg=cat't get the MIB id for the typename %;",__FILE__,__FUNCTION__,itemName);
			return false;
		}
		
		strcat(itemIndents,itemIndent);
		strcat(itemIndents," ");
		
	}
	
	//组装命令
	strcpy(command,snmpGetCom);
	strcat(command,hostIp);
	strcat(command," ");
	strcat(command,itemIndents);
	strcat(command," >");
	strcat(command,commandOutputFile);
	strcat(command," 2>&1");
	//调用系统命令
	sysret = system(command);
	if(sysret == 127){
		webLog("file=%s;func=%s;msg=shell error;",__FILE__,__FUNCTION__);
		return false;
	}
	if(sysret == -1){
		webLog("file=%s;func=%s;msg=call snmp command error;",__FILE__,__FUNCTION__);
		return false;
	}
	//提取命令执行结果
	if(getCommandFinalData(itemNames,finalData)){
		return true;
	}else{
		return false;
	}
}
/*
*功能:获取光口和电口的告警信息
*@itemNames :in		参数名称
*@data		:out	输出结果	
*return 成功,return true,else return false
*/
boolean getFTAlertData(char *itemNames,char *data){
	struct Alert tempAlert; //temp alert structure 
	//打开告警信息文件
	FILE *alertfile = fopen(currentAlertFilePath,"r");
	//FILE *fp = NULL;
	//lock_t lock = lck_open(FTAlertFilePath,"r",&fp);
	//打开文件出错
	if(alertfile == NULL ){	//this file is already lock by other process 
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,currentAlertFilePath);
		return false  ;
	}
	char row[300];		//记录
	char value[20];		//中间变量
	char temp[50];		//临时变量
	char sBeginTime[20];
	char sEndTime[20];
	int ret = 0;
	//获取机架编号
	int devNum = getCardNum(itemNames,"devId");
	//设置返回结果
	strcpy(data,"FTalerts=");
	//读取文件内容
	while(1){
	
		//scanf datas to the alert structure 
		ret = fscanf(alertfile,"%d %d %d %d %d %s %s %d",
						&tempAlert.Specific,
						&tempAlert.address,
						&tempAlert.slot,
						&tempAlert.flag,
						&tempAlert.port,
						sBeginTime,
						sEndTime,
						&tempAlert.value);
		if(ret < 0){
			break;
		}
		
		//获取该机架的告警
		//getValueByName(row,"address",value);
		//设备给出的机架号是从0开始的
		//if(devNum == (atoi(value) + 1)){
		if(devNum == tempAlert.address +1 ){
			//电口0或光口1
			//getValueByName(row,"Specific",value);
			//电口 12
			//if(!strcmp(value,"12")){
			if(tempAlert.Specific == 12){
				sprintf(temp,"FX:%s,","0");
			}
			//光口 13
			//else if(!strcmp(value,"13")){
			else if(tempAlert.Specific == 13){
				sprintf(temp,"FX:%s,","1");
			}else{ //非电口或光口告警 
				continue ;
			}
			strcat(data,temp);
			//卡号
			//getValueByName(row,"slot",value);
			//{
			//	sprintf(temp,"cardId:%d,",atoi(value)+1);
				sprintf(temp,"cardId:%d,",tempAlert.slot+1);
				strcat(data,temp);
			//}
			
			//远端或近端
			//getValueByName(row,"flag",value);
			//近端 12
			//if(!strcmp(value,"0")){
			if(tempAlert.flag == 0){
				sprintf(temp,"remote:%s,","0");
			}
			//远端 13
			else{
				sprintf(temp,"remote:%s,","1");
			}
			strcat(data,temp);
			//一条告警信息结束
			strcat(data,".");
			
		}
	}
	//告警信息结束
	strcat(data,";");
	//关闭文件
	fclose(alertfile);
	//lck_close(lock,fp);
	return true;
}

/*
*功能:板卡统计
*@itemName	:in		请求参数名称
*@datas		:out	输出结果
*return ,success return true,else return false
*/
boolean getCardStatisticsData(char *itemName,char *datas){
	//调用板卡统计进行处理
	return cardStatistics_DTF(itemName,datas);
}
/*
*功能:WEB版本信息
*@itemName	:in		请求参数名称
*@datas		:out	输出结果
*return ,success return true,else return false
*/
boolean getWebVersionData(char *itemName,char *datas){
	//打开文件
	//FILE *fp = fopen(versionFilePath,"r");
	FILE *fp = NULL;
	lock_t lock = lck_open(versionFilePath,"r",&fp);
	//打开文件出错
	if(lock <0 ){	//this file is already lock by other process 
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,versionFilePath);
		return false  ;
	}
	char row[100];		//记录
	char value[100];	//中间变量
	//读取文件内容
	fgets(row,100,fp);	
	//提取版本信息字段
	getValueByName(row,"webVersion",value);
	//设置返回结果
	sprintf(datas,"%s=%s;",itemName,value);
	//关闭文件
	//fclose(fp);
	lck_close(lock,fp);
	return true;
}

