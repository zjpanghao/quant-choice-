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
  static StockLatestInfo *GetInstance(); 
  static bool Exclude(int j) {
    static const std::set<int> exclude = {StockIndictor::PCT_CHANGE, 
                                          StockIndictor::VOLUME_RATIO, 
                                          StockIndictor::COMMITION_DIFF};
    return exclude.count(j) > 0;
  }

  static bool CaculateChangeRate(StockInfo *info); 
 
  bool UpdateNonvariable(const StockInfo &info);

  bool MergeFullInfo(StockInfo *info); 

 private:
  static StockLatestInfo *instance_;
  std::unordered_map<std::string, StockInfo> stock_map_;
  static std::mutex mutex_;
};

}

#endif
