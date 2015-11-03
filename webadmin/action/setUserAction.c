/*
 * setUserAction.c
 * 管理用户Action
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/userServices.h"
#include "../services/sessionServices.h"
#include "../model/userModel.h"
#include "../util/userUtil.h"
#include "../services/syslogServices.h"
#include "../services/authorityServices.h"

int cgiMain() {
	char username[40];        //用户名
	char value[20];	          //中间值
	long sessionId;           //会话ID
	char actName[20];         //请求动作
	char result[2000];        //请求结果
	char params[500];         //请求参数
	actName[0]= NULL;         //初始化
	result[0] = NULL;         //初始化
	
	//设置头部格式
	cgiHeaderContentType("text/html; charset=UTF-8");
	
	/* 获取参数  */
	cgiFormString("username",username,40);
	cgiFormString("sessionId",value,20);
	sessionId = atol(value);
	cgiFormString("actName",actName,20);
	cgiFormString("params",params,500);
	
	/* 检查session有效性 */
	//无效 
	if(!isSessionValid(sessionId,username)){
		//提示用户未登录或会话已经过期
		printf("NLogin");
		return 0;
	}
	//有效
	else{
		//更新session访问时间
		updateSessionService(sessionId);
	}
	
	/*  权限验证  */
	//权限不足
	if(!isAuthorityValid(actName,username)){
		//输入权限不足信息
		printf("NRight");
		return 0;
	}
	/* 调用业务层处理请求 */
	//请求成功
	if(userServices(params,actName,result)){
		//写操作日志
		saveSyslogService("setupLog",actName,username,cgiRemoteAddr,"1");
		//输入请求结果
		printf(result);
	}
	
	//请求失败
	else{
		//写到操作日志
		saveSyslogService("setupLog",actName,username,cgiRemoteAddr,"0");
		//输出系统错误信息
		printf("actName=%s;result=false;",actName);
	}
	return 0;
}
