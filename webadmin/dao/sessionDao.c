/*
*sessionDao.c
*会话DAO
*/
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "sessionDao.h"
#include "../model/sessionModel.h"
#include "../util/sessionUtil.h"
#include "../base/base.h"
#include "../util/adstring.h"
#include "../util/lockFile.h"

/*
 * 功能，保存会话信息
 * @ session :in     要保存的会话数据结构
 * 如果保存成功,返回真,反之返回假
 */
boolean saveSessionDao(struct Session *session){
	FILE *sessionfile = NULL; //会话文件 
	//sessionfile = fopen(sessionFilePath,"a");
	lock_t lock = lck_open(sessionFilePath,"a",&sessionfile);
	//lock file error 
	if(lock < 0){	//this file is already lock by other process 
	//if(sessionfile == NULL){
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,sessionFilePath);
		return false  ;
	}
	//追加记录
	fprintf(sessionfile,"sessionId=%ld;username=%s;acessTime=%ld;loginIp=%s; \n",
						session->sessionId,
						session->username,
						session->acessTime,
						session->loginIp);
	//关闭文件
	//fclose(sessionfile);
	lck_close(lock,sessionfile);
	return true;
}
/*
*功能:更新session
*session :in
*return ,success return true,else return false
*/
boolean updateSessionDao(struct Session *session){
	/******************************************
	 *删除行
	 *******************************************/
	FILE *sessionfile = NULL; //会话文件
	FILE *tempfile = NULL;    //临时文件 
	//sessionfile = fopen(sessionFilePath,"r");
	//tempfile = fopen(tempFilePath,"w");
	lock_t lock_session = lck_open(sessionFilePath,"r",&sessionfile);
	lock_t lock_temp = lck_open(tempFilePath,"w",&tempfile);
	//打开文件出错
	//lock file error 
	if(lock_session < 0){	//this file is already lock by other process 
	//if(sessionfile == NULL){
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,sessionFilePath);
		return false  ;
	}
	
	if(lock_temp < 0 ){	//this file is already lock by other process 
	//if(tempfile == NULL){
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,tempFilePath);
		return false  ;
	}
	
	char row[200];            //文件的每一行数据
	char value[200];          //中间变量 
	//读取文件
	while(fgets(row,200,sessionfile)){
		//获取sessionId字段
		if(getValueByName(row,"sessionId",value)){
			//要修改的记录
			if(session->sessionId == atol(value)){
				sprintf(value,
						"sessionId=%ld;username=%s;acessTime=%ld;loginIp=%s;\n",
						session->sessionId,
						session->username,
						session->acessTime,
						session->loginIp);
				//将修改后的记录写到临时文件中
				fputs(value,tempfile);
			}
			//非要修改的行,直接写到临时文件中
			else{
				fputs(row,tempfile);
			}
		}
		//行内容有错，找不到sessionId字段,采取跳过策略
		else{
			continue;
		}
	}
	//关闭文件
	//fclose(sessionfile);
	//fclose(tempfile);
	lck_close(lock_session,sessionfile);
	lck_close(lock_temp,tempfile);

	/******************************************
	 *将修改过的内容覆盖原文件
	 *******************************************/
	char command[100];            //shell command 
	sprintf(command,"cp -f %s %s",tempFilePath,sessionFilePath);
	//加锁并打开文件 
	lock_session = lck_open(sessionFilePath,"a",&sessionfile);
	lock_temp = lck_open(tempFilePath,"a",&tempfile);
	system(command);
	//释放锁并关闭文件
	lck_close(lock_session,sessionfile);
	lck_close(lock_temp,tempfile);
	return true;
}
/*
*功能:删除会话
*@ session :in 要删除的会话
*删除成功,返回值,否则返回假
*说明,会话数据结构,只需有会话ID值即可
*/
boolean deleteSessionDao(struct Session *session){
	if(session->sessionId == 0){
		webLog("file=%s;func=%s;msg=session Id is null;",__FILE__,__FUNCTION__);
		return false;
	}
	/*
	*删除办法:
	*将不是要删除的会话和未过期的会话记录
	*写到临时文件中,而要删除的会话和已过期
	*的会话,则不写到临时文件中
	*最后将临时文件的内容替代会话议会的内容	
	*/
	/******************************************
	 *删除行
	 *******************************************/
	//打开文件
	FILE *sessionfile = NULL;
	FILE *tempfile = NULL;
	//sessionfile = fopen(sessionFilePath,"r");
	//tempfile = fopen(tempFilePath,"w");
	lock_t lock_session = lck_open(sessionFilePath,"r",&sessionfile);
	lock_t lock_temp = lck_open(tempFilePath,"w",&tempfile);
	//打开文件出错
	//lock file error 
	if(lock_session < 0 ){	//this file is already lock by other process 
	//if(sessionfile == NULL){
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,sessionFilePath);
		return false  ;
	}
	if(lock_temp < 0){	//this file is already lock by other process 
	//if(tempfile == NULL){
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,tempFilePath);
		return false  ;
	}
	
	char value[200];      //中间变量 
	char row[200];        //记录
	//读取文件内容
	while(fgets(row,200,sessionfile)){
		//读取SessionId字段
		if(getValueByName(row,"sessionId",value)){
			//不是要删除的会话
			if(session->sessionId != atol(value)){
				//最后访问时间
				getValueByName(row,"acessTime",value);
				//会话未过期,则写到临时文件中
				if(time(NULL) - atol(value) < sessionExpireTime){
					fputs(row,tempfile);
				}
			}
		}
		//内容出错，找不到sessionId字段,则直接跳过该行
		else{
			continue;
		}
	}
	//关闭文件
	//fclose(sessionfile);
	//fclose(tempfile);
	lck_close(lock_session,sessionfile);
	lck_close(lock_temp,tempfile);
	/******************************************
	 *将修改过的内容覆盖原文件
	 *******************************************/
	char command[100];            //shell command 
	sprintf(command,"cp -f %s %s",tempFilePath,sessionFilePath);
	//锁定文件 
	lock_session = lck_open(sessionFilePath,"a",&sessionfile);
	lock_temp = lck_open(tempFilePath,"a",&tempfile);
	system(command);
	//关闭文件
	lck_close(lock_session,sessionfile);
	lck_close(lock_temp,tempfile);
	return true;
}
/*
*功能:获取会话记录信息
*@session :in out	会话信息必须有sessonId或username之一,否则报错
*找到记录,则返回真,否则返回假.
*说明:如果sessionId username同时存在,则只匹配sessionId
*/
boolean getSessionDao(struct Session *session){
	//打开会话文件
	FILE *sessionfile =NULL ;
	//sessionfile = fopen(sessionFilePath,"r");
	lock_t lock_session = lck_open(sessionFilePath,"r",&sessionfile);
	//打开文件出错
	//lock file error 
	if(lock_session < 0 ){	//this file is already lock by other process 
	//if(sessionfile == NULL){
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,sessionFilePath);
		return false  ;
	}
	char row[200];				//记录
	char value[200];			//中间变量 
	boolean isGet = false;		//找到记录标志
	//读取文件
	while(fgets(row,200,sessionfile)){
		//提供会话ID作为查询条件
		if(session->sessionId != 0){
			getValueByName(row,"sessionId",value);	
			if(session->sessionId == atol(value)){
				isGet = true;
			}
		}
		//提供用户名作为查询条件
		if(session->username != NULL){
			getValueByName(row,"username",value);	
			if(!strcmp(session->username,value)){
				isGet = true;
			}
		}
		//找到记录
		if(isGet){
			//根据记录内容,设置会话数据结构的各个成员变量 
			getValueByName(row,"sessionId",value);
			session->sessionId = atol(value);
			getValueByName(row,"username",session->username);
			getValueByName(row,"acessTime",value);
			session->acessTime = atol(value);
			getValueByName(row,"loginIp",session->loginIp);
			//退出循环
			break;
  		}
		
	}
	//关闭文件
	lck_close(lock_session,sessionfile);
	//fclose(sessionfile);
	return isGet;
}
/*
*功能：根据数据结构中设置的数据作为条件查询符合条件的会话
*@condition :in　在这里设置要查询会话的条件
*@list      :out  输出会话结果链表
*查询成功,则返回真,否则返回假
*/
boolean querySessionsDao(struct Session *condition,struct Sessions **list){
	//打开会话文件
	FILE *sessionfile =NULL ;
	//sessionfile = fopen(sessionFilePath,"r");
	lock_t lock_session = lck_open(sessionFilePath,"r",&sessionfile);
	//打开文件出错
	//lock file error 
	if(lock_session < 0 ){	//this file is already lock by other process 
	//if(sessionfile == NULL){
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,sessionFilePath);
		return false  ;
	}
	char row[200];					//记录
	char value[50];					//中间变量 
	struct Sessions *sessions;		//保存结果的会话链表
	struct Session *session ;		//会话信息指针
	//为链表分配头指针
	struct Sessions *p = (struct Sessions *)malloc(sizeof(struct Sessions));
	//初始化
	p->next = NULL;
	*list = p;
	p->session = NULL;
	p->next = NULL;
	
	//读取文件内容
	while(fgets(row,200,sessionfile)){
		/*
		*对各个条件进行逐一匹配,一旦有不符合条件
		*的就跳出本次循环,进入下轮循环
		*/
		//会话ID
		if(condition->sessionId != 0){
			getValueByName(row,"sessionId",value);
			if(condition->sessionId != atol(value)){
				continue;
			}
		}
		//用户名
		if(condition->username[0] != 0){
			getValueByName(row,"username",value);
			//实现模糊查询
			if(!strstr(value,condition->username)){
				continue;
			}	
		}
		//访问时间
		if(condition->acessTime != 0){
			getValueByName(row,"acessTime",value);
			if(condition->acessTime != atol(value)){
				continue;
			}	
		}
		//登录IP
		if(condition->loginIp[0] != 0){
			getValueByName(row,"loginIp",value);
			if(!strstr(value,condition->loginIp)){
				continue;
			}	
		}
		/*
		*符合所有条件,将当前记录添加到输出结果链表中
		*/
		//为当前记录创建结点,并分配空间
		sessions = (struct Sessions *)malloc(sizeof(struct Sessions));
		//将结点加入到链表中
		p->next = sessions;
		//创建会话,存放记录信息
		session = (struct Session*)malloc(sizeof(struct Session));
		//重置空间内容
		resetSession(session);
		/*
		*根据记录的各字段信息相应赋给会话信息数据结构
		*/
		//会话ID
		getValueByName(row,"sessionId",value);
		session->sessionId = atol(value);
		//用户名
		getValueByName(row,"username",session->username);
		//访问时间
		getValueByName(row,"acessTime",value);
		session->acessTime = atol(value);
		//登录IP
		getValueByName(row,"loginIp",session->loginIp);
		//将该会话赋于当前结点的会话指针
		sessions->session=session;
		//将当前结点的下一结点设置为空
		sessions->next = NULL;
		//将p指针指向当前结点
		p = sessions;
	}
	//关闭文件
	//fclose(sessionfile);
	lck_close(lock_session,sessionfile);
	return true;
}



