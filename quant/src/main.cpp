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
#include <map>
#include <string>
#include <set>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <thread>
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
#include "stock_info.h"


using namespace std;
typedef void  (*psetserverlistdir)(const char* dir);
typedef EQErr (*pmanualactivate)(EQLOGININFO* pLoginInfo, const char* options, logcallback pfnCallback);
typedef EQErr (*psetproxy)(ProxyType type, const char* proxyip, unsigned short proxyport, bool verify, const char* proxyuser, const char* proxypwd);
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
typedef EQErr (*pcps)(const char* cpsCodes, const char* cpsIndicators, const char* cpsConditions, const char* cpsOptions, EQDATA*& pEQData);

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
pmanualactivate   emmanualactivate = NULL;
psetproxy         emsetproxy = NULL;
pcps          emcps = NULL;

//  global varibles
volatile bool csq_conn_state_g = false;

bool GetMarketData(EQDATA *pData, std::map<std::string, CsqInfo> *market_data); 
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
    LOG(WARNING) << log;
}

//取EQVARIENT值示例
std::string eqvalue2string(const EQVARIENT* pEQVarient)
{
    if(!pEQVarient)
    {
        return "--";
    }

    char z[64] = {0};
    // memset(z, 0, 4096);
    std::string s = "--";
    switch(pEQVarient->vtype)
    {
        //以下为了输出便于观察，做了字符宽度限定处理，实际可参考需求处理
    case eVT_null:
        snprintf(z, sizeof(z), "%s", "--");
        s = z;
        break;
    case eVT_char:
        snprintf(z, sizeof(z), "%c", pEQVarient->unionValues.charValue);
        s = z;
        break; 
    case eVT_bool:
        snprintf(z, sizeof(z), "%c", pEQVarient->unionValues.boolValue ? "true" : "false");
        s = z;
        break;
    case eVT_int:
        snprintf(z, sizeof(z), "%d", pEQVarient->unionValues.intValue);
        s = z;
        break;
    case eVT_uInt:
        snprintf(z, sizeof(z), "%u", pEQVarient->unionValues.uIntValue);
        s = z;
        break;
    case eVT_int64: 
        snprintf(z, sizeof(z), "%lld", pEQVarient->unionValues.int64Value);
        s = z;
        break;
    case eVT_uInt64: 
        snprintf(z, sizeof(z), "%llu", pEQVarient->unionValues.uInt64Value);
        s = z;
        break;
    case eVT_float:  
        snprintf(z, sizeof(z), "%.6f", pEQVarient->unionValues.floatValue);
        s = z;
        break;
    case eVT_double: 
        snprintf(z, sizeof(z), "%.6lf", pEQVarient->unionValues.doubleValue);
        s = z;
        break;
    case eVT_asciiString:
        snprintf(z, sizeof(z), "%s", pEQVarient->eqchar.pChar);
        s = z;
        break;
    case eVT_unicodeString: 
        snprintf(z, sizeof(z), "%s", pEQVarient->eqchar.pChar);
        s = z;
        break;
    case eVT_short:     
        snprintf(z, sizeof(z), "%d", pEQVarient->unionValues.shortValue);
        s = z;
        break;
    case eVT_ushort:     
        snprintf(z, sizeof(z), "%u", pEQVarient->unionValues.uShortValue);
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
    struct tm current;
    localtime_r(&tt, &current);
    tm *t = &current;
    char buf[512];
    snprintf(buf, sizeof(buf), "[%02d:%02d:%02d]Received [%d] callback: msgType=%d, err=%d, requestID=%d, serialID=%d.\n", t->tm_hour, t->tm_min, t->tm_sec, times++, pMsg->msgType, pMsg->err, pMsg->requestID, pMsg->serialID);
    if(pMsg->msgType == eMT_err && pMsg->err != EQERR_SUCCESS) {
      printf("error %d\r\n", pMsg->err);
      return 0;
    }

    LOG(INFO) << buf;
    if(!pMsg->pEQData)
    {
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
                       LOG(INFO) << slinehead;
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
        LOG(INFO) << sline;
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
                           LOG(INFO) << slinehead;
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
            LOG(INFO) << sline;
        }
    }

 //   printf("****************************************\n");
    return 0;
}

