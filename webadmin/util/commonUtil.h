/*
 * commonUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef COMMONUTIL_H_
#define COMMONUTIL_H_
#include "../base/base.h"
/*
*功能:根据请求参数设置时间查询范围
*@tr	:out
*@str	:in
*return 
*/
void setTimeRangByString(struct TimeRange *tr,char *str);

#endif /* COMMONUTIL_H_ */

