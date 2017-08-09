/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: zhangzhixiang
 *
 * Created on 2016年9月2日, 上午10:07
 */

#include <string>
#include <set>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <list>
#include "EmQuantAPI.h"
#include "glog/logging.h"

#include "user_init.h"
#include "user_app.h"

#include <zookeeper/zookeeper.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <zookeeper/zookeeper_log.h>
#include "election.h"


using namespace std;

typedef void  (*psetserverlistdir)(const char* dir);
typedef EQErr (*pstart)(EQLOGININFO* pLoginInfo, const char* options, logcallback pfnCallback);
typedef EQErr (*pstop)();
typedef const char* (*pgeterrstring)(EQErr errcode, EQLang lang);
typedef EQErr (*pcsd)(const char* codes, const char* indicators, const char* startdate, const char* enddate, const char* options, EQDATA*& eqdata);
typedef EQErr (*pcss)(const char* codes, const char* indicators, const char* options, EQDATA*& pEQData);
typedef EQErr (*ptradedates)(const char* startdate, const char* enddate, const char* options, EQDATA*& pEQData);
typedef EQErr (*psector)(const char* pukeyCode, const char* tradeDate, const char* options, EQDATA*& pEQData);
typedef EQErr (*pgetdate)(const char* tradeDate, int offDay, const char* options, EQDATA*& pEQData);
typedef EQErr (*psetcallback)(datacallback pfnCallback);
typedef EQID  (*pcsq)(const char* codes, const char* indicators, const char* options, datacallback pfnCallback, LPVOID lpUserParam);
typedef EQErr (*pcsqcancel)(EQID serialID);
typedef EQErr (*preleasedata)(void* pEqData);
typedef EQErr (*pcsc)(const char* code, const char* indicators, const char* startDate, const char* endDate, const char* options, EQDATA*& pEQData);
typedef EQID  (*pcst)(const char* codes, const char* indicators, const char* startdatetime, const char* enddatetime, const char* options, datacallback pfnCallback, LPVOID lpUserParam);
typedef EQErr (*pctr)(const char* ctrName, const char* indicators, const char* options, EQCTRDATA*& pEQCtrData);
typedef EQErr (*pcsqsnapshot)(const char* codes, const char* indicators, const char* options, EQDATA*& pEQData);

psetserverlistdir emsetserverlistdir = NULL;
pstart        emstart = NULL;
pstop         emstop = NULL;
pgeterrstring emgeterrstring = NULL;
pcsd          emcsd = NULL;
pcss          emcss = NULL;
ptradedates   emtradedates = NULL;
psector       emsector = NULL;
pgetdate      emgetdate = NULL;
psetcallback  emsetcallback = NULL;
pcsq          emcsq = NULL;
pcsqcancel    emcsqcancel = NULL;
preleasedata  emreleasedata = NULL;
pcsc          emcsc = NULL;
pcst          emcst = NULL;
pctr          emctr = NULL;
pcsqsnapshot  emcsqsnapshot = NULL;

//转string函数示例
template<typename T>
std::string to_str(const T &v)
{
    stringstream s;
    s << v;
    return s.str();
}

//日志回调函数示例
int write2Log(const char* log)
{
    printf("%s\n",log);
    
    //int fp = open("../bin/log.log", O_RDWR | O_CREAT | O_APPEND, 0640);
    //if(fp == -1)
    //{
    //    return 0;
    //}
    //write (fp ,log, strlen(log));
    //close(fp);
    
    //printf("%s\n",log);
    return 0;
}