//实时行情回调函数示例
int csqCallback(const EQMSG* pMsg, LPVOID lpUserParam)
{
    static int times = 1;
    
    time_t tt = time(NULL);
    struct tm current;
    localtime_r(&tt, &current);
    tm *t = &current;

 //   printf("****************************************\n");
    char buf[512];
    snprintf(buf, sizeof(buf), "[%02d:%02d:%02d]Received [%d] callback: msgType=%d, err=%d, requestID=%d, serialID=%d.\n", t->tm_hour, t->tm_min, t->tm_sec, times++, pMsg->msgType, pMsg->err, pMsg->requestID, pMsg->serialID);
     LOG(INFO) << buf;
    if (pMsg->err == 10002004 || pMsg->err == 10002007) {

      csq_conn_state_g = false;
      LOG(ERROR) << "Csq receive error:" << pMsg->err; 
      return 0;
    }

    if (pMsg->err) {
      LOG(ERROR) << "Csq receive error:" << pMsg->err; 
      return 0;
    }

    csq_conn_state_g = true;
    if(!pMsg->pEQData)
    {
        return 0;
    }

    EQDATA *pEQData = pMsg->pEQData;
    string slinehead(""), sline("");
    std::map<std::string, CsqInfo> market_info;
    if(pEQData->dateArray.nSize != 1) {
      LOG(ERROR) << "error date size";
      return 0;
    } 
   
    if (GetMarketData(pEQData, &market_info) == false) {
      LOG(ERROR) << "recv data error";
      return 0;
    }
    LOG(INFO) << "Total stocks=" << pEQData->codeArray.nSize;
    LOG(INFO ) << market_info.size(); 
    user_recv(market_info);
    return 0; 
}

static std::list<std::string> GetGroupCodes(std::list<std::string> total); 
static bool UpdateMarketCodes(); 
static std::string GetIndicators(); 
void RegisterCsq(const std::list<std::string> &group_codes); 
bool GetCsqShot(const std::list<std::string> &group_codes); 
void UpdateCss();
void UpdateCssThd();
std::list<std::string>  GetAcodes();
std::list<std::string>  GetSingleAcodes(const std::list<std::string> &acodes);

/*
 * 
 */

