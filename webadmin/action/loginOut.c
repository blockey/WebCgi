/*
*loginOut.c
*退出网管系统
*/
#include <stdio.h>
#include <stdlib.h>
#include "../lib/cgic.h"
#include "../services/sessionServices.h"
#include "../base/base.h"

int cgiMain(){
	long sessionId;           //会话ID
	char username[40];        //用户名
	char value[20];           //中间值
	char actName[20];         //请求动作
	cgiHeaderContentType("text/html");
	/*  获取请求参数  */
	cgiFormString("sessionId",value,20);
	sessionId = atol(value);
	cgiFormString("username",username,40);
	cgiFormString("actName",actName,20);
	/*  删除用户该次会话信息 */
	//执行成功
	if(deleteSessionService(sessionId)){
		
		
		
		//输出请求结果
		printf("actName=%s;result=true;",actName);
	}
	//执行失败
	else{
		
		
		//输出请求结果
		printf("actName=%s;result=false;",actName);
	}
	return 0;
 	
}
