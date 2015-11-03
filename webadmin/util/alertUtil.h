/*
 * alertUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef ALERTUTIL_H_
#define ALERTUTIL_H_
#include "../model/alertModel.h"
#include "../base/base.h"
/*
*功能：重置告警数据
*@ user :in out
*return
*/
void resetAlert(struct Alert *alert);
/*
*功能:根据请求参数设置告警信息
*@user	:out
*@str	:in 字符串参数
*return 
*/
void setAlertByString(struct Alert *alert,char *str);
/*
*功能：释放链表内存
*users :in out
*return 
*/
void releaseAlertsMemory(struct Alerts *alerts);
/*
*功能:根据告警类型设备相应描述
*alert
*@desc
*success return true,else return false
*/
boolean getAlertDesc(struct Alert *alert,char *desc);
#endif /* ALERTUTIL_H_ */