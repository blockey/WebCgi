/*
*snmpSetUtil.c
*执行设置硬件参数
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../base/base.h"
#include "snmpSetUtil.h"
#include "snmpUtil.h"
#include "../util/sysvar.h"
#include "../util/adstring.h"


/*
*功能:执行SNNPSET命令
*@itemName		:in		硬件参数名
*@itemValue		:in		要设置的硬件参数值
*@proRresult	:out	执行命令后命令输出结果
*return 成功,return true,else return false
*/
boolean setDeviceParametersUtil(char *itemNames,char *itemValues,char *parameters){
	char datas[2500];		//设置的各项内容
	char command[2500];		//执行命令
	int sysret = -9999;		//调用system的结果
	//获取要设置的参数值
	if(!getDeviceSetParameters(itemNames,itemValues,datas)){
		webLog("file=%s;func=%s;msg=get the final datas to set device unsucessfully;",__FILE__,__FUNCTION__);
		
		return false;
	}
	//组装命令
	strcpy(command,snmpSetCom);
	strcat(command,hostIp);
	strcat(command," ");
	strcat(command,datas);
	strcat(command," >");
	strcat(command,commandOutputFile);
	strcat(command," 2>&1");
	
	//调用系统命令
	sysret = system(command);
	if(sysret == 127){
		webLog("file=%s;func=%s;msg=shell error;",__FILE__,__FUNCTION__);
		return false;
	}
	if(sysret < 0){
		webLog("file=%s;func=%s;msg=execute command error;",__FILE__,__FUNCTION__);
		return false;
	}
	//提取命令执行结果
	if(snmpGetFinalData(itemNames,parameters)){
		return true;
	}else{
		
		return false;
	}
}


