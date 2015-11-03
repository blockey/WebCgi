/*
 * sessionUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SESSIONUTIL_H_
#define SESSIONUTIL_H_
#include "../model/sessionModel.h"

/*
*功能：重置User数据
*@ user :in out
*return
*/
void resetSession(struct Session *session);
/*
*功能：释放链表内存
*users :in out
*return 
*/
void releaseSessionsMemory(struct Sessions *sessions);
#endif /* SESSIONUTIL_H_ */