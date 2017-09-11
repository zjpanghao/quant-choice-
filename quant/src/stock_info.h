#ifndef INCLUDE_STOCK_INFO
#define INCLUDE_STOCK_INFO
#include <mutex>
#include <string>
#include <unordered_map>
#include "glog/logging.h"
#define EAST_INDICATORS_NUM 53
namespace stock_info {

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

  static bool CaculateChangeRate(StockInfo *info) {
    std::string now_price = info->indicators[1];
    std::string pre_close = info->indicators[5];
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
    info->indicators[7] = tmp;
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
    if (info->indicators[0] != "80000" && info->indicators[0] == it->second.indicators[0]) {
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
#if 0
  StockInfo GetLatestInfo(std::string code) {
    return stock_map_[code];
  }

  void SetLatestInfo(std::string code, const StockInfo &info) {
    stock_map_[code] = info;
  }
#endif

 private:
  static StockLatestInfo *instance_;
  std::unordered_map<std::string, StockInfo> stock_map_;
  static std::mutex mutex_;
};

}

#endif
