//upload.c
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdarg.h> 
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<netdb.h> 



#include"../lib/cgic.h"
#include "../base/base.h"
#include "../util/commonUtil.h"
#define BufferLen 1024

int white_sock = 0;
//---------------------------------------------------
//建立TCP连接
//---------------------------------------------------
int htconnect(char *host, int port) {

	struct hostent * site;
	struct sockaddr_in me;
	struct timeval timeout = {3,0}; 
	site = gethostbyname(host);
	if (site == NULL) {
		webLog("getHost err");
		return -2;
	}
	white_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (white_sock < 0) {
		webLog("create sock error ,errno=%d", white_sock);
		return -1;
	}
	setsockopt(white_sock,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(struct timeval)); 
	setsockopt(white_sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(struct timeval)); 
	memset(&me, 0, sizeof(struct sockaddr_in));
	memcpy(&me.sin_addr, site->h_addr_list[0], site->h_length);
	me.sin_family = AF_INET;
	me.sin_port = htons(port);
	return (connect(white_sock, (struct sockaddr *) &me, sizeof(struct sockaddr))
			< 0) ? -1 : white_sock;
}
//---------------------------------------------------
//发送TCP数据
//---------------------------------------------------
int htsend(int sock, char *fmt, ...) {
	char BUF[1024];
	va_list argptr;
	va_start(argptr,fmt);
	vsprintf(BUF, fmt, argptr);
	va_end(argptr);
	printf("%s", BUF);
	return send(sock, BUF, strlen(BUF), 0);
}


//---------------------------------------------------
//通过TCP链接访问目的WEB的cgi
//---------------------------------------------------  
int SendToUpCfgs(char *reqtype,int itemId,int slot,char *extArgs,char *filename) {
	int black_sock;
	char msg[1024] = { 0 };
	char request[1024] = { 0 };
	char URL[1024] = { 0 };
	char ipaddr[32];
	sprintf(URL, "/cgi-bin/webadmin/subSetDevFiles.cgi");
	char *Content = "application/x-www-form-urlencoded";
	char conf[1024] = { 0 };
	sprintf(conf, "reqtype=%s&devId=%d&slot=%d&extArgs=%s&pkgName=%s", reqtype,itemId,slot,extArgs, filename);
#ifdef RUN_ON_PC	
	sprintf(ipaddr,"192.168.1.46");
#else 
	//sprintf(ipaddr,"192.168.0.%d",itemId);
	char *lIpaddr = getDistInnerIp(itemId);
	if(lIpaddr != NULL){
		sprintf(ipaddr,"%s",lIpaddr);
	}else{
		webLog("getLocalIpError\n");
		return 0;
	}
#endif 


	black_sock = htconnect(ipaddr, 80);
	if (black_sock < 1) {
		return 0;
	}
	sprintf(
			request,
			"POST %s HTTP/1.0\r\nAccept: */*\r\nContent-Length:%d\r\nContent-Type: %s\r\nAuthorization:Basic YWRtaW46YWRtaW4=\r\nCredentials:admin:admin\r\n\r\n%s",
			URL, strlen(conf), Content, conf);
	send(black_sock, request, sizeof(request), 0);
	recv(black_sock, msg, sizeof(msg), 0);
	webLog("return:%s", msg);
	close(black_sock);
	close(white_sock);
	return 1;
}

