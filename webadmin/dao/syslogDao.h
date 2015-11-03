/*
 * syslogDao.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SYSLOGDAO_H_
#define SYSLOGDAO_H_
#include "../base/base.h"
#include "../model/syslogModel.h"
/*
*功能:根据类型选择日志文件
*@category :in 类别
*return 
*说明:调用Dao之前必需先选择日志文件
*/
boolean setLogFile(char *category);
/*
 * 功能，保存日志
 * @ syslog :in
 * 保存成功,返回值,反之返回假
 */
boolean saveSyslogDao(struct Syslog *syslog);
/*
*功能:删除日志
*@ logIds :in 日志的ID集
*操作成功,返回真,反之返回假
*注:根据需求,此函数已不再使用
*/
boolean deleteSyslogsDao(char *logIds);
/*
*功能：根据数据结构中设置的数据作为条件查询符合条件的日志
*@condition :in　   查询条件
*@page      :in out 分页信息
*@tr        :in		时间范围
*@list      :out	输出结果
*查询成功返回真,否则返回假
*/
boolean querySyslogsDao(struct Syslog *condition,struct Page *page,struct TimeRange *tr,struct Syslogs **list);

#endif /* SYSLOGDAO_H_ */