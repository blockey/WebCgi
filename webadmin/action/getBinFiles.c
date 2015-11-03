//upload.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "../base/base.h"
#include "../lib/cgic.h"

// #include "../card/snmpSetMeta.h"
//getBinFiles.cgi?emID
//����ʾ����http://192.168.0.193/cgi-bin/webadmin/getBinFiles.cgi?emId=193&fileName=age_time.bin


#define INTERNET_BUFF_LENTH 1024*180 


// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"
// pSrc: Դ����ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���ݳ���
// ����: Ŀ���ַ�������
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf���ַ����ұ�

    int i = 0; 
    for (i = 0; i < nSrcLength; i++)
    {
        // �����4λ
        *pDst++ = tab[*pSrc >> 4];

        // �����4λ
        *pDst++ = tab[*pSrc & 0x0f];

        pSrc++;
    }

    // ����ַ����Ӹ�������
    *pDst = '\0';

    // ����Ŀ���ַ�������
    return nSrcLength * 2;
}



int cgiMain(void){
   	char emId[512] = {0};  //��ԪID
   	char fileName[512] = {0}; //�ļ�����
   	char fullFileName[512] = {0}; //����ԪID�ļ�����
	char configToPcRoute[128] = "/app/webapp/httpd/html/file/to_pc/";

    //ȡ��������
	cgiFormString("emId",emId,sizeof(emId));
	cgiFormString("fileName",fileName,sizeof(fileName));
	cgiHeaderContentType("text/html; charset=utf-8");
	sprintf(fullFileName,"%s_%s",emId,fileName);
    //printf("fullFileName=%s\r\n",fullFileName);


    //���ļ�����
    char bin_file_name[256] = {0};    
    char info[1024] = {0};
    sprintf(bin_file_name,"/app/webapp/httpd/html/performance/%s",fullFileName);    
    int   numread=0;    
    FILE *pFile = NULL;    
    pFile = fopen(bin_file_name,"rb");    
    if (pFile == NULL)    
    {        
        printf("Error: %s not exist!\r\n", bin_file_name);        
        return 0;    
    }
    else
    {
        sprintf(info,"binaryFile=%s\n",bin_file_name);
        webLog(info);
    }

    char buffer[INTERNET_BUFF_LENTH] = {0};
    numread=fread(buffer,sizeof(char),sizeof(buffer),pFile);
    //printf("Numberofitemsread=%d\n",numread);
    fclose(pFile);


    char szHexBuffer[INTERNET_BUFF_LENTH*2] = {0};
    
    gsmBytes2String(buffer,szHexBuffer,numread);
    printf("szHexBuffer=%s\r\n",szHexBuffer);
    webLog(szHexBuffer);

	return 0;
}
