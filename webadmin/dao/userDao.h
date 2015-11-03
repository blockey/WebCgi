/*
 * userDao.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef USERDAO_H_
#define USERDAO_H_
#include "../base/base.h"
#include "../model/userModel.h"

/*
 * 功能，保存新用户
 * @ user :in
 * sucess return true,else return false
 */
boolean saveUserDao(struct User *user);
/*
*功能:更新用户信息
*user :in out
*操作成功返回真,否则返回假
*/
boolean updateUserDao(struct User *user);
/*
*功能:注消用户
*@ userIds :in 要删除的用户ID集
*return 操作结果
*/
boolean deleteUsersDao(char *userIds);
/*
*功能:激活用户
*@ userIds :in 
*return 操作结果
*/
boolean activeUsersDao(char *userIds);
/*
*功能:通过userId或username获取用户各字段的值
*@user :in out
*success return true,else return false
*说明:只能用于userId或username的情况,如果userId 和username均存在,则
*	  只匹配userId
*/
boolean getUserDao(struct User *user);

/*
*功能：根据数据结构中设置的数据作为条件查询符合条件的用户
*@condition :in　在这里设置要查询用户的条件
*@page :in out 如果page = NULL ,则不分页
*@list :out
*success return true,else return false;
*/
boolean queryUsersDao(struct User *condition,struct Page *page,struct Users **list);

#endif /* USERDAO_H_ */