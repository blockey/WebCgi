/*
 * login.c
 * 用户登录
 *  Created on: 26-12月-08
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/userServices.h"
#include "../services/sessionServices.h"
#include "../model/userModel.h"
#include "../model/sessionModel.h"
#include "../util/userUtil.h"
#include "../util/sessionUtil.h"
int cgiMain() {
	char username[40];
	char password[40];
	char returnValue[200];
	//解决中文乱码
	cgiHeaderContentType("text/html; charset=UTF-8");
	// cgiHeaderContentType("text/html");
	/*  获取请求参数  */
	cgiFormString("username",username,40);
	cgiFormString("password",password,40);
	webLog("username=%s",username);
	/*   判断该用户是否已登录  */
	//用户已经登录
	//if(isUserLogin(username)){
		
		//提示信息
	//	printf("result=该用户已登录!;");
		//return 0;
//	}
	/*  用户名和密码是否有效  */
	// 有效
	if(loginValid(username,password)){
		//如果用户已登录,则删除该用户会话
		deleteLastLogin(username);
		//保存会话信息
		if(!saveSessionService(username,cgiRemoteAddr)){
			printf("result=系统错误,请联系相关人员!;");
			return 0;
		}
		/////////////////////////////////////////////////
		//更新用户的登录信息
		struct User user ;     //分配用户结构空间
		//清空数据结构空间
		resetUser(&user);      
		/*  设置数据结构的相关成员值  */
		strcpy(user.username,username);
		//获取该登录用户的信息
		getUserService(&user);
		//更新用户登录信息
		updateLoginInfo(&user,cgiRemoteAddr);
		//////////////////////////////////////////////////////
		/*  获取会话信息  */
		struct Session session; //分配会话结构空间
		//清空数据结构空间
		resetSession(&session);
		/*  设置数据结构的相关成员值  */
		strcpy(session.username,username);
		//获取会话信息
		getSessionService(&session);
		
		//设置用于保存cookie 的相关数据
		sprintf(returnValue,"result=true;sessionId=%ld;userId=%ld;username=%s;category=%s;",
		session.sessionId,user.userId,user.username,user.category);
		//输出登录结果
		printf(returnValue);
		/////////////////////////////////////////////////
	}
	//无效
	else{
		//////////////////////////////////////////////////
		//输出
		printf("result=用户名或密码错误!;");
		
	}
	
	return 0;
}
