/*
 *  setupLogAction.c
 *  系统或设备参数操作日志Action
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/syslogServices.h"
#include "../services/sessionServices.h"
#include "../model/syslogModel.h"
#include "../util/syslogUtil.h"

int cgiMain() {
	char username[40];    //用户名
	char value[20];	      //中间值
	long sessionId;       //会话ID
	char actName[20];     //请求动作
	char result[5120];    //请求结果
	char params[1024];     //请求参数
	actName[0]= NULL;     //初始化
	result[0] = NULL;     //初始化
	
	//设备头部信息
	cgiHeaderContentType("text/html; charset=UTF-8");
	
	/*  获取参数  */
	cgiFormString("username",username,40);
	cgiFormString("sessionId",value,20);
	sessionId = atol(value);
	cgiFormString("actName",actName,20);
	cgiFormString("params",params,500);
	
	/* 检查session有效性  */
	//无效
	if(!isSessionValid(sessionId,username)){
		//无效session
		printf("NLogin");
		return 0;
	}
	//有效
	else{
		//更新session访问时间
		updateSessionService(sessionId);
	}
	//////////////////////////////////////////////////////
	//调用业务层处理请求
	//请求成功
	if(syslogServices("setupLog",username,params,actName,result)){
		//输出请求结果
		printf(result);
		
	}
	//请求失败
	else{
		//输入系统错误信息
		printf("error");
	}
	
	return 0;
}
