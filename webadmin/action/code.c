/*
*code.c
*对表单数据进行编码,将字符串以GB2312的编码输出
*/
#include <stdlib.h>
#include "../lib/cgic.h"
#include <string.h>
#include <stdio.h>
/*
*获取字符串的编码
*@str 要转换的字符串
*对每一个字符进行转换,转换后以 "23/78/96/等形式输出"
*/
void getCode(char *str){
	int i;
	int length = strlen(str);
	char outData[300];      //存储所有字符的编码,用于输出
	char value[5];          //中间值
	strcpy(outData,"");
	for(i = 0;i<length;i++){
		sprintf(value,"%d/",str[i]);    //单个字符的编码
		strcat(outData,value);
	}
	strcpy(str,outData);    //覆盖输入字符串,作为输出结果
}
int cgiMain(){
	char parameterValue[200];      //表单的文本框的数据   
	char parameterName[20];        //表单文本框名字
	int i;
	cgiHeaderContentType("text/html; charset=gb2312");
	printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"> \n");
	printf("<html xmlns=\"http://www.w3.org/1999/xhtml\"> \n");
	printf("<head> \n");
	printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" /> \n");
	printf("<title></title> \n");
	printf("</head> \n");
	printf("<body> \n");
	printf("<input type=\"hidden\" id=\"isReturn\" value=\"true\" /> \n");
	printf("<form action=\"/cgi-bin/webadmin/code.cgi\" method=\"post\" name=\"form1\" id=\"form1\"> \n");
	for(i = 1;i<16;i++){
		sprintf(parameterName,"remote%d",i);
		cgiFormString(parameterName,parameterValue,200);
		if(strlen(parameterValue) != 0){
			//编码
			getCode(parameterValue);
			//将编码结果覆盖原来数据作为输出到页面中
			printf("<input type=\"text\" name=\"%s\" id=\"%s\" value=\"%s\"  /> \n",parameterName,parameterName,parameterValue);
		}
		//只要有一项没数据,则直接跳出
		else{
			break;
		}
	}
	printf("<input type=\"submit\" name=\"submit\" id= \"submit\" value=\"submit\" /> \n");
	printf("</form> \n");
	printf("</body> \n");
	printf("</html> \n");
	return 0;
}

