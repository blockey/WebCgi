/*
*code.c
*�Ա����ݽ��б���,���ַ�����GB2312�ı������
*/
#include <stdlib.h>
#include "../lib/cgic.h"
#include <string.h>
#include <stdio.h>
/*
*��ȡ�ַ����ı���
*@str Ҫת�����ַ���
*��ÿһ���ַ�����ת��,ת������ "23/78/96/����ʽ���"
*/
void getCode(char *str){
	int i;
	int length = strlen(str);
	char outData[300];      //�洢�����ַ��ı���,�������
	char value[5];          //�м�ֵ
	strcpy(outData,"");
	for(i = 0;i<length;i++){
		sprintf(value,"%d/",str[i]);    //�����ַ��ı���
		strcat(outData,value);
	}
	strcpy(str,outData);    //���������ַ���,��Ϊ������
}
int cgiMain(){
	char parameterValue[200];      //�����ı��������   
	char parameterName[20];        //���ı�������
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
			//����
			getCode(parameterValue);
			//������������ԭ��������Ϊ�����ҳ����
			printf("<input type=\"text\" name=\"%s\" id=\"%s\" value=\"%s\"  /> \n",parameterName,parameterName,parameterValue);
		}
		//ֻҪ��һ��û����,��ֱ������
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

