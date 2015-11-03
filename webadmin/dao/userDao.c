/*
*userDao.c
*用户DAO
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../base/base.h"
#include "userDao.h"
#include "../model/userModel.h"
#include "../util/userUtil.h"
#include "../util/adstring.h"
#include "../util/lockFile.h"

/*
*功能:更新用户信息
*user :in out
*操作成功返回真,否则返回假
*/
boolean updateUserDao(struct User *user){
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
	FILE *userfile = NULL;
	FILE *tempfile = NULL;
	lock_t lock_user = lck_open(userFilePath,"r",&userfile);
	lock_t lock_temp = lck_open(tempFilePath,"w",&tempfile);
	//打开文件出错
	//lock file error 
	if(lock_user == FILE_BUSY){	//this file is already lock by other process 
		webLog("file=%s;func=%s;msg=file %s has been locked;",__FILE__,__FUNCTION__,userFilePath);
		return false  ;
	}
	if(lock_user == ERROR){	//lock file error ,perhaps the path is not exist 
		webLog("file=%s;func=%s;msg=%s path error;",__FILE__,__FUNCTION__,userFilePath);
		return false;
	}
	if(lock_temp == FILE_BUSY){	//this file is already lock by other process 
		webLog("file=%s;func=%s;msg=file %s has been locked;",__FILE__,__FUNCTION__,tempFilePath);
		return false  ;
	}
	if(lock_temp == ERROR){	//lock file error ,perhaps the path is not exist 
		webLog("file=%s;func=%s;msg=%s path error;",__FILE__,__FUNCTION__,tempFilePath);
		return false;
	}
	char row[500];			//记录
	char value[500];		//中间变量
	//读文件
	while(fgets(row,500,userfile)){
		//获取userId字段内容
		if(getValueByName(row,"userId",value)){
			//要修改的用户记录
			if(user->userId == atol(value)){
				//用户名
				if(user->username[0] == NULL){
					getValueByName(row,"username",user->username);
				}
				//密码
				if(user->password[0] == NULL){
					getValueByName(row,"password",user->password);
				}
				//用户权限
				if(user->category[0] == NULL){
					getValueByName(row,"category",user->category);
				}
				//是否有效
				if(user->isValid[0] == NULL){
					getValueByName(row,"isValid",user->isValid);
				}
				//注册时间			
				if(user->registTime == 0){
					getValueByName(row,"registTime",value);
					user->registTime = atol(value);
				}
				//上次登录时间
				if(user->lastLoginTime == 0){
					getValueByName(row,"lastLoginTime",value);
					user->lastLoginTime = atol(value);
				}
				//上次登录IP
				if(user->lastLoginIp[0] == NULL){
					getValueByName(row,"lastLoginIp",user->lastLoginIp);
				}
				//最新登录时间
				if(user->loginTime == 0){
					getValueByName(row,"loginTime",value);
					user->loginTime = atol(value);
				}
				//最新登录IP
				if(user->loginIp[0] == NULL){
					getValueByName(row,"loginIp",user->loginIp);
				}
				//备注
				if(user->remask[0] == NULL){
					getValueByName(row,"remask",user->remask);
				}
				//管理员ID
				if(user->adminId == 0){
					getValueByName(row,"adminId",value);
					user->adminId = atol(value);
				}
				//设置修改后的记录
				sprintf(value,
						"userId=%ld;username=%s;password=%s;category=%s;adminId=%ld;isValid=%s;registTime=%ld;lastLoginTime=%ld;lastLoginIp=%s;loginTime=%ld;loginIp=%s;remask=%s;\n",
						user->userId,
						user->username,
						user->password,
						user->category,
						user->adminId,
						user->isValid,
						user->registTime,
						user->lastLoginTime,
						user->lastLoginIp,
						user->loginTime,
						user->loginIp,
						user->remask);
				//将修改后的记录写入临时文件
				fputs(value,tempfile);
			}
			//非要修改的记录,直接写入临时文件
			else{
				fputs(row,tempfile);
			}
		}
		//行内容有错，采取跳过策略
		else{
			continue;
		}
	}
	//关闭文件
	lck_close(lock_user,userfile);
	lck_close(lock_temp,tempfile);
	/******************************************
	 *将修改过的内容覆盖原文件
	 *******************************************/
	 char command[100];            //shell command 
	sprintf(command,"cp -f %s %s",tempFilePath,userFilePath);
	//锁定文件 
	lock_user = lck_open(userFilePath,"a",&userfile);
	lock_temp = lck_open(tempFilePath,"a",&tempfile);
	system(command);
	//关闭文件
	lck_close(lock_user,userfile);
	lck_close(lock_temp,tempfile);
	return true;
}

/*
*功能:通过userId或username获取用户各字段的值
*@user :in out
*success return true,else return false
*说明:只能用于userId或username的情况,如果userId 和username均存在,则
*	  只匹配userId
*/
boolean getUserDao(struct User *user){
	if(user->username[0] == NULL && user->userId == 0){
	
		//printf("username and userId are both null\n");
		
		webLog("file=%s;func=%s;msg=username and userId are both null;",__FILE__,__FUNCTION__);
		return false;
	}
	//打开文件
	FILE *userfile = NULL;
	lock_t lock = lck_open(userFilePath,"r",&userfile);
	//lock file error 
	if(lock < 0  ){	//this file is already lock by other process 
		webLog("file=%s;func=%s;msg=file %s has been locked ;",__FILE__,__FUNCTION__,userFilePath);
		return false  ;
	}
	char row[500];				//记录
	char value[500];			//中间变量 
	boolean isGet = false;		//操作结果返回标志
	//读取文件内容
	while(fgets(row,500,userfile)){
		//如果userId不为空
		if(user->userId != 0){
			getValueByName(row,"userId",value);
			if(user->userId == atol(value)){
				isGet = true;
			}
		}
		//如果username不为空
		if(user->username[0] != NULL){
			getValueByName(row,"username",value);
			if(!strcmp(user->username,value)){
				isGet = true;
			}
		}
		//获取符合条件的记录
		if(isGet){
			/*
			*将记录的内容设置到数据结构中
			*然后将新内容写入临时文件
			*/
			//管理员ID
			getValueByName(row,"adminId",value);
			user->adminId = atol(value);
			//用户ID
			getValueByName(row,"userId",value);
			user->userId = atol(value);
			//用户名
			getValueByName(row,"username",user->username);
			//printf("%s\n",user->username);
			//密码
			getValueByName(row,"password",user->password);
			//权限
			getValueByName(row,"category",user->category);
			//是否有效
			getValueByName(row,"isValid",user->isValid);
			//注册时间
			getValueByName(row,"registTime",value);
			user->registTime = atol(value);
			//上次登录时间
			getValueByName(row,"lastLoginTime",value);
			user->lastLoginTime = atol(value);
			//上次登录IP
			getValueByName(row,"lastLoginIp",user->lastLoginIp);
			//登录时间
			getValueByName(row,"loginTime",value);
			user->loginTime = atol(value);
			//登录IP
			getValueByName(row,"loginIp",user->loginIp);
			//备注
			getValueByName(row,"remask",user->remask);
			break;
		}
		
	}
	//关闭文件
	lck_close(lock,userfile);
	return isGet;
}


