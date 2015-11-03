/*
* getConfig.c
* 获取设备和板卡等的硬件参数
*/
#include <stdlib.h>
#include <stdio.h>
#include "../lib/cgic.h"
#include <string.h>
#include "../base/base.h"
#include "../services/snmpServices.h"
#include "../services/sessionServices.h"

int cgiMain(){
	
	long sessionId;         //会话ID
	char value[20];         //中间值
	char mValues[2560]="";  //获取的硬件参数
	char itemNames[500];    //要获取的硬件参数名称
	char username[40];      //用户名
	//解决中文乱码
	cgiHeaderContentType("text/html; charset=gb2312");
	/*  从请求中获取各参数  */
	cgiFormString("sessionId",value,20);
	sessionId = atol(value);
	cgiFormString("username",username,40);
	cgiFormString("itemNames",itemNames,500);
	
	//检查session有效性
	if(!isSessionValid(sessionId,username)){
		//无效session
		printf("NLogin");
		return 0;
	}else{
		
		//更新session访问时间
		updateSessionService(sessionId);
	}
	/*   对业务业发请求,获取硬件参数值  */
	
	//获取成功
	if(getDeviceParameters(itemNames,mValues)){
		//输出硬件的参数
		printf("result=true;%s",mValues);
		
	}
	//获取失败
	else{
		//提示系统出错
		printf("result=false;");
	}
	return 0;
	
}


