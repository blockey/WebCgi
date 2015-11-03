/*
*alertDao.c
*告警信息DAO
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../base/base.h"
#include "alertDao.h"
#include "../model/alertModel.h"
#include "../util/alertUtil.h"
#include "../util/adstring.h"
#include "../util/lockFile.h"

/* 数据库格式出错处理函数*/
boolean tableError(int counter,FILE *tablefile,char *tablefilePath){
	char cmd[256];
	if(counter > MAX_TABLE_RECORDS){//当前数据表已超出允许人最大记录数 ,一般是数据表出错造成 
		//关闭数据表文件 
		pclose(tablefile);
		//将一空文件覆盖出错的数据表文件
		sprintf(cmd,"cp -f %s %s ",blankFilePath,tablefilePath); 
		system(cmd);
		return true; 
	}
	return false;
}

/*
*功能：清空所有告警 
*success return true,else return false;
*/
boolean deleteAllAlertsDao(){
	char cmd[64];
	//add lock 
	FILE *alertfile = NULL; 
	lock_t lock_alert = lck_open(alertFilePath,"a",&alertfile);
	if(lock_alert < 0){
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,alertFilePath);
		return false  ;
	}
	//clear alert.db
	sprintf(cmd,"cp %s %s -f ",blankFilePath,alertFilePath);
	system(cmd);
	lck_close(lock_alert,alertfile);
	//clear FTAlert.db 
	//sprintf(cmd,"cp %s %s -f ",blankFilePath,currentAlertFilePath);
	//system(cmd);
	return true;
}
/*
*功能：查询当前告警
*@condition :in　在这里设置要查询用户的条件
*@page :in out 分页信息
*@list :out 查询结果指针
*success return true,else return false;
*/
boolean queryCurrentAlertsDao(struct Alert *condition,struct Page *page,struct TimeRange *tr,struct Alerts **list){
	int ret = 0;
	char value[200];              //中间值
	struct Alerts *alerts;        //告警信息集合
	struct Alert *alert ;         //告警信息
	struct Alert *tempAlert;
	
	//为告警链表分配空间
	struct Alerts *p = (struct Alerts *)malloc(sizeof(struct Alerts));
	*list = p;                    //返回链表指针指向新分配的空间
	p->alert = NULL;              //初始化当前结点的告警指针
	p->next = NULL;               //初始化当前结点的下一告警指针

	struct Alerts *alertsDbHead = getAlertSortByTime(currentAlertFilePath);
	if(alertsDbHead == NULL){
		free(p);
		return false; 
	}
	struct Alerts *currentAlertLink = alertsDbHead;
	while((currentAlertLink = currentAlertLink->next) != NULL ){	
		
		//查询时间段的开始和结束时间
		if(tr->from != 0 || tr->to != 0){
			//开始时间不为空,结束时间为空,则查询出大于等于开始时间的记录
			if(tr->from != 0 && tr->to == 0){
				if(currentAlertLink->alert->begintime < tr->from){
					continue;
				}
			}
			//开始时间为空,结束时间不为空,则查询出小于等于结束时间的记录
			if(tr->from == 0 && tr->to != 0){
				if( currentAlertLink->alert->begintime > tr->to){
					continue;
				}
			}
			//开始时间和结束时间均不为空,则查询出在开始时间和结束时间之间的记录
			if(tr->from != 0 && tr->to != 0) {
				if( currentAlertLink->alert->begintime < tr->from || currentAlertLink->alert->begintime > tr->to ){
					continue;
				}
			}
			
		}
		/*各个条件逐一检测	
		* 只要有一个条件不符合,则该记录不符合,跳出该次循环
		*/

		
		//告警类别,实际中并未用到
		if(condition->Specific != 0 && condition->Specific != currentAlertLink->alert->Specific){
			continue;
		}

		//机架号
		//100 表示所有
		if(condition->address != 100 && condition->address != currentAlertLink->alert->address){
			continue;
		}
		//槽位
		//100 表示所有
		if(condition->slot != 100 && condition->slot != currentAlertLink->alert->slot){
			continue;
		}
		//远近端
		if(condition->flag != 0 && condition->flag != currentAlertLink->alert->flag){
			continue;
		}
		//端口,电源专用
		if(condition->port != 0 && condition->port != currentAlertLink->alert->port){
			continue;
		}
		/*
		*告警级别
		*参数值 0 表示一般告警
		*参数值 1 表示严重告警
		*参数值 2 表示所有告警
		*前两种告警与数据表中的值一致
		*最后一种则所有记录符合这一条件
		*/
		
		//非请求所有告警,则进行匹配
		if(condition->value != 2 && condition->value != currentAlertLink->alert->value){
			continue;
		}
		/*
		*分页条件
		*该记录还必须满足分页要求,即取第几条开始,共多少条记录
		*/
		if(page != NULL){
			++(page->total);//符合条件的记录计数自增
			//如果该记录不在分页要求的范围内,则转入下一轮循环
			if(page->total < page->start || page->total > page->end){
				continue;
			}
			
		}
		/*
		*已经符合所有条件,将记录添加到查询结果链表中
		*
		*/

		//为存放当前记录分配链表结点空间
		alerts = (struct Alerts *)malloc(sizeof(struct Alerts));
		//将新结点加入链表中
		p->next = alerts;
		//分配记录信息空间
		alert = (struct Alert*)malloc(sizeof(struct Alert));
		//重置记录信息空间
		resetAlert(alert);

		/*
		*将记录的各字段值相应赋给告警数据结构的成员变量
		*/
		
		memcpy(alert,currentAlertLink->alert,sizeof(struct Alert));
		
		//将该告警赋于当前结点的告警指针
		alerts->alert=alert;
		//将当前结点的下一结点设置为空
		alerts->next = NULL;
		//将p指针指向当前结点
		p = alerts;
		
		
	}
	//realese memory 

	distroyAlert(alertsDbHead);

	//返回
	return true;
	
}


