/*
 * adstring.c
 *
 *  Created on: 26-12月-08
 *      Author: root
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include <math.h>
#include <time.h>
#include "adstring.h"
#include "../base/base.h"
/*
   * 功能 找出字符c在字符串str中的位置
 * @str :in 字符串
 * @c   :in 指定的字符
 * @skip :in 要指定第几次出现该字符的位置 skip = 0表示第一次出现该字符的位置
 * return error 表示输入有误，none表示 没有找到，其他表示已经找到，并
   * 返回其位置
 */
int stringIndex(char *str,char c,int skip ){
	if (str == NULL){
		webLog("file=%s;func=%s;msg=the input string is null;",__FILE__,__FUNCTION__);
		return error;
	}
	int length = strlen(str);		//输入字符串长度
	int appareTimes = 0;			//该字符已出现次数
	int i ;							//计数
	for(i = 0 ; i<length; i++){
		//出现该字符
		if(str[i] == c){
			/*
			*如果返回第几次出现该字符的位置
			*/
			//第1次出现该字符的位置
			if(skip == 0){
				return i;
			}
			//第n次出现该字符的位置
			else{
				//已出现次数自增
				++appareTimes;			
				//如果还没到指定的次数
				if(appareTimes < skip + 1){
					continue;
				}
				//已出现指定次数
				else{
					return i;
				}
			}
		}
	}
	return none;
}
/*
 * 功能：提取子串
 * @ str     :in	父串
 * @ substr  :out	子串
 * @ begin   :in	开始位置
 * @ end     :in	结束位置
 * return 操作成功，返回true ,操作失败 ，返回false
 */
boolean substring(char *str,char *substr,int begin,int end){
	
	/*
	*对输入进行验证
	*/
	//输入字符串
	if(str == NULL || substr == NULL){
		webLog("file=%s;func=%s;msg=parent string or substring is null;",__FILE__,__FUNCTION__);
		return false;
	}
	//开始与结束位置
	if(begin<0 || end <0 ){
		webLog("file=%s;func=%s;msg=begin or end position is illegal ;",__FILE__,__FUNCTION__);
		return false;
	}
	//开始与结束位置
	if(end < begin ){
		webLog("file=%s;func=%s;msg=the end position is little than begin position;",__FILE__,__FUNCTION__);
		return false;
	}
	
	int pIndex ;			//父串位置计算
	int subIndex = 0;		//子串位置计数
	for(pIndex = begin;pIndex<end;pIndex++){
		substr[subIndex++] = str[pIndex];
	}
	substr[subIndex]='\0';
	return true;
}

/*
 * 功能：字符串内容反向排序
 * @ str :in out  输出覆盖输入
 * return 操作成功，返回 true,失败返回false
 */
boolean stringReverse(char *str){
	if(str == NULL){
		webLog("file=%s;func=%s;msg=input string is null;",__FILE__,__FUNCTION__);
		return false;
	}
	int length = strlen(str);	//字符串长度
	int i ;						//计数
	char temp;					//临时字符串变量 
	for( i = 0; i < length/2 ;i++){
		/*
		*首尾位置对换
		*/
		temp = str[i];
		str[i] = str[length - 1];
		str[length - 1] = temp;
	}
	return true;
}
/*
 * 功能，将字符串转换成长整数
 * @ str in
 * return 转换后的长整数
 */
long str2long(char *str){
	if(str == NULL){
		webLog("file=%s;func=%s;msg=the input string is null;",__FILE__,__FUNCTION__);
		return error;
	}
	return atol(str);
}
/*
 * 功能，将长整形转换成字符串
 * @ str out
 * @ num in
 * return 操作成功，返回true，否则返回false
 */