//取EQVARIENT值示例
std::string eqvalue2string(const EQVARIENT* pEQVarient)
{
    if(!pEQVarient)
    {
        return "";
    }

    static char z[4096] = {0};
    memset(z, 0, 4096);
    std::string s = "";
    switch(pEQVarient->vtype)
    {
        //以下为了输出便于观察，做了字符宽度限定处理，实际可参考需求处理
    case eVT_null:
        sprintf(z,"%s", "        --");
        s = z;
        break;
    case eVT_char:
        sprintf(z,"%c", pEQVarient->unionValues.charValue);
        s = z;
        break; 
    case eVT_bool:
        sprintf(z,"%c", pEQVarient->unionValues.boolValue ? "true" : "false");
        s = z;
        break;
    case eVT_int:
        sprintf(z,"%10d", pEQVarient->unionValues.intValue);
        s = z;
        break;
    case eVT_uInt:
        sprintf(z,"%10u", pEQVarient->unionValues.uIntValue);
        s = z;
        break;
    case eVT_int64: 
        sprintf(z,"%10lld", pEQVarient->unionValues.int64Value);
        s = z;
        break;
    case eVT_uInt64: 
        sprintf(z,"%10llu", pEQVarient->unionValues.uInt64Value);
        s = z;
        break;
    case eVT_float:  
        sprintf(z,"%10.3f", pEQVarient->unionValues.floatValue);
        s = z;
        break;
    case eVT_double: 
        sprintf(z,"%14.3lf", pEQVarient->unionValues.doubleValue);
        s = z;
        break;
    case eVT_asciiString:
        sprintf(z,"%s", pEQVarient->eqchar.pChar);
        s = z;
        break;
    case eVT_unicodeString: 
        sprintf(z,"%s", pEQVarient->eqchar.pChar);
        s = z;
        break;
    case eVT_short:     
        sprintf(z,"%10d", pEQVarient->unionValues.shortValue);
        s = z;
        break;
    case eVT_ushort:     
        sprintf(z,"%10u", pEQVarient->unionValues.uShortValue);
        s = z;
        break;

    default:
        break;
    }

    return s;
}
//主回调函数示例
int obtainCallback(const EQMSG* pMsg, LPVOID lpUserParam)
{
    LPVOID pUser = lpUserParam;

    static int times = 1;
    time_t tt = time(NULL);
    tm* t = localtime(&tt);

    printf("****************************************\n");
    printf("[%02d:%02d:%02d]Received [%d] callback: msgType=%d, err=%d, requestID=%d, serialID=%d.\n", t->tm_hour, t->tm_min, t->tm_sec, times++, pMsg->msgType, pMsg->err, pMsg->requestID, pMsg->serialID);


    if(!pMsg->pEQData)
    {
        printf("****************************************\n");
        return 0;
    }

    EQDATA *pEQData = pMsg->pEQData;
    string slinehead(""), sline("");
    if(pEQData->dateArray.nSize == 1)
    {
        slinehead = pEQData->dateArray.pChArray[0].pChar;
        slinehead += " ";
        for(int i=0;i<pEQData->codeArray.nSize;i++)
        {
            sline += string(pEQData->codeArray.pChArray[i].pChar);
            sline += " ";

            for(int j=0;j<pEQData->indicatorArray.nSize;j++)
            {
                if(i == 0)
                {
                    slinehead += string(pEQData->indicatorArray.pChArray[j].pChar);
                    slinehead += " ";

                    if(j == pEQData->indicatorArray.nSize-1)
                    {
                        printf("%s\n",slinehead.c_str());
                    }
                }

                EQVARIENT* pEQVarient = (*pEQData)(i,j,0);
                if(pEQVarient)
                {
                    sline += eqvalue2string(pEQVarient);
                    sline += " ";
                }
            }
            sline += "\n";
        }
        printf("%s",sline.c_str());
    }
    else
    {
        for(int k=0;k<pEQData->dateArray.nSize;k++)
        {
            slinehead = pEQData->dateArray.pChArray[k].pChar;
            sline = "";

            for(int i=0;i<pEQData->codeArray.nSize;i++)
            {
                sline += string(pEQData->codeArray.pChArray[i].pChar);
                sline += " ";

                for(int j=0;j<pEQData->indicatorArray.nSize;j++)
                {
                    if(i == 0)
                    {
                        slinehead += string(pEQData->indicatorArray.pChArray[j].pChar);
                        slinehead += " ";

                        if(j == pEQData->indicatorArray.nSize-1)
                        {
                            printf("%s\n",slinehead.c_str());
                        }
                    }

                    EQVARIENT* pEQVarient = (*pEQData)(i,j,0);
                    if(pEQVarient)
                    {
                        sline += eqvalue2string(pEQVarient);
                        sline += " ";
                    }
                }
                sline += "\n";
            }

            sline += "\n";
            printf("%s",sline.c_str());
        }
    }

    printf("****************************************\n");
    return 0;
}

