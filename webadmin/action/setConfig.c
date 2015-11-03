/*
* setConfig.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../lib/cgic.h"
#include "../base/base.h"
#include "../services/snmpServices.h"
#include "../services/sessionServices.h"

#define INTERNET_BUFF_LENTH 1024*128 
#define OID_LENTH 256         //oid length
#define CONTENT_LENTH 1224*2  //snmp User Data length(Hex Lenth)
#define COMMAND_LENTH 1024*10 //snmpget/snmpset command length(10k)

#define SNMP_DUMP_FILE_PATH   "/app/webapp/datas/snmpdump.txt"

int ReadOidValFromResultFile(char *oid, char *content);


int IsHexString(char *str);

int IsHexString(char *str)
{
    int res = 1,/*hex string return 1,otherwize return  0*/lenth = strlen(str);
    if(lenth<=0)
    {
        res = 0;
        return res;
    }

    int i=0; 
    for(i=0; i<lenth; i++)
    {
        if(str[i]=='\n')
        {//end of string
            break;
        }
      
        if(str[i]==' ')
        {//ignore space
            continue;
        }
        
        if((str[i]>='0' && str[i]<='9')
            || (str[i]>='A' && str[i]<='Z')
            || (str[i]>='a' && str[i]<='z'))
        {
             continue;
        }
        else 
        {
            res = 0;
            break;
        }
    }
	//webLog("src=%s: %d(1-yes,0-no)",str, res);

    return res;
}


/*hex string return 1,otherwize return  0*/
char *strreplace(char *dest, char oldstr, char newstr)
{
    if(oldstr==newstr)
        return dest;
    
    int res = 1,lenth = strlen(dest);
    if(lenth<=0)
    {
        res = 0;
        return NULL;
    }

    int i=0; 
    for(i=0; i<lenth; i++)
    {
        if(dest[i] == oldstr)
        {//ignore space
            dest[i] = newstr;
        }
    }

    return dest;
}



int getPrevValue(char *buffer)
{
    int   numread=0;    
    FILE *pFile = NULL;    
    pFile = fopen(SNMP_DUMP_FILE_PATH,"rb");    
    if (pFile == NULL)    
    {
        webLog("Error: %s not exist!\r\n", SNMP_DUMP_FILE_PATH);    
        return 0;
    }

    char tmpbuffer[INTERNET_BUFF_LENTH] = {0};
    numread=fread(tmpbuffer,sizeof(char),sizeof(tmpbuffer),pFile);
    fclose(pFile);

    if(numread <=0 )
    {//The file is empty!
        return 0;
    }
    
   strreplace(tmpbuffer,'\n',' ');
   strncpy(buffer,tmpbuffer,numread);

    webLog("Last time buffer=%s\r\n",buffer);
    return numread;
}


int setPrevValue(char *buffer)
{
    char tmpcmd[INTERNET_BUFF_LENTH] = {0};

    sprintf(tmpcmd, "echo \"%s\" >%s",
            buffer, SNMP_DUMP_FILE_PATH);
    
    return system(tmpcmd);
}

/*
get name=value, from @src string
*/
void getNameValue(const char *src,const char *name,char *value)
{
	//split ','
	char *pTmpListStr = NULL;
	int size = 0, isOverflow = 0 /*0-no , 1-yes*/;
	char *token = NULL, *mainSpliter = ";",
		 *subSpliter = "=";

    int nLen = strlen(src);
    pTmpListStr = (char *)malloc(sizeof(char)*(nLen+1));
    memset(pTmpListStr,0,nLen+1);
    strncpy(pTmpListStr,src,nLen);
    
	token = strtok(pTmpListStr,mainSpliter); 
	while( token != NULL )
	{
		char *pIntvl = NULL;
		pIntvl = strstr(token,subSpliter);
		if(pIntvl)
		{//not interval
			int i,nStart = 0, nEnd = 0;
			*pIntvl = '\0';
            if(strcmp(name,token)== 0)
            {
    			strcpy(value,pIntvl+1);
                break;
            }
		}

		token = strtok( NULL, mainSpliter);          /* Get next token: */ 
	}

	//free tmp str.
	free(pTmpListStr);
	pTmpListStr = NULL;
	return ;
}

typedef struct _OidDriverTab
{
    int id;
    char szOid[256];
}OidDriverTab;