int main(int argc, char** argv) {
    int rc = 0;
    if ((rc = user_init(argc, argv)) != 0) {
      printf("user_init error %d\n", rc);
      return -1;
    }
#if 1
    ElectionControl election;
    const char *server = "192.168.1.74:2181";
    if (election.Init(server, 500) == false) {
      printf("Init election failed!\n");
      return -1;
    }
    while (election.Election() == false) {
      printf("Now election\n");
      sleep(10);
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
    emmanualactivate = (pmanualactivate)dlsym(pHandle, "manualactivate");
    emsetproxy = (psetproxy)dlsym(pHandle, "setproxy");
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
    emcps = (pcps)dlsym(pHandle, "cps");
    
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
    EQLOGININFO logInfo;
    memset(&logInfo, 0, sizeof(logInfo));
 
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
    //csq test
    printf("*************csq test*************\n");
    UpdateCss();
    std::thread css_thd(UpdateCssThd);
    css_thd.detach();
    int tick = 0;
    int day = -1;
    bool csq_reg = false;
    const int UPDATE_HOUR = 9;
    const int UPDATE_MINUTE = 15;
    bool day_update = false;
    auto TimeUpdate = [] (int hour, int min) { return hour >= UPDATE_HOUR && min >= UPDATE_MINUTE;};
    // std::thread update(UpdateNonvariableIndictorThd);
    // update.detach();

    while(1) {
      time_t now = time(NULL);
      struct tm current;
      localtime_r(&now, &current);
      if (current.tm_hour == 0) {
        day_update = false; 
      }
      // jump weekends and legal holidays 
      if (!quant_util::DateControl::GetInstance()->IsTradeDay(current)) {
        sleep(60);
        continue;
      } 

      if (!csq_conn_state_g || (!day_update && TimeUpdate(current.tm_hour, current.tm_min))|| !csq_reg) {
        if (UpdateMarketCodes()) {
          csq_reg = true;
          if (TimeUpdate(current.tm_hour, current.tm_min)) {
            day_update = true;
          }
        }
      }
      sleep(60);
    }

    //退出
    emstop();
    return 0;
}

std::list<std::string> GetSingleAcodes(const std::list<std::string> & acodes) {
  auto it = acodes.begin();
  const std::string indexs[] = {"000001.SH", "399001.SZ", "000300.SH", "399006.SZ"};
  std::set<std::string> index_set(indexs, indexs + sizeof(indexs) / sizeof(std::string));
  std::list<std::string> tmp;
  while (it != acodes.end()) {
    if (index_set.count(*it) == 0) {
      tmp.push_back(*it);
    } 
    it++;
  }
  return tmp;
}
 
std::list<std::string>  GetAcodes() {
  int max_codes = 30000;
  EQDATA* Acodes = NULL;
  time_t now = time(NULL);
  struct tm current;
  localtime_r(&now, &current);
  char date[16];
  snprintf(date, sizeof(date), "%04d%02d%02d", current.tm_year + 1900, current.tm_mon + 1, current.tm_mday);
  std::set<std::string> stock_codes;
  const std::string indexs[] = {"000001.SH", "399001.SZ", "000300.SH", "399006.SZ"};
  for (const std::string &idx : indexs) {
    stock_codes.insert(idx);
  }
  if (EQERR_SUCCESS == emsector("001004",date, NULL, Acodes)) {
    EQDATA *pData = Acodes;
    for(int i=0;i<pData->codeArray.nSize;i++) {
      for(int k=0;k<pData->dateArray.nSize;k++) {
        if (stock_codes.size() < max_codes) {
          stock_codes.insert(pData->codeArray.pChArray[i].pChar);
        }
          
      }
    }
    emreleasedata(Acodes);
  }
  std::list<std::string> tmp(stock_codes.begin(), stock_codes.end());
  LOG(INFO) << "Acodes size:" << tmp.size();
  return tmp;
}

bool GetCsqShot(const std::list<std::string> &group_codes) {
    std::string indicators = GetIndicators();
    auto it = group_codes.begin();
    while (it != group_codes.end()) {
      std::string group_code = *it;
      EQDATA* pCtrData = NULL;
      int rc = emcsqsnapshot(group_code.c_str(), indicators.c_str(), "", pCtrData);
      if (rc) {
        LOG(ERROR) << "csq error" << rc;
        return false;
      }
      if (pCtrData) {
        std::map<std::string, CsqInfo> market_info;
        GetMarketData(pCtrData, &market_info);
        LOG(INFO) << "CSQ OK";
        emreleasedata(pCtrData);
        user_recv(market_info);
      }
      it++;
    }
    return true;
}

void RegisterCsq(const std::list<std::string> &group_codes) {
    std::string indicators = GetIndicators();
    auto it = group_codes.begin();
    while (it != group_codes.end()) {
      std::string group_code = *it;
#ifdef USER_DELTA
      std::string push_type = "Pushtype=0";
#else
      std::string push_type = "Pushtype=1";
#endif
      // printf("%s", push_type.c_str());
      LOG(INFO) << emcsq(group_code.c_str(), indicators.c_str(), push_type.c_str(), csqCallback, NULL);
      it++;
    }
}

std::string GetIndicators() {
  const char* indicatorCSQ = 
        "Time,Now,High,Low,Open,PRECLOSE,Roundlot,Volume,Amount"; //,Tradestatus";
    char deal_csq [1024];
    const char *deals[] = {"BuyPrice", "BuyVolume", "SellPrice", "SellVolume"};
    std::string indicators(indicatorCSQ);
    for (int j = 0; j < 4; j++) {
      char tmp[16];
      for (int i = 0; i < 10; i ++) {
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
  
  if (0 != emcsqcancel(0)) {
    LOG(ERROR) << "Cancel error";
    return false;
  }
  
  quant_util::AcodesControl::GetInstance()->set_acode(GetSingleAcodes(codes)); 
  std::list<std::string> group_codes = GetGroupCodes(codes);
  if (group_codes.empty())
    return false;

#ifdef USER_DELTA
  if (!GetCsqShot(group_codes))
    return false;
#endif

  RegisterCsq(group_codes); 
  return true;
}

std::list<std::string> GetGroupCodes(std::list<std::string> total) {
  std::list<std::string> result;
  int size_per_group = 1000;
  auto it = total.begin();
  std::string group_codes;
  int count = 0;
  while (it != total.end()) {
    group_codes.append(*it++);
    if (++count == size_per_group) {
      count = 0;
      result.push_back(group_codes);
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

bool GetMarketData(EQDATA *pData, std::map<std::string, CsqInfo> *market_data) {
    EQDATA *pEQData = pData;
    if (!pEQData)
      return false;
    std::map<std::string, CsqInfo> &market_info = *market_data;
    if(pEQData->dateArray.nSize != 1)
      return false;
    for(int i=0;i<pEQData->codeArray.nSize;i++) {
      const char *stock_code = pEQData->codeArray.pChArray[i].pChar;
      if (!stock_code) {
        continue;
      }
      CsqInfo csq_info;
      for(int j=0;j<pEQData->indicatorArray.nSize;j++) {
        
        EQVARIENT* pEQVarient = (*pEQData)(i,j,0);
        if(pEQVarient) {
          csq_info.SetIndex(j, eqvalue2string(pEQVarient));
        } else {
          csq_info.SetIndex(j, NULL_INDICTOR);
        }
      }
      market_info.insert(std::make_pair(stock_code, csq_info));
    }
    LOG(INFO) << "The size" << market_info.size();
    return true;
}

void UpdateCss() {
    std::string indicators = "Tradestatus";
    std::map<std::string, CssInfo> css_map;
    std::list<std::string> codes = quant_util::AcodesControl::GetInstance()->acode();
    if (codes.size() == 0)
     codes = GetSingleAcodes(GetAcodes());
    std::list<std::string> group_codes = GetGroupCodes(codes);
    auto it = group_codes.begin();
    while (it != group_codes.end()) {
      std::string group_code = *it;
      EQDATA* pData = NULL;
      std::string option = "";
      EQErr error = emcss(group_code.c_str(), indicators.c_str(), NULL, pData);
      if(error != EQERR_SUCCESS) {
        it++;
        continue;
      }
      for(int j=0;j<pData->indicatorArray.nSize;j++) {
        for(int i=0;i<pData->codeArray.nSize;i++) {
          for(int k=0;k<pData->dateArray.nSize;k++) {
            std::string code = pData->codeArray.pChArray[i].pChar;
            //printf("%s %s: ",pData->indicatorArray.pChArray[j].pChar, pData->codeArray.pChArray[i].pChar);
            EQVARIENT* pEQVarient = (*pData)(i,j,k);
            if(pEQVarient) {
              string s = eqvalue2string(pEQVarient);
              // printf("%s\n",s.c_str());
              css_map[code].SetIndex(j, s);
              // LOG(INFO) << "css " << code << s;
            }
          }
        }
      }
      emreleasedata(pData);
      it++;
    }
    stock_info::StockLatestInfo::GetInstance()->UpdateCssInfo(css_map);
}


void UpdateCssThd() {
  time_t now;
  struct tm current;
  int delay = 30;
  auto NeedCss = [](const struct tm &current) {
    if (current.tm_hour > 15 
        || current.tm_hour < 9 
        || (current.tm_hour == 15 && current.tm_min > 3)
        || (current.tm_hour == 9 && current.tm_min < 15)
        || (current.tm_hour == 11 && current.tm_min > 33) 
        || current.tm_hour == 12) {
      return false;
    }
    return true;
  };

  while (true) {
    sleep(120);
    now = time(NULL);
    localtime_r(&now, &current);
    if (!NeedCss(current)) {
      continue;
    }
    if (!quant_util::DateControl::GetInstance()->IsTradeDay(current)) {
      continue;
    }
    UpdateCss();
  }

}


