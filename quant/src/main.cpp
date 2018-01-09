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
#include "predis/redis_pool.h"
#include "quant_pack.h"
#include "quant_kafka.h"
#include "quant_process.h"
#include "dbpool.h"


using namespace std;
//  global varibles
volatile bool csq_conn_state_g = false;

//转string函数示例
template<typename T>
static std::string to_str(const T &v)
{
    stringstream s;
    s << v;
    return s.str();
}

//日志回调函数示例
static int write2Log(const char* log)
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

static DBPool* InitDbpool(std::string dbserver) {
	/*init dbpool */
	DBPool *pool = DBPool::GetInstance();
	if (pool->PoolInit(dbserver.c_str(), 
      "stock", "stock","stock", 5, 1, 10) < 0) {
	  LOG(ERROR) << "error init dbpool" << dbserver;
	  return NULL;
	}
	return pool;
}

/*
 * 
 */

int main(int argc, char** argv) {
    int rc = 0;
    glog_init(argv[0]);
    DBPool *db_pool = InitDbpool("192.168.1.88");
#if 1
    ElectionControl election;
    const char *server = "192.168.1.74:2181";
    if (election.Init(server, 5000) == false) {
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
    quant::Login *login = quant::Login::getInstance();
    int loginCount = 0;
    while (!login->checkStart()) {
      LOG(ERROR) << "Login error times "<< loginCount++;
      sleep(10);
    }
    LOG(INFO) << "Login Success.";
    //登陆后设置主回调函数
    setcallback(obtainCallback);
    

    int tick = 0;
    int day = -1;
    bool csq_reg = false;
    const int UPDATE_HOUR = 9;
    const int UPDATE_MINUTE = 15;
    const int DAYLINE_UPDATE_HOUR = 16;
    const int DAYLINE_UPDATE_MINUTE = 0;
    auto TimeUpdate = [] (int hour, int min) { return hour >= UPDATE_HOUR && min >= UPDATE_MINUTE;};
    auto DayLineTimeUpdate = [] (int hour, int min) { return hour >= DAYLINE_UPDATE_HOUR && min >= DAYLINE_UPDATE_MINUTE;};
    std::shared_ptr<quant::QuantStore> store(new quant::QuantKafka("192.168.1.74:9092", "east_wealth", 2));
    store->init();
    stock_info::StockLatestInfo::GetInstance()->set_store(store.get());
    RedisPool pool("192.168.1.72", 7481, 4, 20, "4", "ky_161019");
    auto indictors = quant::Indictors::getInstance();
    quant::AcodesControl *codeCnt 
        = quant::AcodesControl::GetInstance();
    codeCnt->set_pool(&pool);
    quant::AsynHandle *csq_handle(
        new quant::CsqHandle(indictors->getCsq(), 0));
    std::string acodes;
    while (!codeCnt->updateAcodes()) {
      sleep(10);
    }
    std::string single_codes;
    if (!quant::AcodesControl::GetInstance()->GetSingleCodes(&single_codes)) {
      return -1;
    }

    if (!quant::AcodesControl::GetInstance()->GetAllCodes(&acodes)) {
      return -1;
    }


    quant::SynHandle *css_handle = 
        new quant::CssHandle(indictors->getCssState());
    css_handle->setCodes(single_codes);
    css_handle->Update();
    quant::SynHandle *css_dayline_handle = 
        new quant::CssHandle("CLOSE", new quant::QuantDayLineProcess(db_pool));
    css_dayline_handle->setCodes(acodes);
    quant::SynHandle *csqshot_handle(
        new quant::CsqshotHandle(indictors->getCsq()));
    csqshot_handle->Update();
    LOG(INFO) << "csq update ok";
    quant::QuantMon  mon;
    mon.addMon(csq_handle);
    mon.start();
    bool day_update = true;
    bool dayLineUpdate = false;
    int css_count = 0;
    while(1) {
      if (!login->checkStart()) {
        LOG(INFO) << "check start failed wait";
        sleep(10);
        continue;
      }
      struct tm  current;
      time_t now = time(NULL);
      localtime_r(&now, &current);
      if (current.tm_hour == 3) {
        day_update = false;
        dayLineUpdate = false;
      }

      bool trade_day = quant_util::DateControl::GetInstance()
          ->IsTradeDay(current);
      if (!trade_day) {
        sleep(3600);
        continue;
      }

      if (!day_update && TimeUpdate(current.tm_hour, current.tm_min)) {
        if (codeCnt->updateAcodes()) {
          if (!quant::AcodesControl::GetInstance()
              ->GetAllCodes(&acodes)) {
            LOG(WARNING) << "acodescontrol get allcodes error";
          }

          if (!quant::AcodesControl::GetInstance()
              ->GetSingleCodes(&single_codes)) {
            LOG(ERROR) << "css fetch single codes error";
          }
          css_handle->setCodes(single_codes);
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

      if (!dayLineUpdate && 
           DayLineTimeUpdate(current.tm_hour, current.tm_min)) {
        css_dayline_handle->setCodes(acodes);
        LOG(INFO) << "dayline update" << acodes.size();
        dayLineUpdate = css_dayline_handle->Update();
      }
     
      css_count = (css_count + 1) % 5;
      if (css_count == 3 
          && current.tm_hour >= 9 
          && current.tm_hour <= 15) {
        if(!css_handle->Update()) {
          LOG(ERROR) << "Css update error";
        }
      }
      sleep(30);
    }
    //退出
    stop();
    return 0;
}

