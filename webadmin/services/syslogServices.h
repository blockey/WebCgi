/*
 * syslogServices.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SYSLOGSERVICES_H_
#define SYSLOGSERVICES_H_
#include "../base/base.h"
#include "../model/syslogModel.h"
/*
 * 功能，保存日志
 * @ category :in
 * @ actName :in
 * @ username :in
 * @ loginIp :in
 * @ isSuccess :in
 * 保存成功返回真,否则返回假
 */
boolean saveSyslogService(char *category,char *actName,char *username,char *loginIp,char *isSuccess);

/*
*功能:注消用户
*@ userId :in
*return 操作结果
*/
boolean deleteSyslogsService(char *category,char *params);
/*
*功能:针对用户的操作都调用这个函数进行处理
*@category :in	日志类别
*@username :in  用户名
*@params :in    请求参数
*@actName :in   请求动作
*@result :out   处理结果输出
*操作成功返回真,否则返回假
*
*/
boolean syslogServices(char *category,char *username,char *params,char *actName,char *result);
#endif /* SYSLOGSERVICES_H_ */
