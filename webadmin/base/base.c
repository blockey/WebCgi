/*
 * base.c
 *
 *  Created on: 26-12月-08
 *      Author: root
 */
#include "base.h"
#include "../util/adstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "../util/lockFile.h"



/*
*功能：调试程序输出
*@ msg :in
*return null
*/

void webLog(const char *fmt, ...){
#ifdef LOG_
	char timestr[50];
	char msg[1024*10] = {0};				//出错信息 
	va_list va;					//可变参数列表 
	va_start(va, fmt);
	vsprintf(msg, fmt, va);	//设置出错信息 
	va_end(va);
	time_t timep;				//当前时间 
	time (&timep);				//获取当前时间 
	long2TimeStr(timep,timestr);  //将时间格式化 
	//FILE *logfile = NULL;			//日志文件 
	FILE *logfile = fopen(logFilePath,"a");
	//lock_t lock = lck_open(logFilePath,"a",&logfile);
	
	fprintf(logfile,"time=%s;process=webadmin;msg=%s\n",timestr,msg);	//将出错信息写进日志文件中 
	
	//lck_close(lock,logfile);
	fclose(logfile);
#endif
}


/***********************************************************
this function is use to format conversion 
for example,
"05 19 04 4B 41 00 5A 0A 00 00 00 00 00 00 00 00 00 40 00 00 00 01 00 0
0 00 01 3C"
to 
"0519044B41005A0A0000000000000000004000000001000000013C"
***************************************************************/
char *trimEx(char *s)
{
	int i;
	int j = 0;
	
	if (s == NULL)
	{
		return NULL;
	}
	int len = (int)strlen(s);	
	
	char *p = (char *)malloc(sizeof(char)*(len+1));
	memset(p, 0, len+1);
	
	for (i = 0; i < len; i++)
	{
		if (s[i] == ' ' )		//ignored at the beginning of the space-���Կ�ͷ�ո�
		{
			continue;
		}
		p[j] = s[i];
		j++;
	}
	
	if (p[j - 1] == ' ')
	{
		p[j - 1] = '\0';
	}
	else
	{
		p[j] = '\0';
	}
	
	strcpy(s, p);
    free(p);
	return s;
}


/***********************************************************
this function is use to format conversion 
for example,
"0519044B41005A0A0000000000000000004000000001000000013C"  
to 
"05 19 04 4B 41 00 5A 0A 00 00 00 00 00 00 00 00 00 40 00 00 00 01 00 0
0 00 01 3C"
***************************************************************/
char *AddBlank(char *src, char *dst)
{
	int i;
	int j = 0;
	
	if (src == NULL || dst == NULL)
	{
		return NULL;
	}
	int len = (int)strlen(src);	
	
	char *p = (char *)malloc(sizeof(char)*(len+len/2+2));
	memset(p, 0, len+len/2+2);
	for (i = 0; i < len; i++)
	{

		p[j] = src[i];
		j++;

		if ((i+1)%2 == 0 )		//ignored at the beginning of the space-���Կ�ͷ�ո�
		{
			p[j] = ' ';      //added a blank every two char
			j++;
		}	
	}
	
	
	if (p[j - 1] == ' ')
	{
		p[j - 1] = '\0';
	}
	else
	{
		p[j] = '\0';
	}

	if(len%2 != 0)
	{//even digital
		strcat(p,"0");
	}
	
	strcpy(dst, p);
    free(p);
	return dst;
}


