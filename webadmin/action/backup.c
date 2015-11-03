#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/sessionServices.h"
#include "../services/syslogServices.h"
#include "../services/authorityServices.h"
#define DOWNLOAD_URL "/file/agent.gtb"

int cgiMain() {	
	char cmd[256];
	long sessionId;            //会话ID
	char value[20];            //临时变量
	char username[40];         //用户名
	char actName[50];          //请求动作
	char res[256];					//处理结果 
	boolean blSuc = true ;		//请求处理成功 
	
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
	sprintf(cmd,"cp -f %s %s",srcAgentConfPath,bakAgentConfPath);
	if(system(cmd)<0){
		sprintf(res,"false");
		blSuc = false; 
		goto result;		
	}
result:
	/*   ================ 业务逻辑处理  ===================*/
	//将当前的snmp代理配置文件拷贝到指定目录下供下载 
	if(blSuc == true){
		
		//写操作日志
		saveSyslogService("setupLog",actName,username,cgiRemoteAddr,"1");
	}else{
		//写操作日志
		saveSyslogService("setupLog",actName,username,cgiRemoteAddr,"0");
	}
	
	
	/*   ==================   输出hmtl内容  ===================  */
	cgiHeaderContentType("text/html; charset=utf-8");
	if(blSuc == true){
		printf("result=true;url=%s",DOWNLOAD_URL);
	}else{
		printf("result=%s;",res);	
	}
	return 0;
}

