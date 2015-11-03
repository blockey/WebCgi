/*
*checkLogin.c
*验证用户是否已登录
*主要根据会话中，是否存在该会话来判断
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/sessionServices.h"

int cgiMain(){
	// printf("start check login\n");
	long sessionId;      //会话ID
	char value[20];      //中间变量
	char username[40];   //用户名
	char actName[20];    //请求动作
	cgiHeaderContentType("text/html");
	/*  提取请求参数  */
	cgiFormString("username",username,40);   //用户名
	cgiFormString("sessionId",value,20);     //会话ID 字符串
	sessionId = atol(value);                 //会话ID 长整形
	cgiFormString("actName",actName,20);     //请求动作
	
	/* 验证该会话  */
	//会话有效
	if(isSessionValid(sessionId,username)){
		printf("actName=%s;result=true;",actName);
	}
	//会话无效
	else{
		printf("actName=%s;result=false;",actName);
	}
	return 0;
}

