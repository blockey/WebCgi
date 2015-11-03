/*
 * userServices.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef USERSERVICES_H_
#define USERSERVICES_H_
#include "../base/base.h"
#include "../model/userModel.h"
/*
 * 功能，保存新用户
 * @ user :in 用户的相关信息,包括username,password,adminId ,
 *               以"name1=value1;name2=value2"形式
 * 说明:用户类别:1,最高管理员;2,系统管理员;3,普通管理员.
 *		 有效标志:0无效;1有效
 *     新添加的用户的类别均为普通管理员,有效
 *     注册之后在由管理员修改
 */
boolean saveUserService(struct User *user);
/*
*功能:更新用户信息
*userInfo :in 以"name1=value1;name2=value2"形式
*return ,success return true,else return false
*/
boolean updateUserService(char *userInfo);
/*
*功能:根据用户发送过来的用户ID删除用户
*@ params :in	主要是用户ID信息
*操作成功返回真,否则返回假
*/
boolean deleteUsersService(char *params);
/*
*功能:根据用户ID激活用户
*@ params :in
*操作成功返回真,否则返回假
*/
boolean activeUsersService(char *params);
/*
*功能:通过userId或username获取用户信息
*@user :in out
*success return true,else return false
*说明:只能用于userId或username的情况,如果userId 和username均存在,则
*	  只匹配userId
*/
boolean getUserService(struct User *user);
/*
*功能：查询用户
*@params	:in  浏览器端传递过来的参数
*@actName	:in  请求的操作
*@result	:out 查询结果
*查询成功返回真,否则返回假
*/
boolean queryUsersService(char *params,char *actName,char *result);
/*
*功能:确定该用户名是否存在,唯一性检查
*@ user :in 
* return 如果该用户名已存在,返回真
*/
boolean isUserExist(struct User *user);
/*
 * 功能：验证登录信息
 * @ username :in
 * @ password :in
 * return 和数据库内容匹配，则返回真，否则返回假
 * 注销用户不能登录
 */
boolean loginValid(char *username,char *password);
/*
*功能:更新登录信息,包括登录IP和登录时间
*user :in out
*loginIp :in
*sucess return true,else return false
*/
boolean updateLoginInfo(struct User *user,char *loginIp);
/*
*功能:获取用户信息
*@params	:in		请求参数
*@actName   :in		请求动作
*@result    :out	输出结果
*操作成功返回真,否则返回假
*/
boolean getUserInfoServices(char *params,char *actName,char *result);
/*
*功能:用户信息服务
*@params   :in     请求参数
*@actName  :in	   请求动作
*@result   :out    输出结果
*操作成功返回真,否则返回假
*/
boolean userServices(char *params,char *actName,char *result);
#endif /* USERSERVICES_H_ */