boolean long2str(char *str,long num){
	if(str == NULL ){
		webLog("file =%s;func=%s;msg=output string is null;",__FILE__,__FUNCTION__);
		return false;
	}
	sprintf(str,"%ld",num);
	return true;
}
/*
*功能：将十进制转换为二进制字符串
*@ num :in
*@ binstr :out 输出的二进制字符串
*return 
*/
void int2binstr(const int num,char *binstr){
	int x = num;
	int i;
	binstr[8] = '\0';
	int k = 7;
	for(i=1;;i++) 
	{
		binstr[k]=x%2+'0';
		x=x/2;
		k--;
		if(x==0) break; 
	} 
	//前面补0
	for(;k>=0;k--){
		binstr[k]='0';
	}

}
/*
*功能:去掉字符串后的空格和换行符
*@ str in out
*return 
*/
void trim(char *str){
	int length = strlen(str);	//字符串长度
	int i ;						//计算
	boolean isData = false;		//有效数据标志
				
	int mov;					//当前位置
	//处理字符串前面部分
	
	for(i = 0 ;i<length;i++){
		if(isData){
			break;
		}
		if(str[i]=='\n' || str[i]==' ' || str[i]=='\"'){
			for(mov = i;mov<length;mov++){
				str[mov]=str[mov+1];
			}
			
		}else{
			isData = true;
		}
	}
	//处理后面部分
	isData = false;
	length = strlen(str);
	for( i=length -1 ;i > 0;i--){
		if(isData){
			break;
		}
		if(str[i]=='\n' || str[i]==' '|| str[i]=='\"'){
			str[i]='\0';
			
		}else{
			isData = true;
		}
	}
	
}
/*
*功能:将日期转换为常规格式
*@ str :in out
*return 转换后的日期字符串
*/
void dtFormat(char *str){
	//去掉无效字符
	trim(str);
	struct datetime dt;			//时间数据结构
	char format[30];			//输出格式化字符串
	/*
	*设置时间各变量的值
	*/
	//年
	substring(str,dt.year,0,4);
	//月
	substring(str,dt.month,4,6);
	//日
	substring(str,dt.date,6,8);
	//时
	substring(str,dt.hour,8,10);
	//分
	substring(str,dt.minute,10,12);
	//秒
	substring(str,dt.second,12,14);
	//设置输出数据
	sprintf(format,"%s-%s-%s %s:%s:%s",
				dt.year,
				dt.month,
				dt.date,
				dt.hour,
				dt.minute,
				dt.second);
	//覆盖输入作为输出
	strcpy(str,format);
}
/*
*功能:将long形时间格式化字符串
*@ lTime :in     以秒为格式的时间
*@ str   :out	 输出准备时间字符串,以"2009-1-1 00:00:00"的格式
*return 
*/
void long2TimeStr(long lTime,char *str){
	struct tm *p;		//时间数据结构
	char value[10];		//中间变量 
	//设置数据结构各变量 
	p=localtime(&lTime); 
	//初始化
	strcpy(str,"");
	
	/*
	*设置各项信息
	*/
	sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",
						1900+p->tm_year,
						1+p->tm_mon,
						p->tm_mday,
						p->tm_hour,
						p->tm_min,
						p->tm_sec);
	//年
//	sprintf(value,"%d",(1900+p->tm_year));
//	strcat(str,value);
	//月
//	if((1+p->tm_mon) < 10){
//		sprintf(value,"-0%d",(1+p->tm_mon));
//	}else{
//		sprintf(value,"-%d",(1+p->tm_mon));
//	}
//	strcat(str,value);
	//日
//	if(p->tm_mday < 10){
//		sprintf(value,"-0%d",p->tm_mday);
//	}else{
//		sprintf(value,"-%d",p->tm_mday);
//	}
//	strcat(str,value);
	//时
//	if(p->tm_hour < 10){
//		sprintf(value," 0%d",p->tm_hour);
//	}else{
//		sprintf(value," %d",p->tm_hour);
//	}
//	strcat(str,value);
//	//分
//	if(p->tm_min < 10){
//		sprintf(value,":0%d",p->tm_min);
//	}else{
//		sprintf(value,":%d",p->tm_min);
//	}
//	strcat(str,value);
//	//秒
//	if(p->tm_sec < 10){
//		sprintf(value,":0%d",p->tm_sec);
//	}else{
//		sprintf(value,":%d",p->tm_sec);
//	}
//	strcat(str,value);

}

/*
*功能:将日期字符串转换成long 格式
*@str :in
*return long
*说明:字符串以"2009-1-1 00:00:00"的格式
*/
long str2time_t(char *str){	
	if(!strcmp(str,"0")){
		return 0;
	}
	time_t timep;		//时间和秒形式
	struct tm tp;		//时间数据结构
	char *date;			//日期
	char *time;			//时间
	//提取日期部分
	date = strtok(str," ");
	//提取时间部分
	time = strtok(NULL," ");
	
	/*
	*日期部分转换
	*/
	//年
	tp.tm_year = atol(strtok(date,"-"))-1900;
	//月
	tp.tm_mon = atol(strtok(NULL,"-"))-1;
	//日
	tp.tm_mday = atol(strtok(NULL,"-"));
	
	/*
	*时间部分转换
	*/
	//时
	tp.tm_hour = atol(strtok(time,":"));
	//分
	tp.tm_min = atol(strtok(NULL,":"));
	//秒
	tp.tm_sec = atol(strtok(NULL,":"));
	//将时间数据转换成秒形式
	timep = mktime(&tp);
	return timep;
}
/*
*功能:将字符串转换为大写字母
*@ str :in out
*return 转换后的字符串
*/
void str2upper(char *str){
	int i ;			//计算
	for(i = 0;i<strlen(str);i++){
		str[i] = toupper(str[i]);
	}
}







