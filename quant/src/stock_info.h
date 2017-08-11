#ifndef INCLUDE_STOCK_INFO
#define INCLUDE_STOCK_INFO
#include <mutex>
#include <string>
#include <unordered_map>
#define EAST_INDICATORS_NUM 52
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
  
  void MergeFullInfo(StockInfo *info) {
    std::string code = info->code;
    auto it = stock_map_.find(code);
    if (it == stock_map_.end()) {
      stock_map_[code] = *info;
      return;
    }
    for (int i = 0; i < EAST_INDICATORS_NUM; i++) {
      if (info->indicators[i] == "--") {
        info->indicators[i] = it->second.indicators[i];
      } else {
        it->second.indicators[i] = info->indicators[i];
      }
    }
  }

  StockInfo GetLatestInfo(std::string code) {
    return stock_map_[code];
  }

  void SetLatestInfo(std::string code, const StockInfo &info) {
    stock_map_[code] = info;
  }

 private:
  static StockLatestInfo *instance_;
  std::unordered_map<std::string, StockInfo> stock_map_;
  static std::mutex mutex_;
};

}

#endif
