/*
 * userUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef USERUTIL_H_
#define USERUTIL_H_
#include "../model/userModel.h"
/*
*功能：重置User数据
*@ user :in out
*return
*/
void resetUser(struct User *user);
/*
*功能：释放链表内存
*users :in out
*return 
*/
void releaseUsersMemory(struct Users *users);
/*
*功能:根据字符串参数设置User
*@user :out
*@str :in 字符串参数
*return 
*/
void setUserByString(struct User *user,char *str);
#endif /* USERUTIL_H_ */