/*
*功能：在字符串中根据指定的分隔符提取第i个元素，i从1开始
*@ str		:in
*@ substr	:out
*@ ch		:分隔符
*@ i		:第几个子串
*return 成功提取第i个元素，则返回真，否则返回假
*/
boolean getItem(char *str,char *substr,char ch,int i){
	int start = 0;			//要提取的子串的开始位置
	int end=0;				//要提取的子串的结束位置
	int startSkip = i - 2;  //第几个子串的,开始位置
	int endSkip = i - 1;	//第几个子串的,结束位置
	//获取子串的结束位置
	end = stringIndex(str,ch,endSkip);
	if(end == none){
		return false;
	}
	//获取子串的开始位置 
	if(i == 1){
		start = 0;
	}else{
		start = stringIndex(str,',',startSkip)+1;
	}
	//提取子串
	substring(str,substr,start,end);	
	return true;
}
/*
*功能：在字符串中删除指定元素
*@ str		:in out 父串
*@ delStr	:in     要删除的子串
*return 成功提取第i个元素，则返回真，否则返回假
*/
boolean delItem(char *str,char *delStr){
	int i = 1;			//计数
	char substr[100];	//子串
	char outData[300] = "";	//输出结果
	/*
	*重新获取一遍所有子串,并将子串存放到输出结果中
	*在此过程中,遇到要删除的子串,则不放到输出结果中
	*最后把输出结覆盖父串即可
	*/
	while(getItem(str,substr,',',i++)){
		//跳过指定字符串即可
		if(!strstr(substr,delStr)){
			strcat(outData,substr);
			strcat(outData,",");
		}
	}
	strcpy(str,outData);
	return true;
}
/*
*功能:将时间字符转换为设备接受的时间字符串
*@ str :in out 
*输入字符串形式 "2009-01-02 12:12:00"
*输出字符串形式 "20090102121200"
*return
*/
void time2str(char *str){
	
	int length = strlen(str);  //字符串长度
	int i;					   //计数
	int k;					   //计数
	/*
	*只需将非数字的字符删去即可
	*/
	for(i = 0;i<length;i++){
		//不是数字字符,则删去 
		if(!isdigit(str[i])){
			for(k = i;k<length;k++ ){
				str[k]=str[k+1];
			}
			--length;
		}
	}
}
/*
*功能:从字符串中提取变量对应的值
*@str	:in		字符串
*@name	:in		字段名
*@value :out	字段值,输出
*seccess ,return true,else return false
*说明:字符串是"name1=value1;name2=value2;"的形式
*/
boolean getValueByName(const char *str,const char *name,char *value){
	char *temp;				//临时指针
	char find_str[50];		//查找字符串
	char *start;			//开始位置
	char *end;				//结束位置	
	int size;				//长度
	if(str == NULL || name == NULL || value == NULL){
		webLog("file=%s;func=%s;msg=input string ,name or value is null;",__FILE__,__FUNCTION__);
		return false;
	}
	//构造查找字符串
	strcpy(find_str,name);
	strcat(find_str,"=");
	//获取字段名位置
	temp = strstr(str,find_str);
	//字符串中无要找的变量
	if(!temp){
		return false;
	}
	//查找字段值位置
	start = temp + strlen(find_str);
	//以;为字段值结束位置
	end = strchr(temp,';');
	//计算字段值长度
	size = end - start;
	//将字段值拷贝到输出中
	memcpy(value,start,size);
	value[size] = '\0';
	return true;
}


/*
*功能:字符串拷贝
*@dest :out
*@src :in
*return 
*说明:用于对远端字符串为40的情况
*/
void copyStrs(char *dest,char *src){
	/*
	*如果为最大长度,则要在最后加上'\0'作为字符串结束
	*/
	int len = 40;	//字符串长度
	int i ;
	boolean isMax = true;	//是否为最大长度
	for(i = 0; i < len; i++){
		dest[i] = src[i];
		if(src[i] == '\0'){
			isMax = false;
			break;
		}
	}
	if(isMax){
		dest[len] = 0;
	}
	
}


