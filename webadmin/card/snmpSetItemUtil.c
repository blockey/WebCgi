#include "snmpvar.h"
#include "../base/base.h"
#include "snmpSetItemMeta.h"
#include "snmpSetItemUtil.h"
#include "../util/adstring.h"
#include <string.h>
/*
*功能:获取Trap服务器的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean getTrapServerSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[250];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[200];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char itemType[15];		//参数类型	
	char saveData[500];		//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	getTrapServerSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取设备卡类型的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean getSingleCardTypeSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[250];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[200];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//MIB变量类型标识
	char saveData[200];		//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	getSingleCardTypeSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取设备卡使用状态的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean getSingleCardUseSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[250];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[500];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char saveData[200];		//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	getSingleCardUseSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取设备卡使用状态的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean devicesCardUseSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[250];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[200];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char saveData[200];		//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	devicesCardUseSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取机架的电源配置设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean cardPowerConfigSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[250];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[200];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char saveData[200];		//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	cardPowerConfigSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取设备卡其他参数的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean cardConfInfoSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[250];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[200];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char saveData[200];		//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	getCardConfInfoSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:批量设置卡的远端地址信息
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean devicesCardRemoteSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[2500];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[2500];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char saveData[2500];	//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	devicesCardRemoteSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取设备卡其他参数的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean devicesCardTypeSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[250];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[500];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char saveData[200];		//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	devicesCardTypeSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取设备卡远端信息的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean singleDeviceCardRemoteSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[3000];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[3000];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char saveData[3000];	//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	singleDeviceCardRemoteSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取设备卡近端扩展信息设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean singleDeviceCardExtSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[3000];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[3000];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char saveData[3000];	//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	singleDeviceCardExtSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取设备卡远端信息的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean remoteCardConfigInfoSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[3000];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[3000];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	char saveData[3000];	//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	remoteCardConfigInfoSetData(itemName,itemValue,saveData);
	strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:设置snmp代理配置参数 
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean refleshAgentCfgSetParameters(char *itemNames,char *itemValues,char *parameters){
	char datas[100];		//设置的各项内容
	char itemName[50];		//硬件参数名
	char itemValue[100];	//硬件参数值
	char itemIndent[70];	//参数名标识
	char typeIndent[5];		//参数类型标识
	//char saveData[100];	//处理后的参数值
	//获取设置项名
	getItem(itemNames,itemName,',',1);
	//获取设置值
	getItem(itemValues,itemValue,',',1);
	//获取设置项和设置类型标识符
	if(!getIndentifies(itemName,itemIndent,typeIndent)){
		return false;
	}
	//获取要设置参数的十六进制值
	//remoteCardConfigInfoSetData(itemName,itemValue,saveData);
	//strcpy(itemValue,saveData);
	//命令所需的其他参数补齐
	strcpy(datas,"");
	strcat(datas,itemIndent);
	strcat(datas," ");
	strcat(datas,typeIndent);
	strcat(datas," ");	
	strcat(datas,itemValue);
	strcpy(parameters,datas);
	return true;
}
/*
*功能:获取一般网络设置的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean get_netSetup_Parameters(char *itemNames,char *itemValues,char *parameters){
	
	int i = 1;
	unsigned char midItemValue[200];	//无符号中间变量
	char datas[350] = "";					//设置的各项内容
	char itemName[50];					//硬件参数名
	char itemValue[200];				//硬件参数值
	char itemIndent[70];				//参数名标识
	char itemType[30];					//参数类型
	char typeIndent[5];					//参数类型标识
	//对每一个参数进行转换
	while(getItem(itemNames,itemName,',',i) && getItem(itemValues,itemValue,',',i)){
		//获取设置项和设置类型标识符
		if(!getIndentifies(itemName,itemIndent,typeIndent)){
			return false;
		}
		//如果是时间,则对值进行处理
		if(!strcmp(itemName,"setDeviceTime")){
			time2str(itemValue);
		}
		else{
			//获取MIB变量类型
			getItemtype(itemName,itemType);
			//如果是16进制字符串,则要转换		
			if(!strcmp(itemType,"Hex-STRING")){
				memcpy(midItemValue,itemValue,strlen(itemValue));
				str2hexstr(midItemValue,itemValue,strlen(itemValue));
			}
				
		}
		strcat(datas,itemIndent);
		strcat(datas," ");
		strcat(datas,typeIndent);
		strcat(datas," ");	
		strcat(datas,itemValue);
		strcat(datas," ");	
		i++;
		
	}
	strcpy(parameters,datas);
	return true;
}

///////////////////////////////////////////////////////////////////
//以上函数通用工具
/*
*功能:获取MIB标识符
*@ itemName :in
*@ itemIndent :out
*@ typeIndent :out
*if success ,return true,else return false
*/
boolean getIndentifies(char *itemName,char *itemIndent,char *typeIndent){
	char itemType[15];//MIB变量类型	
	//获取MIB标识符
	if(!getItemIndent(itemName,itemIndent)){
		//没找到相应MIB标识符
		webLog("file=%s;func=%s;msg=can't get the MIB id for the typename %s;",__FILE__,__FUNCTION__,itemName);
		return false;
	}
	//获取MIB变量类型
	if(!getItemtype(itemName,itemType)){
		//没找到相应MIB变量类型
		webLog("file=%s;msg=%s;msg=can't get the data type for the typename %s;",__FILE__,__FUNCTION__,itemName);
		return false;
	}
	//获取MIB变量类型标识符
	if(!getItemtypeIndents(itemType,typeIndent)){
		//没找到相应MIB变量类型
		webLog("file=%s;func=%s;msg=can't get the MIB id for the data type %s ;",__FILE__,__FUNCTION__,itemType);
		return false;
	}
	return true;
}