//实时行情回调函数示例
int csqCallback(const EQMSG* pMsg, LPVOID lpUserParam)
{
    static int times = 1;
    
    time_t tt = time(NULL);
    tm* t = localtime(&tt);

    printf("****************************************\n");
    printf("[%02d:%02d:%02d]Received [%d] callback: msgType=%d, err=%d, requestID=%d, serialID=%d.\n", t->tm_hour, t->tm_min, t->tm_sec, times++, pMsg->msgType, pMsg->err, pMsg->requestID, pMsg->serialID);

    if(!pMsg->pEQData)
    {
    //    printf("****************************************\n");
        return 0;
    }

    EQDATA *pEQData = pMsg->pEQData;
    string slinehead(""), sline("");
    std::list<std::string> market_info;
    if(pEQData->dateArray.nSize == 1)
    {
        slinehead = pEQData->dateArray.pChArray[0].pChar;
        slinehead += " ";
        for(int i=0;i<pEQData->codeArray.nSize;i++)
        {
            std::string info;
            info += string(pEQData->codeArray.pChArray[i].pChar);
            sline += string(pEQData->codeArray.pChArray[i].pChar);
            sline += " ";
            info += " ";

            for(int j=0;j<pEQData->indicatorArray.nSize;j++)
            {
                if(i == 0)
                {
                    slinehead += string(pEQData->indicatorArray.pChArray[j].pChar);
                    slinehead += " ";

                    if(j == pEQData->indicatorArray.nSize-1)
                    {
                        printf("%s\n",slinehead.c_str());
                    }
                }

                EQVARIENT* pEQVarient = (*pEQData)(i,j,0);
                if(pEQVarient)
                {
                    sline += eqvalue2string(pEQVarient);
                    sline += " ";
                    info += eqvalue2string(pEQVarient);
                    info += " ";
                    
                }
            }
            sline += "\n";
            market_info.push_back(info);
        }
       //  printf("%s",sline.c_str());
    }
    else
    {
        for(int k=0;k<pEQData->dateArray.nSize;k++)
        {
            slinehead = pEQData->dateArray.pChArray[k].pChar;
            sline = "";
            std::string info;
            for(int i=0;i<pEQData->codeArray.nSize;i++)
            {
                sline += string(pEQData->codeArray.pChArray[i].pChar);
                sline += " ";
                info += string(pEQData->codeArray.pChArray[i].pChar);
                info += " ";

                for(int j=0;j<pEQData->indicatorArray.nSize;j++)
                {
                    if(i == 0)
                    {
                        slinehead += string(pEQData->indicatorArray.pChArray[j].pChar);
                        slinehead += " ";

                        if(j == pEQData->indicatorArray.nSize-1)
                        {
                            printf("%s\n",slinehead.c_str());
                        }
                    }

                    EQVARIENT* pEQVarient = (*pEQData)(i,j,0);
                    if(pEQVarient)
                    {
                        sline += eqvalue2string(pEQVarient);
                        sline += " ";
                        info += eqvalue2string(pEQVarient);
                        info += " ";
                    }
                }
                sline += "\n";
                market_info.push_back(info);
            }

            sline += "\n";
          //  printf("%s",sline.c_str());
        }
    }

   // printf("Thread %x my result is: %s", pthread_self(), sline.c_str());
   user_recv(market_info);
   // printf("****************************************\n");
    return 0;
}

