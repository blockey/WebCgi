//upload.c
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"../lib/cgic.h"
#define BufferLen 1024
#define UPDATEDIR "/app/update"
int cgiMain(void){
    cgiFilePtr file;
    int targetFile;
    mode_t  mode;
    char name[128];
    char fileNameOnServer[64];
    char contentType[1024];
    char buffer[BufferLen];
    char *tmpStr=NULL;
    int size;
    int got,t;
	 char cmd[256];
	long sessionId;            //会话ID
	char value[20];            //临时变量
	char username[40];         //用户名
	char actName[50];          //请求动作
	char msg[64];					//提示信息 
	 
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
		goto END;
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
		goto END;
	}
	 
	
	//清除旧文件 
	sprintf(cmd,"rm -f /backup/app.tar.gz ");
	system(cmd);
	  
	sprintf(cmd,"rm -rf %s/*",UPDATEDIR);
	system(cmd);
	 
    //取得html页面中file元素的值，应该是文件在客户机上的路径名
    if (cgiFormFileName("updatePkg", name, sizeof(name)) !=cgiFormSuccess) {
        fprintf(stderr,"could not retrieve filename\n");
        goto FAIL;
    }  
    cgiFormFileSize("updatePkg", &size);
    //取得文件类型，不过本例中并未使用
    cgiFormFileContentType("updatePkg", contentType, sizeof(contentType));
    //目前文件存在于系统临时文件夹中，通常为/tmp，通过该命令打开临时文件。临时文件的名字与用户文件的名字不同，所以不能通过路径/tmp/userfilename的方式获得文件
    if (cgiFormFileOpen("updatePkg", &file) != cgiFormSuccess) {
        fprintf(stderr,"could not open the file\n");
        goto FAIL;
    }
    t=-1; 
    //从路径名解析出用户文件名
    while(1){
        tmpStr=strstr(name+t+1,"\\");
        if(NULL==tmpStr)
            tmpStr=strstr(name+t+1,"/");//if "\\" is not path separator, try "/"
        if(NULL!=tmpStr)
            t=(int)(tmpStr-name);
        else
            break;
    } 
    strcpy(fileNameOnServer,"/app/update/");
    strcat(fileNameOnServer,name+t+1);
    mode=S_IRWXU|S_IRGRP|S_IROTH;    
    //在当前目录下建立新的文件，第一个参数实际上是路径名，此处的含义是在cgi程序所在的目录（当前目录））建立新文件    
    targetFile=open(fileNameOnServer,O_RDWR|O_CREAT|O_TRUNC|O_APPEND,mode);
    if(targetFile<0){
        fprintf(stderr,"could not create the new file,%s\n",fileNameOnServer);
        goto FAIL;
    }
    //从系统临时文件中读出文件内容，并放到刚创建的目标文件中 
    while (cgiFormFileRead(file, buffer, BufferLen, &got) ==cgiFormSuccess){
        if(got>0)
            write(targetFile,buffer,got);    
    }
    cgiFormFileClose(file);
    close(targetFile);
    strcpy(msg,"升级文件上传成功!");
    sprintf(cmd,"/app/bin/Online&");
	 system(cmd);
    goto END; 
FAIL:
    fprintf(stderr,"Failed to upload");
    return 1;
END:    
   cgiHeaderContentType("text/html; charset=utf-8");
	printf("<html >\n");
	printf("<head> \n");
	printf("<title>Upload Information</title> \n");
	printf("</head> \n");
	printf("<body> \n");
	printf("<script type=\"text/javascript\">");
	//提示操作信息 
//	printf("alert(\"%s\");",msg);	
	//提示信息后返回原页面 
	printf("window.location.href=\"/net/progress.html\";");
	printf("</script>");
	printf("</body> \n");
	printf("</html> \n");
	return 0;
    return 0;
}
