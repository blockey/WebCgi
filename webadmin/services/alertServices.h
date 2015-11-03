/*
 * alertServices.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef ALERTSERVICES_H_
#define ALERTSERVICES_H_
#include "../base/base.h"
#include "../model/alertModel.h"

/*
*功能:告警信息服务
*@params  :in 请求参数
*@actName :in 请求动作
*@result  :out 输出结果
*操作成功,返回值,反之返回假
*/
boolean alertServices(char *params,char *actName,char *result);
#endif /* ALERTSERVICES_H_ */
