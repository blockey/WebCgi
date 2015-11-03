/*
 * getUserAction.c
 * 获取用户名信息Action
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
#include "../services/authorityServices.h"

int cgiMain() {
	char username[40];   //用户名
	char value[20];	     //中间值
	long sessionId;      //会话ID
	char actName[20];    //请求动作
	char result[4096];   //请求结果
	char params[500];    //请求参数
	actName[0]= NULL;    //初始化
	result[0] = NULL;    //初始化

	//解决中文乱码
	cgiHeaderContentType("text/html; charset=UTF-8");
	
	/*    获取参数   */
	cgiFormString("username",username,40);
	cgiFormString("sessionId",value,20);
	sessionId = atol(value);
	cgiFormString("actName",actName,20);
	cgiFormString("params",params,500);
	
	/*  检查session有效性   */
	//会话无效
	if(!isSessionValid(sessionId,username)){
		//提示用户未登录或会话已过期
		printf("NLogin");
		return 0;
	}
	//会话有效
	else{
		//更新session访问时间
		updateSessionService(sessionId);
	}
	
	/*    权限验证    */
	//权限不足
	if(!isAuthorityValid(actName,username)){
		//提示权限不足
		printf("NRight");
		return 0;
	}
	
	/*   调用业务层处理请求   */
	//请求处理成功
	if(userServices(params,actName,result)){
		//输出请求结果
		
		printf(result);
		
	}
	//请求处理失败
	else{
		//输出系统出错信息
		printf("actName=%s;result=false",actName);
	}
	
	return 0;
}
