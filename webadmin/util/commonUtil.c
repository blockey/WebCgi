#include <stdlib.h>
#include "commonUtil.h"
#include<fcntl.h>
#include<sys/stat.h>
#include<stdarg.h> 
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<netdb.h> 

#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include "../base/base.h"
#include "adstring.h"

/*
*功能:根据请求参数设置时间查询范围
*@tr	:out
*@str	:in
*return 
*/
void setTimeRangByString(struct TimeRange *tr,char *str){
	char value[20];			//中间变量
	//初始化
	tr->from = 0;
	tr->to = 0;
	value[0] = NULL;
	//获取时间段的开始时间
	if(getValueByName(str,"from",value)){
		if(value[0] != NULL){
			//将字符串转换成long型
			tr->from = str2time_t(value);
		}
	}
	value[0] = NULL;
	//获取时间段结束时间
	if(getValueByName(str,"to",value)){
		if(value[0] != NULL){
			//将字符串转换成long型
			tr->to = str2time_t(value);
		}
		
	}
}
char *getDistInnerIp(uchar emId){
	struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    int fd = socket( AF_INET, SOCK_DGRAM, 0);
    if( fd <= 0 ){
        webLog(" Create raw_sock Failed !!  \n") ;
        return NULL;
    }
    struct ifreq ifreq_Buf;
    //strcpy(ifreq_Buf.ifr_name, "eth0");  // "eth1"
    strcpy(ifreq_Buf.ifr_name, "dtl0");  //����DCNͨ��
    if ( -1 == ioctl(fd, SIOCGIFADDR, &ifreq_Buf)){
        webLog(" Get Hw Addr Failed !! \n");
        return NULL;
    }
    close(fd);
    struct sockaddr_in *addr = (struct sockaddr_in *)&ifreq_Buf.ifr_addr;
    char *address = inet_ntoa( addr->sin_addr);
	struct in_addr iIp = addr->sin_addr;
	//替换IP最后一位为网元IP
	iIp.s_addr &=~0xFF000000;
	iIp.s_addr |= (emId<<24);
	address = inet_ntoa( iIp);			
	//替换IP最后一个字节
	
	return address;
}

char *getLocalIp(void)
{
	struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    int fd = socket( AF_INET, SOCK_DGRAM, 0);
    if( fd <= 0 ){
        webLog(" Create raw_sock Failed !!  \n") ;
        return NULL;
    }
    struct ifreq ifreq_Buf;
    //strcpy(ifreq_Buf.ifr_name, "eth0");  // "eth1"
    strcpy(ifreq_Buf.ifr_name, "dtl0");  //����DCNͨ��
    if ( -1 == ioctl(fd, SIOCGIFADDR, &ifreq_Buf)){
        webLog(" Get Hw Addr Failed !! \n");
        return NULL;
    }
    close(fd);
    struct sockaddr_in *addr = (struct sockaddr_in *)&ifreq_Buf.ifr_addr;
    char *address = inet_ntoa( addr->sin_addr);
	return address;
}