//日内跳价服务回调函数示例，本示例是将内容落地到文件，便于观察
int cstCallback(const EQMSG* pMsg, LPVOID lpUserParam)
{
    LPVOID pUser = lpUserParam;
    char buf[1024] = {0};
    
    if(!pMsg->pEQData)
    {
        return 0;
    }

    EQDATA *pEQData = pMsg->pEQData;
    if(pEQData->codeArray.nSize != 1)
    {
        return 0;
    }

    memset(buf, 0, 1024);
    sprintf(buf,"./%s.txt", pEQData->codeArray.pChArray[0].pChar);
    
    int fp = open(buf, O_RDWR | O_CREAT | O_TRUNC, 0640);
    if(fp == -1)
    {
        return 0;
    }

    memset(buf, 0, 1024);
    int pos = 0;
    for(int j=0;j<pEQData->indicatorArray.nSize;j++)
    {
        sprintf(buf+pos,"%s ", pEQData->indicatorArray.pChArray[j].pChar);
        pos = pos + pEQData->indicatorArray.pChArray[j].nSize;
    }
    sprintf(buf+pos, "%s", "\n");
    write(fp ,buf, strlen(buf));
    
    string slinehead(""), sline("");
    for(int k=0;k<pEQData->dateArray.nSize;k++)
    {
        memset(buf, 0, 1024);
        sline = "";
        for(int j=0;j<pEQData->indicatorArray.nSize;j++)
        {
            EQVARIENT* pEQVarient = (*pEQData)(0,j,k);
            if(pEQVarient)
            {
                sline += eqvalue2string(pEQVarient);
                sline += " ";
            }
        }
        sprintf(buf,"%s\n",sline.c_str());
        write(fp ,buf, strlen(buf));
    }
    close(fp);
    return 0;
}


static std::list<std::string> get_group_codes(std::list<std::string> total); 
static bool UpdateMarketCodes(); 
static std::string GetIndicators(); 
void RegisterCsq(const std::list<std::string> &group_codes); 
/*
 * 
 */

