#include "pageUtil.h"
#include "../base/base.h"
#include <stdlib.h>
/*
*功能:根据当前页计算记录的开始和结束索引
*@page :in out
*return 
*/
void calculatePage(struct Page *page){
	page->start = (page->current-1) * page->pageSize + 1;
	page->end = page->current * page->pageSize  ;
}
/*
*功能:清空Page的数据
*@ page :in out
*return 
*/
void resetPage(struct Page *page){
	//当前页
	page->current = 0;
	//首页
	page->start = 0;
	//尾页
	page->end = 0;
	//总共记录数
	page->total = 0;
	//每页大小
	page->pageSize = 0;
}
/*
*功能:根据字符参数设置Page
*@str :in
*@page :out
*return
*/
void setPageByString(struct Page *page,char *str){
	char value[20];		//中间变量 
	//当前页
	if(getValueByName(str,"current",value)){
		page->current = atol(value);
	}
	//每页大小
	if(getValueByName(str,"pageSize",value)){
	page->pageSize = atol(value);
	}
}
