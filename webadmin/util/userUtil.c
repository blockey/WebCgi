#include <stdlib.h>
#include "../model/userModel.h"
#include "userUtil.h"
/*
*功能：重置User数据
*@ user :in out
*return
*/
void resetUser(struct User *user){
	//用户ID
	user->userId=0;
	//管理员ID
	user->adminId=0;
	//用户权限
	user->category[0]=0;
	//是否有效
	user->isValid[0]=0;
	//用户密码
	user->password[0]= 0;
	//注册时间
	user->registTime=0;
	//用户名
	user->username[0]=0;
	//上次登录时间
	user->lastLoginTime=0;
	//上次登录IP
	user->lastLoginIp[0]=0;
	//最近登录时间
	user->loginTime = 0;
	//最近登录IP
	user->loginIp[0]= 0;
	//备注
	user->remask[0] = 0;
}
/*
*功能:根据请求参数设置User
*@user :out
*@str :in 字符串参数
*return 
*/
void setUserByString(struct User *user,char *str){
	char value[300];	//中间变量 
	//用户ID
	if(getValueByName(str,"userId",value)){
		user->userId = atol(value);
	}
	//管理员ID
	if(getValueByName(str,"adminId",value)){
	user->adminId = atol(value);
	}
	//用户名
	getValueByName(str,"username",user->username);
	//密码
	getValueByName(str,"password",user->password);
	//权限
	getValueByName(str,"category",user->category);
	//是否有效
	getValueByName(str,"isValid",user->isValid);
	//注册时间
	if(getValueByName(str,"registTime",value)){
		user->registTime = atol(value);
	}
	//上次登录时间
	if(getValueByName(str,"lastLoginTime",value)){
		user->lastLoginTime = atol(value);
	}
	//上次登录IP
	getValueByName(str,"lastLoginIp",user->lastLoginIp);
	//最近登录时间
	if(getValueByName(str,"loginTime",value)){
		user->loginTime = atol(value);
	}
	//最近登录IP
	getValueByName(str,"loginIp",user->loginIp);
	//备注
	getValueByName(str,"remask",user->remask);
}
/*
*功能：释放链表内存
*users :in out
*return 
*/
void releaseUsersMemory(struct Users *users){
	struct Users *p;	//链表指针
	//逐个删除结点
	while(users){
		free(users->user);
		p = users;
		users = users->next;
		free(p);
	}
}