/*
web-client: OID ID -->  OID Value mapping Table
Just for Configure OID
*/
OidDriverTab  driver[] = 
{
    {1,"1.3.6.1.4.1.16985.1.12.2.5.2.1.1"},
    {2,"1.3.6.1.4.1.16985.1.12.2.5.2.1.2"},
    {3,"1.3.6.1.4.1.16985.1.12.2.5.2.1.3"},
    {4,"1.3.6.1.4.1.16985.1.12.2.5.2.1.4"},
    {5,"1.3.6.1.4.1.16985.1.12.2.5.2.1.5"},
    {6,"1.3.6.1.4.1.16985.1.12.2.5.2.1.6"},
    {7,"1.3.6.1.4.1.16985.1.12.2.5.2.1.7"},
    {8,"1.3.6.1.4.1.16985.1.12.2.5.2.1.8"},
    {9,"1.3.6.1.4.1.16985.1.12.2.5.2.1.9"},
    {10,"1.3.6.1.4.1.16985.1.12.2.5.2.1.10"},
    {11,"1.3.6.1.4.1.16985.1.12.2.5.2.1.11"},
    {12,"1.3.6.1.4.1.16985.1.12.2.5.2.1.12"},
    {13,"1.3.6.1.4.1.16985.1.12.2.5.2.1.13"},
    {14,"1.3.6.1.4.1.16985.1.12.2.5.2.1.14"},
    {15,"1.3.6.1.4.1.16985.1.12.2.5.2.1.15"},
    {16,"1.3.6.1.4.1.16985.1.12.2.5.2.1.16"},
    {17,"1.3.6.1.4.1.16985.1.12.2.5.2.1.17"},
    {18,"1.3.6.1.4.1.16985.1.12.2.5.2.1.18"},
    {19,"1.3.6.1.4.1.16985.1.12.2.5.2.1.19"},
    {20,"1.3.6.1.4.1.16985.1.12.2.5.2.1.20"},
    {21,"1.3.6.1.4.1.16985.1.12.2.5.2.1.21"},
    {22,"1.3.6.1.4.1.16985.1.12.2.5.2.1.22"},
    {23,"1.3.6.1.4.1.16985.1.12.2.5.2.1.23"},
    {24,"1.3.6.1.4.1.16985.1.12.2.5.2.1.24"},
    {25,"1.3.6.1.4.1.16985.1.12.2.5.2.1.25"},
    {26,"1.3.6.1.4.1.16985.1.12.2.5.2.1.26"},
    {27,"1.3.6.1.4.1.16985.1.12.2.5.2.1.27"},
    {28,"1.3.6.1.4.1.16985.1.12.2.5.2.1.28"},
    {29,"1.3.6.1.4.1.16985.1.12.2.5.2.1.29"},
    {30,"1.3.6.1.4.1.16985.1.12.2.5.2.1.30"},
    {31,"1.3.6.1.4.1.16985.1.12.2.5.2.1.31"},
    {32,"1.3.6.1.4.1.16985.1.12.2.5.2.1.32"},
    {33,"1.3.6.1.4.1.16985.1.12.2.5.2.1.33"},
    {34,"1.3.6.1.4.1.16985.1.12.2.5.2.1.34"},
    {35,"1.3.6.1.4.1.16985.1.12.2.5.2.1.35"},
    {36,"1.3.6.1.4.1.16985.1.12.2.5.2.1.36"},
    {37,"1.3.6.1.4.1.16985.1.12.2.5.2.1.37"},
    {38,"1.3.6.1.4.1.16985.1.12.2.5.2.1.38"},
    {39,"1.3.6.1.4.1.16985.1.12.2.5.2.1.39"},
    {40,"1.3.6.1.4.1.16985.1.12.2.5.2.1.40"},

};

int getOidStr(int id, char *OidStr)
{
    int res=1, i=0, size = sizeof(driver)/sizeof(driver[0]);
    
    for(i=0; i<size; i++)
    {
        if(driver[i].id == id)
        {
            strcpy(OidStr,driver[i].szOid);
            break;    
        }
    }

    if(i == size)
    {
        res = -1;
    }
    
    return res;
}

