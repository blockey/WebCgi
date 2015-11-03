/*
*userServices.c
*用户服务接口
*/
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../base/base.h"
#include "userServices.h"
#include "../dao/userDao.h"
#include "../util/userUtil.h"
#include "../util/pageUtil.h"
#include "../model/userModel.h"
#include "../util/sysvar.h"


/*
*功能:通过userId或username获取用户信息
*@user :in out
*success return true,else return false
*说明:只能用于userId或username的情况,如果userId 和username均存在,则
*	  只匹配userId
*/
boolean getUserService(struct User *user){
	if(user->userId == 0 && user->username[0] == NULL){
		webLog("file=%s;func=%s;msg=userId or username is null ;",__FILE__,__FUNCTION__);
		return false;
	}
	if(getUserDao(user)){
		return true;
	}
	return false;
}


/*
*功能:确定该用户名是否存在,唯一性检查
*@ user :in 
* return 如果该用户名已存在,返回真
*/
boolean isUserExist(struct User *user){

	//调用DAO查询该用户
	//能查询出该用户
	if(getUserDao(user)){
		return true;
	}
	//数据库表中没此用户
	else{
		return false;
	}
}

/*
 * 功能：验证登录信息
 * @ username :in
 * @ password :in
 * return 和数据库内容匹配，则返回真，否则返回假
 * 注销用户不能登录
 */
boolean loginValid(char *username,char *password){
	if(username == NULL || password == NULL){
		webLog("file=%s;func=%s;msg=username or password is null;",__FILE__,__FUNCTION__);
		return false;
	}
	struct User userQuery;		//用户查询变量
	struct Users *users;		//用户链表
	//重置数据空间
	resetUser(&userQuery);		
	boolean isValid = false;	//登录信息是否有效标志
	//设置用户名
	strcpy(userQuery.username,username);
	/*调用DAO查询
	*因为用户名肯定是唯一的,
	*所以最多只可能存在一个符合条件的用户
	*/
	//找到该用户
	if(getUserDao(&userQuery)){
		//printf("user has\n");
		//该用户未被注消
		if(!strcmp(userQuery.isValid,"1") ){
			//printf("isValid\n");
			//判断密码是否正确
			if(!strcmp(password,userQuery.password)){
			//	printf("password right\n");
				isValid = true;
			}
		}
		
	}
	return isValid;
}
/*
*功能:更新登录信息,包括登录IP和登录时间
*user :in out
*loginIp :in
*sucess return true,else return false
*/
boolean updateLoginInfo(struct User *user,char *loginIp){
	//把上次登录时间设置为原最新登录时间
	user->lastLoginTime = user->loginTime;
	//把上次登录IP设置为原最新登录IP
	strcpy(user->lastLoginIp,user->loginIp);
	//设置最新登录时间为当前时间
	user->loginTime = time(NULL);
	//设置最新登录IP为当前用户登录IP
	strcpy(user->loginIp,loginIp);
	//调用DAO更新用户信息
	//更新成功
	if(updateUserDao(user)){
		return true;
	}
	//更新失败
	return false;
	
}


