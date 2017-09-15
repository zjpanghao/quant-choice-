#include "stock_info.h"

namespace stock_info {
StockLatestInfo *StockLatestInfo::instance_ = NULL;
std::mutex StockLatestInfo::mutex_;
StockLatestInfo* StockLatestInfo::GetInstance() {
  if (instance_ == NULL) {
    mutex_.lock();
    if (instance_ == NULL) {
      instance_ = new StockLatestInfo();
    }
    mutex_.unlock();
  }
  return instance_;
}

bool StockLatestInfo::CaculateChangeRate(StockInfo *info) {
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

bool StockLatestInfo::UpdateNonvariable(const StockInfo &info) {
  std::lock_guard<std::mutex> lock_guard(mutex_);
  std::string code = info.code;
  auto it = stock_map_.find(code);
  if (it == stock_map_.end()) {
    stock_map_[code] = info;
    return true;
  }
  bool r = false;
  for (int i = 0; i < EAST_INDICATORS_NUM; i++) {
    if (info.indicators[i] != "--" && (i == StockIndictor::OPEN || 
        i == StockIndictor::PRECLOSE)) {
      if (it->second.indicators[i] != info.indicators[i]) {
        it->second.indicators[i] = info.indicators[i];
        LOG(INFO) << "update " << i << "for " << code << "to" << info.indicators[i];
        r = true;
      }
    }
  }
 
  return r;
}

bool StockLatestInfo::MergeFullInfo(StockInfo *info) {
  std::lock_guard<std::mutex> lock_guard(mutex_);
  std::string code = info->code;
  auto it = stock_map_.find(code);
  if (it == stock_map_.end()) {
    stock_map_[code] = *info;
    return true;
  }
  bool r = false;
  for (int i = 0; i < EAST_INDICATORS_NUM; i++) {
    if (info->indicators[i] == "--") {
      info->indicators[i] = it->second.indicators[i];
    } else {
      if (it->second.indicators[i] != info->indicators[i]) {
        it->second.indicators[i] = info->indicators[i];
        r = true;
      }
    }
  }
  return r;
}

} // namespace stock_info
