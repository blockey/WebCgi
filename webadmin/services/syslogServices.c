#include <string.h>
#include <stdlib.h>
#include <time.h>
/*
*syslogServices.c
*日志服务接口
*/
#include "../base/base.h"
#include "syslogServices.h"
#include "../dao/syslogDao.h"
#include "../util/syslogUtil.h"
#include "../util/pageUtil.h"
#include "../model/syslogModel.h"
#include "../util/sysvar.h"
#include "../util/commonUtil.h"
#include "../model/userModel.h"
#include "../util/adstring.h"
#include "userServices.h"
/*
 * 功能，保存日志
 * @ category :in
 * @ actName :in
 * @ username :in
 * @ loginIp :in
 * @ isSuccess :in
 * 保存成功返回真,否则返回假
 */
boolean saveSyslogService(char *category,char *actName,char *username,char *loginIp,char *isSuccess){	
	
	struct Syslog syslog;				//日志
	//重置数据空间
	resetSyslog(&syslog);	
	//设置用户名
	if(username != NULL && strlen(username) > 0){
		strcpy(syslog.username,username);	
	}else{
		strcpy(syslog.username,"null");
	}
	//设置登录IP
	strcpy(syslog.loginIp,loginIp);	
	//设置操作结果
	strcpy(syslog.isSuccess,isSuccess);
	
	//获取该操作描述
	if(actName != NULL && strlen(actName) > 0){
		getActionDesc(actName,syslog.action);
	}else{
		getActionDesc("null",syslog.action);
	}
	//设置创建时间为当前时间
	syslog.createTime = time(NULL);
	//设置日志ID为当前时间
	syslog.logId = syslog.createTime;
	//设置日志文件
	setLogFile(category);
	//调用dao进行保存
	//保存成功
	if(saveSyslogDao(&syslog)){
		return true;
	}
	//保存失败
	return false;
}

/*
*功能:删除日志
*@ category :in
*@ params   :in
*return 操作结果
*根据需要,此函数不再使用
*/
boolean deleteSyslogsService(char *category,char *params){
	char value[300];	//中间变量
	//设置日志文件
	setLogFile(category);
	//获取要删除的日志ID信
	getValueByName(params,"logIds",value);
	//dao
	if(deleteSyslogsDao(value)){
		return true;
	}
	return false;
}


/*
*功能:针对用户的操作都调用这个函数进行处理
*@category :in	日志类别
*@username :in  用户名
*@params :in    请求参数
*@actName :in   请求动作
*@result :out   处理结果输出
*操作成功返回真,否则返回假
*
*/
boolean syslogServices(char *category,char *username,char *params,char *actName,char *result){
	
	struct Syslog syslog ;		//日志
	char value[1000];			//中间变量 
	char timeString[50];		//查询时间段范围
	
	
	//重置数据空间
	resetSyslog(&syslog);	
	//根据字符串参数设置log
	setSyslogByString(&syslog,params);
	/*
	*根据需要,删除日志记录不再使用
	*/
	/*
	if(!strcmp(actName,"deleteSyslogs")){
		if(deleteSyslogsService(category,params)){
			sprintf(result,"actName=%s;result=true;",actName);
		}else{
			sprintf(result,"actName=%s;result=false;",actName);
		}
		return true;
	}*/
	if(!strcmp(actName,"querySyslogs")){
		struct Syslogs *syslogs;	//日志链表
		struct Syslogs *p;			//日志链表变量
		struct Syslog *rSyslog;		//日志链表的日志信息
		struct Page page;			//分页
		//重置数据空间
		resetPage(&page);
		struct TimeRange tr;		//时间段查询范围
		
		//获取当前用户
		struct User user;			//用户
		//重置用户空间
		resetUser(&user);
		//设置用户名
		strcpy(user.username,username);
		getUserService(&user);
		//不是系统管理员,则只能查看自己的记录
		if(strcmp(user.category,"1")){
			//不是模糊搜索,用##0标识
			strcpy(syslog.username,username);
			strcat(syslog.username,"##0");
		}
		//如果是管理员,则进行模糊搜索
		else{
			//用户名是搜索条件
			if(syslog.username[0] != 0){
				//不是模糊搜索,用##1标识
				strcat(syslog.username,"##1");
			}
		}
		//根据字符串参数设置Page
		setPageByString(&page,params);
		//计算分页数据
		calculatePage(&page);
		
		//根据字符串参数设置TimeRange
		setTimeRangByString(&tr,params);
		//设置日志文件
		setLogFile(category);
		//dao
		if(querySyslogsDao(&syslog,&page,&tr,&syslogs)){
			char list[4192]="";
			//初始化
			strcpy(result,"");
			//设置返回结果信息
			sprintf(value,"actName=%s;result=true;",actName);
			strcat(result,value);
			//设置返回分页信息
			sprintf(value,"current=%d;pageSize=%d;total=%d;",page.current,page.pageSize,page.total);
			strcat(result,value);
			//设置返回记录信息
			strcat(result,"list=");
			p = syslogs;
			//没有符合条件的结果
			if(syslogs->next == NULL){
				strcat(list,"none;");
				strcat(result,list);
				////释放内存
				releaseUsersMemory(syslogs);
				return true;
			}
			//有符合条件的结果
			while(syslogs->next){
				//指向下一结点,头结果不储存日志信息
				syslogs = syslogs->next;
				if(syslogs == NULL){
					break;
				}
				//结点的日志信息
				rSyslog = syslogs->syslog;
				/*
				*将日志的各项信息设置返回记录字符串中
				*/
				//日志ID
				if(getValueByName(params,"logId",value)){
					sprintf(value,"logId_%ld,",rSyslog->logId);
					strcat(list,value);
				}
				//创建时间
				if(getValueByName(params,"createTime",value)){
					long2TimeStr(rSyslog->createTime,timeString);
					sprintf(value,"createTime_%s,",timeString);
					strcat(list,value);
				}
				//用户名
				if(getValueByName(params,"username",value)){
					sprintf(value,"username_%s,",rSyslog->username);
					strcat(list,value);
				}
				//是否成功
				if(getValueByName(params,"isSuccess",value)){
					sprintf(value,"isSuccess_%s,",rSyslog->isSuccess);
					strcat(list,value);
				}
				//动作
				if(getValueByName(params,"action",value)){
					sprintf(value,"action_%s,",rSyslog->action);
					strcat(list,value);
				}
				//登录IP
				if(getValueByName(params,"loginIp",value)){
					sprintf(value,"loginIp_%s,",rSyslog->loginIp);
					strcat(list,value);
				}
				//使用#分隔每条记录
				strcat(list,"#");
			}
			//使用;分隔每项信息
			strcat(list,";");
			strcat(result,list);
			//释放链表内存
			releaseSyslogsMemory(p);
		}
		return true;
	}
	webLog("file=%s;func=%s;msg=no handling procedure for the actName %s;",__FILE__,__FUNCTION__,actName);
	return false;
}
