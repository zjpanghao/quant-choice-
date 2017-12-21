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

#include <zookeeper/zookeeper.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>
#include <zookeeper/zookeeper_log.h>
#include "election.h"
#include "stock_info.h"
#include "login.h"
#include "csq_handle.h"
#include "css_handle.h"
#include "cst_handle.h"
#include "csc_handle.h"
#include "csqshot_handle.h"
#include "quant_mon.h"
#include "quant_code.h"
#include "redis_pool.h"
#include "quant_pack.h"
#include "quant_kafka.h"


using namespace std;
//  global varibles
volatile bool csq_conn_state_g = false;

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
    return 0;
}

//实时行情回调函数示例

static std::list<std::string> GetGroupCodes(std::list<std::string> total); 
static bool UpdateMarketCodes(); 
static std::string GetIndictors(); 
void RegisterCsq(const std::list<std::string> &group_codes); 
bool SendCsqShot(quant::CsqHandle* handle); 
void UpdateCss();
void UpdateCssThd();
std::list<std::string>  GetAcodes();
std::list<std::string>  GetSingleAcodes(const std::list<std::string> &acodes);

void glog_init(std::string name) {
  google::InitGoogleLogging(name.c_str());
  if (access("telelog", 0) != 0)
    system("mkdir telelog");
  google::SetLogDestination(google::INFO, "telelog/info");	
  google::SetLogDestination(google::WARNING, "telelog/warn");   
  google::SetLogDestination(google::ERROR, "telelog/error");   
  FLAGS_logbufsecs = 0;
}

/*
 * 
 */

int main(int argc, char** argv) {
    int rc = 0;
    glog_init(argv[0]);
#if 0
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
    //设置SeverList.json文件存放目录
    setserverlistdir("../bin/");
    quant::Login login;
    if (login.start()) {
      LOG(ERROR) << "Login error";
      return -1;
    }
    LOG(INFO) << "Login Success.";
    //登陆后设置主回调函数
    setcallback(obtainCallback);
    

    int tick = 0;
    int day = -1;
    bool csq_reg = false;
    const int UPDATE_HOUR = 9;
    const int UPDATE_MINUTE = 15;
    auto TimeUpdate = [] (int hour, int min) { return hour >= UPDATE_HOUR && min >= UPDATE_MINUTE;};
    // std::thread update(UpdateNonvariableIndictorThd);
    // update.detach();
    // UpdateMarketCodes(); 
    std::shared_ptr<quant::QuantStore> store(new quant::QuantKafka("192.168.1.74:9092", "east_wealth_test"));
    store->init();
    stock_info::StockLatestInfo::GetInstance()->set_store(store.get());
    RedisPool pool("192.168.1.72", 7481, 1, 20, "4", "ky_161019");
    auto indictors = quant::Indictors::getInstance();
    quant::AcodesControl *codeCnt 
        = quant::AcodesControl::GetInstance();
    codeCnt->set_pool(&pool);
    quant::AsynHandle *csq_handle(
        new quant::CsqHandle(indictors->getCsq(), 0));
    //quant::AsynHandle *cst_handle(
    //    new quant::CstHandle("TIME, HighLimit, LowLimit"));
    // cst_handle->Update();
    std::string acodes;
    // makdeshare acodes ok
    while (!codeCnt->updateAcodes()) {
      sleep(10);
    }
    quant::SynHandle *css_handle(
        new quant::CssHandle("Tradestatus"));
    css_handle->Update();
    quant::SynHandle *csqshot_handle(
        new quant::CsqshotHandle(indictors->getCsq()));
    csqshot_handle->Update();
    
    LOG(INFO) << "fetch codes ok";
    //SendCsqShot(csq_handle);
    quant::QuantMon  mon;
    mon.addMon(csq_handle);
    mon.start();
    bool day_update = true;
    while(1) {
      struct tm  current;
      time_t now = time(NULL);
      localtime_r(&now, &current);
      if (current.tm_hour == 1) {
        day_update = false;
      }

      if (!day_update && TimeUpdate(current.tm_hour, current.tm_min)) {
        if (codeCnt->updateAcodes()) {
          // SendCsqShot(csq_handle);
          if(!css_handle->Update()) {
            LOG(ERROR) << "Css update error";
          }
          if (!csqshot_handle->Update()) {
            LOG(ERROR) << "Csq shot update error";
          }
          mon.forceUpdate();
          day_update = true;
        }
      } 
      sleep(30);
    }
    //退出
    stop();
    return 0;
}

std::string GetIndictors() {
  const char* indicatorCSQ = 
        "Time,Now,High,Low,Open,PRECLOSE,Roundlot,Volume,Amount"; //,Tradestatus";
    char deal_csq [1024];
    const char *deals[] = {"BuyPrice", "BuyVolume", "SellPrice", "SellVolume"};
    std::string indicators(indicatorCSQ);
#if 0
    for (int j = 0; j < 4; j++) {
      char tmp[16];
      for (int i = 0; i < 5; i ++) {
        sprintf(tmp, ",%s%d",deals[j], i + 1);
        indicators.append(tmp); 
      }
    }
#endif
  return indicators;                            
}