/*
*功能：查询历史告警
*@condition :in　在这里设置要查询用户的条件
*@page :in out 分页信息
*@list :out 查询结果指针
*success return true,else return false;
*/
boolean queryAllAlertsDao(struct Alert *condition,struct Page *page,struct TimeRange *tr,struct Alerts **list){
	int ret = 0;
	char value[200];              //中间值
	struct Alerts *alerts;        //告警信息集合
	struct Alert *alert ;         //告警信息
	struct Alert tempAlert;
	char sBeginTime[20];
	char sEndTime[20];
	char command[100];            //执行的命令字符串
	char record[300];
	
	/*
	*为提高效率,修改为使用管程通讯 
	*
	*/
	FILE *alertfile = NULL;      //alert file 
	//command 
	sprintf(command,"sed -e '1!G;h;$!d' %s",alertFilePath);
	if((alertfile = popen(command,"r")) == NULL){	//error 
		webLog("file=%s;func=%s;msg=open pipe %s error;",__FILE__,__FUNCTION__,alertFilePath);
		return false; 
	}
	//为告警链表分配空间
	struct Alerts *p = (struct Alerts *)malloc(sizeof(struct Alerts));
	*list = p;                    //返回链表指针指向新分配的空间
	p->alert = NULL;              //初始化当前结点的告警指针
	p->next = NULL;               //初始化当前结点的下一告警指针
	//read alert file 
	while(fgets(record,300,alertfile)){
		//对记录进行校验, 
		if(!checkTableRecordValid(record,8)){
			continue;//跳过错误数据 
		}
		//scanf datas to the alert structure 
		sscanf(record,"%d %d %d %d %d %s %s %d",
						&tempAlert.Specific,
						&tempAlert.address,
						&tempAlert.slot,
						&tempAlert.flag,
						&tempAlert.port,
						sBeginTime,
						sEndTime,
						&tempAlert.value);
		if(strcmp(sBeginTime,"0")){
			sBeginTime[10]=' ';
			tempAlert.begintime = str2time_t(sBeginTime);
		}else{
			tempAlert.begintime = 0;
		}
		if(strcmp(sEndTime,"0")){
			sEndTime[10]=' ';
			tempAlert.endtime = str2time_t(sEndTime);
		}else{
			tempAlert.endtime = 0;
		}
		
		//查询时间段的开始和结束时间
		if(tr->from != 0 || tr->to != 0){
			//开始时间不为空,结束时间为空,则查询出大于等于开始时间的记录
			if(tr->from != 0 && tr->to == 0){
				if(tempAlert.begintime < tr->from){
					continue;
				}
			}
			//开始时间为空,结束时间不为空,则查询出小于等于结束时间的记录
			if(tr->from == 0 && tr->to != 0){
				if( tempAlert.begintime > tr->to){
					continue;
				}
			}
			//开始时间和结束时间均不为空,则查询出在开始时间和结束时间之间的记录
			if(tr->from != 0 && tr->to != 0) {
				if( tempAlert.begintime < tr->from || tempAlert.begintime > tr->to ){
					continue;
				}
			}
			
		}
		/*各个条件逐一检测	
		* 只要有一个条件不符合,则该记录不符合,跳出该次循环
		*/

		
		//告警类别,实际中并未用到
		if(condition->Specific != 0 && condition->Specific != tempAlert.Specific){
			continue;
		}
		/*
		*告警类别:当前告警,历史告警,所有告警
		*当前告警,则请求参数中用1表示
		*历史告警,则请求参数中用0表示
		*所有告警,则请求参数中用2表示
		*如果是所有告警,则所有记录均符合这一条件,所有只判断前两种情况则可
		*/
		

		//机架号
		//100 表示所有
		if(condition->address != 100 && condition->address != tempAlert.address){
			continue;
		}
		//槽位
		//100 表示所有
		if(condition->slot != 100 && condition->slot != tempAlert.slot){
			continue;
		}
		//远近端
		if(condition->flag != 0 && condition->flag != tempAlert.flag){
			continue;
		}
		//端口,电源专用
		if(condition->port != 0 && condition->port != tempAlert.port){
			continue;
		}
		/*
		*告警级别
		*参数值 0 表示一般告警
		*参数值 1 表示严重告警
		*参数值 2 表示所有告警
		*前两种告警与数据表中的值一致
		*最后一种则所有记录符合这一条件
		*/
		
		//非请求所有告警,则进行匹配
		if(condition->value != 2 && condition->value != tempAlert.value){
			continue;
		}
		/*
		*分页条件
		*该记录还必须满足分页要求,即取第几条开始,共多少条记录
		*/
		if(page != NULL){
			++(page->total);//符合条件的记录计数自增
			//如果该记录不在分页要求的范围内,则转入下一轮循环
			if(page->total < page->start || page->total > page->end){
				continue;
			}
			
		}
		/*
		*已经符合所有条件,将记录添加到查询结果链表中
		*
		*/

		//为存放当前记录分配链表结点空间
		alerts = (struct Alerts *)malloc(sizeof(struct Alerts));
		//将新结点加入链表中
		p->next = alerts;
		//分配记录信息空间
		alert = (struct Alert*)malloc(sizeof(struct Alert));
		//重置记录信息空间
		resetAlert(alert);

		/*
		*将记录的各字段值相应赋给告警数据结构的成员变量
		*/
		
		memcpy(alert,&tempAlert,sizeof(struct Alert));
		
		//将该告警赋于当前结点的告警指针
		alerts->alert=alert;
		//将当前结点的下一结点设置为空
		alerts->next = NULL;
		//将p指针指向当前结点
		p = alerts;
	}
	pclose(alertfile);
	//返回
	return true;
	
}

