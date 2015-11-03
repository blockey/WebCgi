/*
 * pageUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef PAGEUTIL_H_
#define PAGEUTIL_H_
#include "../base/base.h"
/*
*功能:根据当前页计算记录的开始和结束索引
*@page :in out
*return 
*/
void calculatePage(struct Page *page);
/*
*功能:清空Page的数据
*@ page :in out
*return 
*/
void resetPage(struct Page *page);
/*
*功能:根据字符参数设置Page
*@str :in
*@page :out
*return
*/
void setPageByString(struct Page *page,char *str);

#endif /* PAGEUTIL_H_ */
