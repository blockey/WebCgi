/*
*userModel.h
*用户信息数据结构定义
*/
#ifndef USERMODEL_H_
#define USERMODEL_H_
//用户信息
struct User{
	long userId;          //用户ID
	long adminId;         //管理员ID,也就是操作添加该用户的管理员
	char username[40];    //用户名
	char password[50];    //密码
	char category[2];     //权限
	char isValid[2];      //有效性
	long registTime;      //注册时间
	long lastLoginTime;   //上次登录时间
	char lastLoginIp[20]; //上次登录IP
	long loginTime;       //最新登录时间
	char loginIp[20];     //最新登录IP
	char remask[300];     //备注
};
/*
*用户信息链表定义
*/
struct Users{
	struct User *user;    //用户信息指针
	struct Users *next;   //下一个用户信息指针
};

#endif /* USERMODEL_H_ */