/*********************      排序            **************************/

boolean  addAlertTolinks(const struct Alerts *head,const struct Alert *alert){
	struct Alerts *alertTail = head;
	while(alertTail->next){
		alertTail = alertTail->next;
	}
	//new
	struct Alerts *newAlertLink = (struct Alerts *)malloc(sizeof(struct Alerts));
	if(newAlertLink == NULL){
		return false ; 
	}
	//set attribute 
	newAlertLink->alert = alert;
	newAlertLink->next = NULL;
	newAlertLink->last = alertTail; 
	alertTail->next = newAlertLink;
	return true;
}
struct Alerts *getAlertsLinkHead(){
	//new
	struct Alerts *head = (struct Alerts *)malloc(sizeof(struct Alerts));
	if(head == NULL){
		return NULL; 
	}
	memset(head,0,sizeof(struct Alerts));
	head->next = NULL;
	head->last = NULL;
	return head;
}
void distroyAlert(struct Alerts *head){
	struct Alerts *alertLink = head ,*p;
	while(alertLink){
		p = alertLink;
		alertLink = alertLink->next;
		free(p->alert);
		free(p);
	}
}

boolean sortAlerts(struct Alerts *head){
	struct Alerts *currentAlertLink,*toSortAlertLink,*temp;
	currentAlertLink = head->next;
	
	while(currentAlertLink){
		toSortAlertLink = currentAlertLink->next;
		while(toSortAlertLink){
			
			if(toSortAlertLink->alert->begintime > currentAlertLink->alert->begintime){
				//last ->next exchange 
				currentAlertLink->last->next = toSortAlertLink;
				toSortAlertLink->last->next = currentAlertLink;
				
				//current ->last exchange 
				temp = currentAlertLink->last;
				currentAlertLink->last = toSortAlertLink->last;
				toSortAlertLink->last = temp;
				
				//next ->last exchange 
				if(currentAlertLink->next){
					currentAlertLink->next->last = toSortAlertLink;
				}
				if(toSortAlertLink->next){
					toSortAlertLink->next->last = currentAlertLink;
				}

				//current ->next exchange 
				temp = toSortAlertLink->next;
				toSortAlertLink->next = currentAlertLink->next ;
				currentAlertLink->next = temp;
				//variable exchange 
				temp = currentAlertLink;
				currentAlertLink = toSortAlertLink;
				toSortAlertLink = temp;
			}
			toSortAlertLink = toSortAlertLink->next;
		}
		currentAlertLink = currentAlertLink->next;
	}
	return true;
}
//
struct Alerts *getAlertSortByTime(char *alertDB){
	char sBeginTime[20];
	char sEndTime[20];	
	FILE *alertfile = NULL;      //alert file 
	int counter = 0;
	char record[300];
	alertfile = fopen(alertDB,"r");
	if(alertfile== NULL){	//error 
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,alertDB);
		return false; 
	}
	
	struct Alerts *head = getAlertsLinkHead();
	int ret ;
	if(head == NULL){
		return NULL ;
	}
	
	while(fgets(record,300,alertfile)){
		//对记录进行校验, 
		if(!checkTableRecordValid(record,8)){
			continue;//跳过错误数据 
		}
		//scanf datas to the alert structure 
		struct Alert *tempAlert = (struct Alert *)malloc(sizeof(struct Alert));
		if(tempAlert == NULL){
			webLog("file=%s;func=%s;msg=allocate momery %s error;",__FILE__,__FUNCTION__);
			return false;
			
		}
		sscanf(record,"%d %d %d %d %d %s %s %d",
						&tempAlert->Specific,
						&tempAlert->address,
						&tempAlert->slot,
						&tempAlert->flag,
						&tempAlert->port,
						sBeginTime,
						sEndTime,
						&tempAlert->value);
		if(strcmp(sBeginTime,"0")){
			sBeginTime[10]=' ';
			tempAlert->begintime = str2time_t(sBeginTime);
		}else{
			tempAlert->begintime = 0;
		}
		if(strcmp(sEndTime,"0")){
			sEndTime[10]=' ';
			tempAlert->endtime = str2time_t(sEndTime);
		}else{
			tempAlert->endtime = 0;
		}
		addAlertTolinks(head,tempAlert);
	}	
	fclose(alertfile);
	////////////////////////////////////////////////
	sortAlerts(head);
	//
	return head; 
	
}
