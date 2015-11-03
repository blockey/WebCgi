#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/sessionServices.h"
#include "../services/authorityServices.h"

#define LOGPATH "/app/etc/update.log"

int cgiMain() {	
	long sessionId;            //会话ID
	char value[128];
	char username[40];         //用户名
	char actName[50];          //请求动作
	int itemId=0;
	char row[300];
	char msg[4096];
	char res[128];
	char cmd[512];
	FILE *log = NULL;
	boolean blSuc = true; 
	/* ==================基本校验 ====================== */
	
	/*  获取请求参数  */
	cgiFormString("sessionId",value,20); 
	sessionId = atol(value);
	cgiFormString("username",username,40);
	cgiFormString("actName",actName,50);
	cgiFormString("itemId",value,128);
	itemId =  atol(value);
	//webLog("-------itemId=%d",itemId);
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
	//下载升级信息文件 
	//清目录 
	system("rm /app/etc/update.log -f");
	//发命令下载文件  
#ifdef RUN_ON_PC	
	sprintf(cmd,"/usr/bin/wget http://192.168.1.46/file/update.log -P %s >/root/result 2>&1","/app/etc");
#else 	
	//sprintf(cmd,"/usr/bin/wget http://%s/file/update.log -P %s >/app/log/result 2>&1",getDistInnerIp(itemId),"/app/etc");
	sprintf(cmd,"cp /app/webapp/httpd/html/file/update.log /app/etc");
	webLog("cmd=%s",cmd);
#endif 
	system(cmd);
	
	
	
	//读取进度信息 
	log = fopen(LOGPATH,"r");
	if(!log){
		webLog("00000");
		strcpy(res,"false");
		blSuc = false;
		goto result;
	}
	strcpy(msg,"");
	while(fgets(row,300,log)){
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
		
	//清辅助文件 
	sprintf(cmd,"rm /app/log/result -f");
	system(cmd);


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