int cgiMain(void) {
	cgiFilePtr file;
	int targetFile;
	mode_t mode;
	char *pkgName = NULL;
	char name[128];
	char reqtype[128];
	char extArgs[128];
	char fileNameOnServer[64];
	char contentType[1024];
	char buffer[BufferLen];
	char *tmpStr = NULL;
	int size;
	int got, t;
	char cmd[256];
	char value[20]; //临时变量

	char msg[64]; //提示信息 

	int devId = 0;
	int slot = 0;
	/* ==================基本校验 ====================== */

	/*  获取请求参数  */
	cgiFormString("reqtype", reqtype,128);
	cgiFormString("extArgs", extArgs,128);
	if(!strcmp(reqtype,"setCrossLinkRemarks")){			//1.交叉备注信息
		cgiFormString("devId", value, 20);
		devId = atol(value);	
		//清除旧文件 
		sprintf(cmd,"/app/webapp/httpd/html/file/CrosslinkRemarks_%02d.txt",
			devId);
		remove(cmd);
	}else if(!strcmp(reqtype,"setConfigFiles")){	//2.配置信息
		cgiFormString("devId", value, 20);
		devId = atol(value);	
		cgiFormString("slot", value, 20);
		slot = atol(value);
			//清除旧文件 
		sprintf(cmd,
			"/app/webapp/httpd/html/file/CardCfgBackup_%02d_%02d.zip",
			devId, slot);
		remove(cmd);
	}else if(!strcmp(reqtype,"setAlarmFiles")){	//3.告警文件 
		cgiFormString("devId", value, 20);
		devId = atol(value);	
		//清除旧文件 
		sprintf(cmd,"/app/webapp/httpd/html/file/MSAP_AlarmLevel_%02d.cfg",
			devId);
		remove(cmd);
	}else{															//未定义 
		strcpy(msg,"undefine reqtype!");
		goto FAIL;
	}
	
	

	//取得html页面中file元素的值，应该是文件在客户机上的路径名
	if (cgiFormFileName("uploadfile", name, sizeof(name)) != cgiFormSuccess) {
		strcpy(msg, "系统出错:无法读取上传文件");
		goto FAIL;
	}
	cgiFormFileSize("uploadfile", &size);
	if (size > 30 * 1024 * 1024) {
		strcpy(msg, "不能上传大于30M的文件");
		goto FAIL;
	}
	//取得文件类型，不过本例中并未使用
	cgiFormFileContentType("uploadfile", contentType, sizeof(contentType));
	//目前文件存在于系统临时文件夹中，通常为/tmp，通过该命令打开临时文件。临时文件的名字与用户文件的名字不同，所以不能通过路径/tmp/userfilename的方式获得文件
	if (cgiFormFileOpen("uploadfile", &file) != cgiFormSuccess) {
		fprintf(stderr, "could not open the file\n");
		goto FAIL;
	}
	t = -1;
	//从路径名解析出用户文件名
	while (1) {
		tmpStr = strstr(name + t + 1, "\\");
		if (NULL == tmpStr)
			tmpStr = strstr(name + t + 1, "/");//if "\\" is not path separator, try "/"
		if (NULL != tmpStr)
			t = (int) (tmpStr - name);
		else
			break;
	}
	pkgName = name + t + 1;
	strcpy(fileNameOnServer, "/app/webapp/httpd/html/file/");
	strcat(fileNameOnServer, pkgName);
	mode = S_IRWXU | S_IRGRP | S_IROTH;
	//在当前目录下建立新的文件，第一个参数实际上是路径名，此处的含义是在cgi程序所在的目录（当前目录））建立新文件    
	targetFile = open(fileNameOnServer, O_RDWR | O_CREAT | O_TRUNC | O_APPEND,
			mode);
	if (targetFile < 0) {
		fprintf(stderr, "could not create the new file,%s\n", fileNameOnServer);
		goto FAIL;
	}
	//从系统临时文件中读出文件内容，并放到刚创建的目标文件中 
	while (cgiFormFileRead(file, buffer, BufferLen, &got) == cgiFormSuccess) {
		if (got > 0)
			if (-1 == write(targetFile, buffer, got))
			{
				perror("write error!");
			}
	}
	cgiFormFileClose(file);
	close(targetFile);
	strcpy(msg, "升级文件上传成功!");

	if(!strcmp(reqtype,"setCrossLinkRemarks")){			//1.交叉备注信息
		SendToUpCfgs(reqtype,devId,0,"resv",pkgName);
	}else if(!strcmp(reqtype,"setConfigFiles")){	//2.配置信息
		SendToUpCfgs(reqtype,devId,slot,"resv",pkgName);
	}else if(!strcmp(reqtype,"setAlarmFiles")){	//3.告警文件 
		SendToUpCfgs(reqtype,devId,0,"resv",pkgName);
	}else{															//未定义 
		strcpy(msg,"undefine reqtype!");
		goto FAIL;
	}
	
	//清辅助文件 
	sprintf(cmd,"/app/log/result");
	remove(cmd);
	
	goto END;
FAIL: 
	cgiHeaderContentType("text/html; charset=utf-8");
	printf("<html >\n");
	printf("<head> \n");
	printf("<title>Upload Information</title> \n");
	printf("</head> \n");
	printf("<body> \n");
	printf("<script type=\"text/javascript\">");
	//提示操作信息 
	printf("alert(\"%s\");", msg);
	//提示信息后返回原页面 
	printf("window.location.href=\"/net/update.html\";");
	printf("</script>");
	printf("</body> \n");
	printf("</html> \n");
	return 0;
END: 
	cgiHeaderContentType("text/html; charset=utf-8");
	printf("<html >\n");
	printf("<head> \n");
	printf("<title>Upload Information</title> \n");
	printf("</head> \n");
	printf("<body> \n");
	//	printf("<br>devId=%d <br> devId2=%d<br>\n",devId,devId2);
	//	printf("<script type=\"text/javascript\">");
	//提示操作信息 
	//printf("alert(\"%s\");",msg);	
	//提示信息后返回原页面 
	//	printf("window.location.href=\"/net/progress.html?itemId=%d\";",itemId);
	//	printf("</script>");
	printf("<br>ok <br>");
	printf("</body> \n");
	printf("</html> \n");
	return 0;
}
