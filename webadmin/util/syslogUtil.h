/*
 * syslogUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SYSLOGUTIL_H_
#define SYSLOGUTIL_H_
#include "../model/syslogModel.h"
#include "../base/base.h"
/*
*功能：重置User数据
*@ user :in out
*return
*/
void resetSyslog(struct Syslog *syslog);
/*
*功能：释放链表内存
*users :in out
*return 
*/
void releaseSyslogsMemory(struct Syslogs *syslogs);
/*
*功能:根据字符串参数设置User
*@user :out
*@str :in 字符串参数
*return 
*/
void setSyslogByString(struct Syslog *syslog,char *str);

#endif /* SYSLOGUTIL_H_ */