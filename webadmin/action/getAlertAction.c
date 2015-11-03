/*
 * alertAction.c
 *
 *  Created on: 26-12月-08
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/alertServices.h"
#include "../services/sessionServices.h"

int cgiMain() {
	char username[40];    //用户名
	char value[20];	      //中间值
	long sessionId;       //会话ID
	char actName[20];     //请求动作
	char result[4096];    //存放处理结果
	char params[500];     //请求参数
	actName[0]= NULL;     //初始化
	result[0] = NULL;     //初始化
	
	//解决中文乱码
	cgiHeaderContentType("text/html; charset=UTF-8");
	////////////////////////////////////////////////////
	//获取参数
	cgiFormString("username",username,40);
	cgiFormString("sessionId",value,20);
	sessionId = atol(value);
	cgiFormString("actName",actName,20);
	cgiFormString("params",params,500);
	
	//检查session有效性
	if(!isSessionValid(sessionId,username)){
		//无效session
		printf("NLogin");
		return 0;
	}else{
		//更新session访问时间
		updateSessionService(sessionId);
	}
	//////////////////////////////////////////////////////
	//调用业务层处理请求,处理后结果存放在result中
	if(alertServices(params,actName,result)){
		
		printf(result);
		
	}else{
		printf("actName=%s;result=false;",actName);
	}
	
	return 0;
}