int main(int argc, char** argv) {
    int rc = 0;
#if 1
    ElectionControl election;
    const char *server = "192.168.1.85:2181, 192.168.1.80:2181, 192.168.1.81:2182, 192.168.1.82:2181";
    if (election.Init(server, 30000) == false) {
      printf("Init election failed!\n");
      return -1;
    }
    while (election.Election() == false) {
      printf("Now election\n");
      sleep(10);
    }
    
    if ((rc = user_init(argc, argv)) != 0) {
      printf("user_init error %d\n", rc);
      return -1;
    }
#endif
    // daemon(0, 0);
    const char* szDllPathx64 = "../bin/libEMQuantAPIx64.so";
    const char* szDllPathx86 = "../bin/libEMQuantAPI.so";
    const char* szDllPath = szDllPathx86;
    if (__WORDSIZE == 64)
    {
        szDllPath = szDllPathx64;
    }
    void* pHandle = dlopen(szDllPath,RTLD_LAZY);
    void* Error = dlerror();
    if (Error)
    {
        printf("\n\nOpen Dll failed. error:[%s]",Error);
        return -1;
    }

    emsetserverlistdir = (psetserverlistdir)dlsym(pHandle, "setserverlistdir");
    emstart = (pstart)dlsym(pHandle, "start");
    emstop = (pstop)dlsym(pHandle, "stop");
    emgeterrstring = (pgeterrstring)dlsym(pHandle, "geterrstring");
    emcsd = (pcsd)dlsym(pHandle, "csd");
    emcss = (pcss)dlsym(pHandle, "css");
    emtradedates = (ptradedates)dlsym(pHandle, "tradedates");
    emsector = (psector)dlsym(pHandle, "sector");
    emgetdate = (pgetdate)dlsym(pHandle, "gettradedate");
    emsetcallback = (psetcallback)dlsym(pHandle, "setcallback");
    emcsq = (pcsq)dlsym(pHandle, "csq");
    emcsqcancel = (pcsqcancel)dlsym(pHandle, "csqcancel");
    emreleasedata = (preleasedata)dlsym(pHandle, "releasedata");
    emcsc = (pcsc)dlsym(pHandle, "csc");
    emcst = (pcst)dlsym(pHandle, "cst");
    emctr = (pctr)dlsym(pHandle, "ctr");
    emcsqsnapshot = (pcsqsnapshot)dlsym(pHandle, "csqsnapshot");
    
    Error = dlerror();
    if (Error)
    {
        dlclose(pHandle);
        printf("\n\nDll sym failed. Error:[%s]",Error);
        return -1;
    }
    //设置SeverList.json文件存放目录
    emsetserverlistdir("../bin/");
    
    EQErr error = EQERR_SUCCESS;
    //------------------------------------------------------------------------------
    //请在此处将您的用户名和密码赋值给logInfo
    //------------------------------------------------------------------------------
    std::string name = "eeysj0006";
    std::string password = "";
    EQLOGININFO logInfo;
    memset(logInfo.userName, 0, 255);
    memset(logInfo.password, 0, 255);
    int len1 = name.length() < 254 ? name.length() : 254;
    int len2 = password.length() < 254 ? password.length() : 254;
    strncpy(logInfo.userName, name.c_str(), len1);
    strncpy(logInfo.password, password.c_str(), len2);
 
    //初始化和设置日志回调以及登陆  option参数"TestLatency=0"服务器不测速  "TestLatency=1"服务器测速选择最优(会有个服务器测速的过程)
    error = emstart(&logInfo, "TestLatency=0,ForceLogin=1", write2Log);
    if(error != EQERR_SUCCESS)
    {
        printf("Login fail.\n");
        return -1;
    }
    printf("%s\n", emgeterrstring(0, eLang_ch));
    printf("Login Success.\n");
    //登陆后设置主回调函数
    emsetcallback(obtainCallback);
#if 0
    //ctr test
    printf("*************ctr test*************\n");
    EQCTRDATA* pCtrData = NULL;
    error = emctr("INDEXCOMPOSITION",NULL, "IndexCode=000001.SH,EndDate=2017-01-13", pCtrData); 
    if(error == EQERR_SUCCESS)
    {
            for(int j=0;j<pCtrData->column;j++)
            {
                    printf("%s ", pCtrData->indicatorArray.pChArray[j].pChar);
            }
            printf("\n");

            for(int i=0;i<pCtrData->row;i++)
            {
                    for(int j=0;j<pCtrData->column;j++)
                    {
                            EQVARIENT* pEQVarient = (*pCtrData)(i,j);
                            if(pEQVarient)
                            {
                                    string s = eqvalue2string(pEQVarient);
                                    printf("%s ", s.c_str());
                            }
                    }
                    printf("\n");
            }

            emreleasedata(pCtrData);
    }
    
    EQDATA* pData = NULL;
    
    //csqsnapshot test
    printf("*************csqsnapshot test*************\n");
    error = emcsqsnapshot("000005.SZ,600602.SH,600652.SH,600653.SH,600654.SH,600601.SH,600651.SH,000004.SZ,000002.SZ,000001.SZ,000009.SZ", "PRECLOSE,OPEN,HIGH,LOW,NOW,AMOUNT", "", pData);
    if(error == EQERR_SUCCESS)
    {
            string slinehead(""), sline("");
            if(pData->dateArray.nSize == 1)
            {
                    slinehead = pData->dateArray.pChArray[0].pChar;
                    slinehead += " ";
                    for(int i=0;i<pData->codeArray.nSize;i++)
                    {
                            sline += string(pData->codeArray.pChArray[i].pChar);
                            sline += " ";

                            for(int j=0;j<pData->indicatorArray.nSize;j++)
                            {
                                    if(i == 0)
                                    {
                                            slinehead += string(pData->indicatorArray.pChArray[j].pChar);
                                            slinehead += " ";

                                            if(j == pData->indicatorArray.nSize-1)
                                            {
                                                    printf("%s\r\n",slinehead.c_str());
                                            }
                                    }

                                    EQVARIENT* pEQVarient = (*pData)(i,j,0);
                                    if(pEQVarient)
                                    {
                                            sline += eqvalue2string(pEQVarient);
                                            sline += " ";
                                    }
                            }
                            sline += "\r\n";
                    }
                    printf("%s",sline.c_str());
            }

            emreleasedata(pData);
    }
    
    //csc  test
    //"DATE,TIME,HIGH,OPEN,LOW,CLOSE,VOLUME,AMOUNT,HQOI,CHANGEOI,CLEAR,CHANGE,PCTCHANGE,BIAS,BOLL,DMI,EXPMA,KDJ,MA,MACD,RSI"
    printf("*************csc test*************\n");
    error = emcsc("300059.SZ", "DATE,TIME,HIGH,OPEN,LOW,CLOSE,VOLUME,AMOUNT,HQOI,CHANGEOI,CLEAR,CHANGE,PCTCHANGE", "20160810", "20160811", NULL, pData);
    if(error == EQERR_SUCCESS)
    {
            for(int k=0;k<pData->dateArray.nSize;k++)
            {
                    for(int j=0;j<pData->indicatorArray.nSize;j++)
                    {
                            EQVARIENT* pEQVarient = (*pData)(0,j,k);
                            if(pEQVarient)
                            {
                                    string s = eqvalue2string(pEQVarient);
                                    printf("%s ",s.c_str());
                            }
                    }
                    printf("\n");
            }
            emreleasedata(pData);
    }

    const char* codes="000002.SZ,300059.SZ";
    const char* indicator = "OPEN,CLOSE,HIGH";
    //csd  test
    printf("*************csd test*************\n");
    pData = NULL;
    error = emcsd(codes, indicator, "2016/01/10", "2016/04/13", "Period=1,Adjustflag=1,Curtype=1,Pricetype=1,Type=2", pData);
    if(error == EQERR_SUCCESS)
    {
        for(int j=0;j<pData->indicatorArray.nSize;j++)
        {
            for(int i=0;i<pData->codeArray.nSize;i++)
            {
                for(int k=0;k<pData->dateArray.nSize;k++)
                {
                    printf("%s %s %s : ",pData->indicatorArray.pChArray[j].pChar, pData->dateArray.pChArray[k].pChar, pData->codeArray.pChArray[i].pChar);

                    EQVARIENT* pEQVarient = (*pData)(i,j,k);
                    if(pEQVarient)
                    {
                        string s = eqvalue2string(pEQVarient);
                        printf("%s\n",s.c_str());
                    }
                }
            }
        }
        emreleasedata(pData);
        pData = NULL;
    }

  
    //css  test
    printf("*************css test*************\n");
    pData = NULL;
    error = emcss(codes, indicator, "Period=1,AdjustFlag=1,CurType=1,PriceType=1,ReportDate=20140930,TradeDate=20160217,StartDate=2015/02/10,EndDate=2015/03/10,ItemsCode=9,type=1,Cycle=1", pData);
    if(error == EQERR_SUCCESS)
    {
        for(int j=0;j<pData->indicatorArray.nSize;j++)
        {
            for(int i=0;i<pData->codeArray.nSize;i++)
            {
                for(int k=0;k<pData->dateArray.nSize;k++)
                {
                    printf("%s %s: ",pData->indicatorArray.pChArray[j].pChar, pData->codeArray.pChArray[i].pChar);

                    EQVARIENT* pEQVarient = (*pData)(i,j,k);
                    if(pEQVarient)
                    {
                        string s = eqvalue2string(pEQVarient);
                        printf("%s\n",s.c_str());
                    }
                }
            }
        }
        emreleasedata(pData);
        pData = NULL;
    }

    //tradedates test
    printf("*************tradedates test*************\n");
    pData = NULL;
    error = emtradedates("2016-3-14", "2016-6-1", "Period=1,Order=1,Market=CNSESH", pData);
    if(error == EQERR_SUCCESS)
    {
        for(int j=0;j<pData->indicatorArray.nSize;j++)
        {
            for(int i=0;i<pData->codeArray.nSize;i++)
            {
                for(int k=0;k<pData->dateArray.nSize;k++)
                {
                    printf("%s %s %s : ",pData->indicatorArray.pChArray[j].pChar, pData->dateArray.pChArray[k].pChar, pData->codeArray.pChArray[i].pChar);

                    EQVARIENT* pEQVarient = (*pData)(i,j,k);
                    if(pEQVarient)
                    {
                        string s = eqvalue2string(pEQVarient);
                        printf("%s\n",s.c_str());
                    }
                }
            }
        }
        emreleasedata(pData);
        pData = NULL;
    }

    //getdate test
    printf("*************getdate test*************\n");
    pData = NULL;
    error =  emgetdate("20160426", -3, "Market=CNSESH", pData);
    if(error == EQERR_SUCCESS)
    {
        for(int j=0;j<pData->indicatorArray.nSize;j++)
        {
            for(int i=0;i<pData->codeArray.nSize;i++)
            {
                for(int k=0;k<pData->dateArray.nSize;k++)
                {
                    printf("%s %s %s : ",pData->indicatorArray.pChArray[j].pChar, pData->dateArray.pChArray[k].pChar, pData->codeArray.pChArray[i].pChar);

                    EQVARIENT* pEQVarient = (*pData)(i,j,k);
                    if(pEQVarient)
                    {
                        string s = eqvalue2string(pEQVarient);
                        printf("%s\n",s.c_str());
                    }
                }
            }
        }
        emreleasedata(pData);
        pData = NULL;
    }

    //sector test
    printf("*************sector test*************\n");
    pData = NULL;
    error =  emsector("011001001001", "2016-07-25", NULL, pData);
    if(error == EQERR_SUCCESS)
    {
        for(int i=0;i<pData->codeArray.nSize;i++)
        {
                for(int j=0;j<pData->indicatorArray.nSize;j++)
                {
                        for(int k=0;k<pData->dateArray.nSize;k++)
                        {
                                //printf("%s %s %s : ",pData->indicatorArray.pChArray[j].pChar, pData->dateArray.pChArray[k].pChar, pData->codeArray.pChArray[i].pChar);

                                EQVARIENT* pEQVarient = (*pData)(i,j,k);
                                if(pEQVarient)
                                {
                                        string s = eqvalue2string(pEQVarient);
                                        printf("%s   ",s.c_str());
                                }
                        }
                }
                printf("\n");
        }
        emreleasedata(pData);
        pData = NULL;
    }
    //cst test
    printf("*************cst test*************\n");
    const char* cstindicator = "OPEN,HIGH,LOW,CLOSE,NOW,AMOUNT,VOLUME,PRECLOSE,HIGHLIMIT,LOWLIMIT,BUYPRICE1,BUYVOLUME1,SELLPRICE1,SELLVOLUME1,BUYPRICE2,BUYVOLUME2,SELLPRICE2,SELLVOLUME2,BUYPRICE3,BUYVOLUME3,SELLPRICE3,SELLVOLUME3,BUYPRICE4,BUYVOLUME4,SELLPRICE4,SELLVOLUME4,BUYPRICE5,BUYVOLUME5,SELLPRICE5,SELLVOLUME5,BUYPRICE6,BUYVOLUME6,SELLPRICE6,SELLVOLUME6,BUYPRICE7,BUYVOLUME7,SELLPRICE7,SELLVOLUME7,BUYPRICE8,BUYVOLUME8,SELLPRICE8,SELLVOLUME8,BUYPRICE9,BUYVOLUME9,SELLPRICE9,SELLVOLUME9,BUYPRICE10,BUYVOLUME10,SELLPRICE10,SELLVOLUME10";
    //时间参数只有时分秒起作用，即只获取当天的日内跳价
    EQID nCstSerialId = emcst(codes, cstindicator, "20161027083000", "20161027153000", NULL, cstCallback, NULL);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
    //csq test
    printf("*************csq test*************\n");
    int tick = 0;
    int day = -1;
    while(1) {
      time_t now = time(NULL);
      struct tm current;
      localtime_r(&now, &current);
      if (current.tm_mday != day) {
        if (UpdateMarketCodes()) {
          day = current.tm_mday; 
        }
        
      }
      sleep(60);
    }

    //退出
    emstop();
    return 0;
}

