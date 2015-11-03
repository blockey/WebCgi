#include <stdlib.h>
#include "../model/syslogModel.h"
#include "syslogUtil.h"
#include "../base/base.h"
#include "adstring.h"
/*
*功能：重置日志结构空间
*@ user :in out
*return
*/
void resetSyslog(struct Syslog *syslog){
	//日志ID
	syslog->logId=0;
	//用户名
	syslog->username[0]=0;
	//创建时间
	syslog->createTime = 0;
	//是否成功
	syslog->isSuccess[0]=0;
	//操作描述
	syslog->action[0]= 0;
	//登录IP
	syslog->loginIp[0]=0;
	
}
/*
*功能:根据请求参数设置日志信息
*@user :out
*@str :in 字符串参数
*return 
*/
void setSyslogByString(struct Syslog *syslog,char *str){
	char value[300];		//中间变量
	//日志ID
	if(getValueByName(str,"logId",value)){
		syslog->logId = atol(value);
	}
	//用户名
	getValueByName(str,"username",syslog->username);
	//创建时间
	if(getValueByName(str,"createTime",value)){
		syslog->createTime = atol(value);
	}
	//是否成功
	getValueByName(str,"isSuccess",syslog->isSuccess);
	//操作动作描述
	getValueByName(str,"action",syslog->action);
	//登录IP
	getValueByName(str,"loginIp",syslog->loginIp);
	
}
/*
*功能：释放链表内存
*users :in out
*return 
*/
void releaseSyslogsMemory(struct Syslogs *syslogs){
	struct Syslogs *p;		//链表指针
	//逐个结点释放
	while(syslogs){
		free(syslogs->syslog);
		p = syslogs;
		syslogs = syslogs->next;
		free(p);
	}
}




