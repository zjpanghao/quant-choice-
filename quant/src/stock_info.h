#ifndef INCLUDE_STOCK_INFO
#define INCLUDE_STOCK_INFO
#include <mutex>
#include <string>
#include <unordered_map>
#include "glog/logging.h"
#include <set>
#define EAST_INDICATORS_NUM 52


namespace stock_info {
enum StockIndictor {
  TIME,
  NOW,
  HIGH,
  LOW,
  OPEN,
  PRECLOSE,
  ROUNDLOT,
  PCT_CHANGE,
  VOLUME,
  AMOUNT,
  VOLUME_RATIO,
  COMMITION_DIFF
};

struct StockInfo {
  std::string code;
  std::string indicators[EAST_INDICATORS_NUM]; 
};

class StockLatestInfo {
 public:
  static StockLatestInfo *GetInstance() {
    if (instance_ == NULL) {
      mutex_.lock();
      if (instance_ == NULL) {
        instance_ = new StockLatestInfo();
      }
      mutex_.unlock();
    }
    return instance_;
  }
  static bool Exclude(int j) {
    static const std::set<int> exclude = {StockIndictor::PCT_CHANGE, 
                                          StockIndictor::VOLUME_RATIO, 
                                          StockIndictor::COMMITION_DIFF};
    return exclude.count(j) > 0;
  }
  static bool CaculateChangeRate(StockInfo *info) {
    std::string now_price = info->indicators[StockIndictor::NOW];
    std::string pre_close = info->indicators[StockIndictor::PRECLOSE];
    if (now_price.length() == 0 || pre_close.length() == 0) {
      return false;
    }
    double price = atof(now_price.c_str());
    double close = atof(pre_close.c_str());
    if (price < 0.00001 || close < 0.0001)
      return false;
    double rate =  (price - close) / close;
    LOG(INFO) << info->code << " " << rate;
    char tmp[16];
    snprintf(tmp, 16, "%.6lf", rate);
    info->indicators[StockIndictor::PCT_CHANGE] = tmp;
    LOG(INFO) << "set " << tmp;
    return true;
  }   
 
  bool MergeFullInfo(StockInfo *info) {
    std::lock_guard<std::mutex> lock_guard(mutex_);
    std::string code = info->code;
    auto it = stock_map_.find(code);
    if (it == stock_map_.end()) {
      stock_map_[code] = *info;
      return true;
    }
    // makesure only send once each timestamp
    if (info->indicators[StockIndictor::TIME] == "--" ||
        (info->indicators[StockIndictor::TIME] != "80000" 
        && info->indicators[StockIndictor::TIME] == it->second.indicators[StockIndictor::TIME])) {
      return false;
    }

    for (int i = 0; i < EAST_INDICATORS_NUM; i++) {
      if (info->indicators[i] == "--") {
        info->indicators[i] = it->second.indicators[i];
      } else {
        it->second.indicators[i] = info->indicators[i];
      }
    }
    return true;
  }

 private:
  static StockLatestInfo *instance_;
  std::unordered_map<std::string, StockInfo> stock_map_;
  static std::mutex mutex_;
};

}

#endif