std::list<std::string>  GetAcodes() {
  EQDATA* Acodes = NULL;
  time_t now = time(NULL);
  struct tm current;
  localtime_r(&now, &current);
  char date[16];
  snprintf(date, sizeof(date), "%04d%02d%02d", current.tm_year + 1900, current.tm_mon + 1, current.tm_mday);
  std::set<std::string> stock_codes;
  if (EQERR_SUCCESS == emsector("001004",date, NULL, Acodes)) {
    EQDATA *pData = Acodes;
    for(int i=0;i<pData->codeArray.nSize;i++) {
      for(int k=0;k<pData->dateArray.nSize;k++) {
        stock_codes.insert(pData->codeArray.pChArray[i].pChar);
      }
    }
    emreleasedata(Acodes);
  }
  std::list<std::string> tmp(stock_codes.begin(), stock_codes.end());
  return tmp;
}

void RegisterCsq(const std::list<std::string> &group_codes) {
    std::string indicators = GetIndicators();
    auto it = group_codes.begin();
    while (it != group_codes.end()) {
      std::string group_code = *it;
      emcsq(group_code.c_str(), indicators.c_str(), "Pushtype=0", csqCallback, NULL);
      it++;
    }
}

std::string GetIndicators() {
  const char* indicatorCSQ = "Time,\
                                Now,\
                                High,\
                                Low,\
                                Open,\
                                PRECLOSE,\
                                AMOUNT,\
                                Roundlot,\
                                PctChange,\
                                Volume,\
                                Amount,\
                                VolumeRatio,\
                                CommissionDiff";
    char deal_csq [1024];
    const char *buy_price = "BuyPrice";
    const char *sell_price = "SellPrice";
    const char *buy_volume = "BuyVolume";
    const char *sell_volume = "SellVolume";
    const char *deals[] = {"BuyPrice", "SellPrice", "BuyVolume", "SellVolume"};
    std::string indicators(indicatorCSQ);
    for (int j = 0; j < 4; j++) {
      char tmp[16];
      for (int i = 0; i < 10; i ++) {
        indicators.append(",");
        sprintf(tmp, ",%s%d",deals[j], i + 1);
        indicators.append(tmp); 
      }
    }
  return indicators;                            
}

bool UpdateMarketCodes() {
  std::list<std::string> codes = GetAcodes();
  if (codes.empty()) {
    printf("Error fetch acodes");
    return false;
  }
  emcsqcancel(0);
  std::list<std::string> group_codes = get_group_codes(codes);
  RegisterCsq(group_codes); 
  return true;
}

std::list<std::string> get_group_codes(std::list<std::string> total) {
  std::list<std::string> result;
  int size_per_group = 100;
  auto it = total.begin();
  std::string group_codes;
  int count = 0;
  while (it != total.end()) {
    group_codes.append(*it++);
    if (++count == size_per_group) {
      count = 0;
      result.push_back(group_codes);
      printf("add %s", group_codes.c_str());
      group_codes.clear();
    } else if (it != total.end()) {
      group_codes.append(",");
    }
  }
  if (group_codes.length()) {
    result.push_back(group_codes);
  }
  return result;
}

