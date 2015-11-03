#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../base/base.h"
#include "snmpMeta.h"
#include "snmpGetItemServices.h"
#include "../util/adstring.h"
/*
*功能:提取命令执行结果
*@ str :out 结果
*return 执行成功 返回true 
*/
boolean getCommandSrcData(char *itemNames,char *str){
	char row[400];
	FILE *fp = fopen(commandOutputFile,"r");
	char *p;
	char itemValue[2024];//临时的每一item的结果
	char itemType[20];
	char tempValue[2024];//临时的每一item的结果
	char tempType[20];
	boolean ret = false;
	boolean isFind = false;
	boolean oneFinish = false;
	//用于从itemNames中提取itemName
	char itemName[50];
	int i = 1;
	boolean isEof = false;
	strcpy(str,"");
	while(!isEof){
		isEof = !fgets(row,300,fp);
		if(strstr(row,"Timeout")){
			webLog("file=%s;func=%s;msg=snmp time out;",__FILE__,__FUNCTION__);
			ret = false;
			break;
		}
		//到达文件末尾,保存最后一个结果
		if(isEof){
			//提取itemName
			getItem(itemNames,itemName,',',i);
			i++;
			//数据转换
			//dataTransfer(itemName,tempType,tempValue);
			strcat(str,tempValue);
			break;
		}
		//查找包含结果信息的行
		if( getItemType(row,itemType) && getItemValue(row,itemValue)){
			//已获取上一个结果,则保存结果
			if(isFind){
				//提取itemName
				getItem(itemNames,itemName,',',i);
				i++;
				//数据转换
				//dataTransfer(itemName,tempType,tempValue);
				strcat(str,tempValue);
			}
			strcpy(tempType,itemType);
			strcpy(tempValue,itemValue);
			trim(tempValue);
			isFind = true;
			ret = true;
		}else{
			if(isFind){
				strcat(tempValue," ");//之后每行用分号隔开
				trim(row);
				strcat(tempValue,row);
			}
		}
	}
	fclose(fp);
	if(!ret){
		webLog("file=%s;func=%s;msg=the format of snmp data error",__FILE__,__FUNCTION__);
	}
	return ret;
}
/*
*功能:提取命令执行结果
*@ str :out 结果
*return 执行成功 返回true 
*/
boolean getCommandFinalData(char *itemNames,char *str){
	char row[400];
	FILE *fp = fopen(commandOutputFile,"r");
	char *p;
	char itemValue[2024];//临时的每一item的结果
	char itemType[20];
	char tempValue[2024];//临时的每一item的结果
	char tempType[20];
	boolean ret = false;
	boolean isFind = false;
	boolean oneFinish = false;
	//用于从itemNames中提取itemName
	char itemName[50];
	int i = 1;
	boolean isEof = false;
	//strcpy(str,"");
	while(!isEof){
		isEof = !fgets(row,300,fp);
		if(strstr(row,"Timeout")){
			webLog("file=%s;func=%s;msg=snmp time out;",__FILE__,__FUNCTION__);
			ret = false;
			break;
		}
		//到达文件末尾,保存最后一个结果
		if(isEof){
			//提取itemName
			getItem(itemNames,itemName,',',i);
			i++;
			//数据转换
			dataConveter(itemName,tempType,tempValue);
			strcat(str,tempValue);
			break;
		}
		//查找包含结果信息的行
		if( getItemType(row,itemType) && getItemValue(row,itemValue)){
			//已获取上一个结果,则保存结果
			if(isFind){
				//提取itemName
				getItem(itemNames,itemName,',',i);
				i++;
				//数据转换
				dataConveter(itemName,tempType,tempValue);
				strcat(str,tempValue);
			}
			strcpy(tempType,itemType);
			strcpy(tempValue,itemValue);
			trim(tempValue);
			isFind = true;
			ret = true;
		}else{
			if(isFind){
				strcat(tempValue," ");//之后每行用分号隔开
				trim(row);
				strcat(tempValue,row);
			}
		}
	}
	fclose(fp);
	if(!ret){
		webLog("file=%s;func=%s;msg=the format of data error!",__FILE__,__FUNCTION__);
	}
	return ret;
}