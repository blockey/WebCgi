#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/sessionServices.h"
#include "../services/authorityServices.h"

#define UPDATEDIR "/app/update"

int cgiMain() {	
	cgiFilePtr file;
	char buffer[1024];
	int got;
	char cmd[256];
	long sessionId;            //会话ID
	char value[20];            //临时变量
	char username[40];         //用户名
	char actName[50];          //请求动作
	boolean blSuc = true ;		//请求处理成功 
	char msg[256];					//提示信息 
	char updatePkgPath[256];
	
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
		sprintf(msg,"未登录或会话已过期!");
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
		sprintf(msg,"您无此操作权限!");
		blSuc = false; 
		goto result;
	}
	
	
	/*=================  业务逻辑处理  =========================     */
	//judge file size
	int fileSize =0; 
	if(cgiFormFileSize("updatePkg",&fileSize) != cgiFormSuccess){
		webLog("Get the upload file size error.");
		sprintf(msg,"系统出错,请稍后重试!");
		blSuc = false;
		goto result;
	}else {
		//if the file size is too big 
		if(fileSize > 4*1024* 1024){//4M
			webLog("the upload file is too big.");
			sprintf(msg,"升级包不可以大于4M!");
			blSuc = false;
			goto result;
		}
	}
	///////////////////
	//升级目录下的所有文件
	sprintf(cmd,"rm -rf %s/*",UPDATEDIR);
	system(cmd);
	////////////////////上传文件 
	sprintf(updatePkgPath,"%s/%s",UPDATEDIR,"updatePkg.zip");
	FILE *fp = fopen(updatePkgPath,"w+b");
	//将上传的文件存放到指定位置 
	if (cgiFormFileOpen("updatePkg", &file) != cgiFormSuccess) {
		webLog("Could not get the upload file.");
		sprintf(msg,"系统出错,请稍后重试!");
		blSuc = false;
	}else{
		
		while (cgiFormFileRead(file, buffer, sizeof(buffer), &got) == cgiFormSuccess){
			fwrite(buffer,1,got,fp);
		}
		cgiFormFileClose(file);
		sprintf(msg,"成功上传升级文件!!");
		blSuc = true ;
	}
	fclose(fp);
	
	//执行升级
	sprintf(cmd,"/app/bin/Online&");
	system(cmd);
	
	/* ====================   处理结果    ======================*/
result:
	
	
	/*   ==================   输出hmtl内容  ===================  */
	cgiHeaderContentType("text/html; charset=utf-8");
	printf("<html >\n");
	printf("<head> \n");
	printf("<title>Upload Information</title> \n");
	printf("</head> \n");
	printf("<body> \n");
	printf("<script type=\"text/javascript\">");
	//提示操作信息 
	printf("alert(\"%s\");",msg);	
	//提示信息后返回原页面 
	printf("window.location.href=\"/net/progress.html\";");
	printf("</script>");
	printf("</body> \n");
	printf("</html> \n");
	return 0;
}

