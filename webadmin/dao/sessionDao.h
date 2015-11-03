/*
 * sessionDao.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SESSIONDAO_H_
#define SESSIONDAO_H_
#include "../base/base.h"
#include "../model/sessionModel.h"
/*
 * 功能，保存会话信息
 * @ session :in     要保存的会话数据结构
 * 如果保存成功,返回真,反之返回假
 */
boolean saveSessionDao(struct Session *session);
/*
*功能:更新session
*session :in
*return ,success return true,else return false
*/
boolean updateSessionDao(struct Session *session);
/*
*功能:删除会话
*@ session :in 要删除的会话
*删除成功,返回值,否则返回假
*说明,会话数据结构,只需有会话ID值即可
*/
boolean deleteSessionDao(struct Session *session);

/*
*功能:获取会话记录信息
*@session :in out	会话信息必须有sessonId或username之一,否则报错
*找到记录,则返回真,否则返回假.
*说明:如果sessionId username同时存在,则只匹配sessionId
*/
boolean getSessionDao(struct Session *session);
/*
*功能：根据数据结构中设置的数据作为条件查询符合条件的会话
*@condition :in　在这里设置要查询会话的条件
*@list      :out  输出会话结果链表
*查询成功,则返回真,否则返回假
*/
boolean querySessionsDao(struct Session *condition,struct Sessions **list);

#endif /* SESSIONDAO_H_ */