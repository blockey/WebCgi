#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/sessionServices.h"
#include "../services/authorityServices.h"

//#define LOGPATH "/app/etc/log"
#define LOGPATH "/app/etc/update.log"
int cgiMain() {	
	long sessionId;            //会话ID
	char value[128];
	char username[40];         //用户名
	char actName[50];          //请求动作
	char row[100];
	char msg[1024];
	char res[128];
	FILE *log = NULL;
	boolean blSuc = true; 
	/* ==================基本校验 ====================== */
	
	/*  获取请求参数  */
	cgiFormString("sessionId",value,20); 
	sessionId = atol(value);
	cgiFormString("username",username,40);
	cgiFormString("actName",actName,50);
	/* 检查session有效性  */
	//无效
	if(!isSessionValid(sessionId,username)){
		//无效session
		sprintf(res,"NLogin");
		blSuc = false; 
		goto result;
	}
	//有效
	else{
		//更新session访问时间
		updateSessionService(sessionId);
	}
	
	/*  权限验证  */
	//权限不足
	if(!isAuthorityValid(actName,username)){
		//输出权限不足信息
		sprintf(res,"NRight");
		blSuc = false; 
		goto result;
	}
	//读取进度信息 
	log = fopen(LOGPATH,"r");
	if(!log){
		strcpy(res,"false");
		blSuc = false;
		goto result;
	}
	strcpy(msg,"");
	while(fgets(row,100,log)){
		if(strstr(row,"失败")){
			strcat(msg,"<font color=red >");
			strcat(msg,row);
			strcat(msg,"<br>");
			strcat(msg,"</font>");
		}else{
			strcat(msg,"<font color=green >");
			strcat(msg,row);
			strcat(msg,"<br>");
			strcat(msg,"</font>");
		}
	}
	fclose(log);
result:	
	/*   ==================   输出hmtl内容  ===================  */
	cgiHeaderContentType("text/html; charset=utf-8");
	if(blSuc == true){
		printf("result@true;msg@%s;",msg);
	}else{
		printf("result@%s;",res);	
	}
	return 0;
}