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

bool StockLatestInfo::MergeData(StockInfo *info,
                                StockInfo *store_info) {
  std::string value;
  std::string store_value;
  bool r = false;
  for (int i = 0; i < EAST_INDICTORS_NUM; i++) {
    bool r = info->GetIndex(i, &value);
    bool store_ok = store_info->GetIndex(i, &store_value);
    if (!r || !store_ok) {
      return false;
    }
    
    if (value == NULL_INDICTOR) {
      info->SetIndex(i, store_value);
    } else {
      store_info->SetIndex(i, value);
      r = true;
    }
  }
  return true;
}

bool StockLatestInfo::MergeFullInfo(StockInfo *info) {
  std::lock_guard<std::mutex> lock_guard(mutex_);
  std::string code = info->code();
  auto it = stock_map_.find(code);
  if (it == stock_map_.end()) {
    stock_map_[code] = *info;
    return true;
  }
  bool r = MergeData(info, &it->second);
  return r;
}

} // namespace stock_info