/*
setConfig.cgi input format
O=X&F=1&V=XXXXXX
O-OID ,SNMP OID ID by Global
F-Flag for Truncate, a long SNMP packet will be devided several packets. 1- 
V-SNMP PACKET VALUE, Hex string that like "01020FAD...".
Calling Format:
eg. setConfig.cgi?O=2&F=0&V=01020FAD...
*/ 
int cgiMain(){
	
	char szOid[OID_LENTH] = {0};   
	char szContent[CONTENT_LENTH] = {0};
	char szFlag[8]={0};	
    
	cgiHeaderContentType("text/html; charset=gb2312");
    
	cgiFormString("O", szOid, sizeof(szOid));
	cgiFormString("V", szContent, sizeof(szContent));
	cgiFormString("F", szFlag, sizeof(szFlag));

	char szBuffer[INTERNET_BUFF_LENTH]={0};	
 	char szPreFileBuffer[INTERNET_BUFF_LENTH]={0};	 // file size dumped previous 
    int Oid = atoi(szOid);
	int flag = atoi(szFlag); //packet truncate flag. 0-false,1-true

    int fileSize = getPrevValue(szPreFileBuffer);
	if(flag == 0)
	{
        if(fileSize <=0 )
        {
            sprintf(szBuffer,"O=%d;V=%s", Oid, szContent);
            setPrevValue(szBuffer);
        }
        else
        {
            char preOid[OID_LENTH] = {0}, preContent[CONTENT_LENTH] = {0};
            getNameValue(szPreFileBuffer, "O", preOid);
            getNameValue(szPreFileBuffer, "V", preContent);

            int nPreOid = atoi(preOid);
            if(nPreOid != Oid)
            {//Different OID, Drop Last Dumped Datas
                sprintf(szBuffer,"O=%d;V=%s", Oid, szContent);
                setPrevValue(szBuffer);
            }
            else
            {
                sprintf(szBuffer,"O=%d;V=%s%s", Oid, preContent, szContent);
                setPrevValue(szBuffer);
            }
        }
        
        return 0;
    }

    //flag != 0
    if(fileSize >=0 )
    {//have dumped snmp packet
        char preOid[OID_LENTH] = {0}, preContent[CONTENT_LENTH] = {0};
        
        getNameValue(szPreFileBuffer, "O", preOid);
        getNameValue(szPreFileBuffer, "V", preContent);
    

        int nPreOid = atoi(preOid);
        if(nPreOid == Oid)
        {
            sprintf(szBuffer,"%s%s",preContent,szContent);
            strcpy(szContent,szBuffer);
            setPrevValue(""); //clear the dump file
        }
    }

    if(getOidStr(Oid,szOid) < 0)
    {//O -Error: cannot map the OID string
        return -1;
    }

	//snmpset to transmit the snmp packet
    char command[COMMAND_LENTH] = {0};
    char szValue[COMMAND_LENTH] = {0};

    AddBlank(szContent, szValue);
    
    sprintf(command, "%s %s %s x \"%s\" > %s 2>&1",
        snmpSetCom,hostIp,
        szOid, szValue,
        commandOutputFile);
	
	webLog("command=%s",command);
	int res = system(command);
	if(res == 127){
		webLog("file=%s;func=%s;msg=shell error;",__FILE__,__FUNCTION__);
		return -1;
	}
	else if(res < 0){
		webLog("file=%s;func=%s;msg=execute Set command error;",__FILE__,__FUNCTION__);
		return -1;
	}

	//snmpget to get the transmiting status 
   sprintf(command, "%s %s %s > %s 2>&1",
        snmpGetCom,hostIp,
        szOid,commandOutputFile);
	webLog("command=%s",command);
	res = system(command);
	if(res == 127){
		webLog("file=%s;func=%s;msg=shell error;",__FILE__,__FUNCTION__);
		return -1;
	}
	else if(res < 0){
		webLog("file=%s;func=%s;msg=execute Get command error;",__FILE__,__FUNCTION__);
		return -1;
	}

    

    //analyze the result
    memset(szContent,0,sizeof(szContent));
    res = ReadOidValFromResultFile(szOid,szContent);
	if(res>0){
		printf("result=true;%s",szContent);
		
	}
	//??ȡʧ??
	else{
		//??ʾϵͳ????
		printf("result=false;");
	}
	return 0;
	
}


int ReadOidValFromResultFile(char *oid, char *content)
{
    char row[1024*4] = {0};
	FILE *fp = NULL; 
	char tmpContent[CONTENT_LENTH] = {0};
    
	int  ret = 1;
	boolean isEof = false,isFoundHexHead = false;
    
    fp = fopen(commandOutputFile,"r");
	//strcpy(str,"");
	while(!isEof)
    {
        memset(row, 0, sizeof(row));
		isEof = !fgets(row,sizeof(row),fp);
        int nlen = strlen(row);
        strreplace(row,'\n',' ');
        
        //if(row[nlen] == '\n')
            //row[nlen] = ' '; //clear the '\n'
        if(isFoundHexHead && IsHexString(row))
        {
            strcat(content,trimEx(row));
            continue;
        }
        
		if(strstr(row,"Timeout"))
        {
			webLog("file=%s;func=%s;msg=snmp time out;",__FILE__,__FUNCTION__);
			ret = -2; //agent might be exitted
			break;
		}
		
		if(strstr(row,oid+2))
        {//
        	char *p = strstr(row,"Hex-STRING:");
            if(p == NULL)
            {
                ret = -1;
                break;
            }

            p += strlen("Hex-STRING:");

            isFoundHexHead = true;            
            sprintf(content, "%s", trimEx(p));
		}
	}
	fclose(fp);

    trimEx(content);
	return ret;

}
                                                                                        
