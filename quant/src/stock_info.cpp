#include "stock_info.h"

namespace stock_info {
std::mutex StockLatestInfo::mutex_;
StockLatestInfo* StockLatestInfo::GetInstance() {
  static StockLatestInfo info;
  return &info;
}

bool StockLatestInfo::UpdateCssInfo(const std::map<std::string, CssInfo> &css_map) {
  LOG(INFO) << "UpdateCssInfo: " << css_map.size();
  std::lock_guard<std::mutex> lock_guard(mutex_);
  auto css_it = css_map.begin();
  while (css_it != css_map.end()) {
    auto css_info = css_it->second;
    std::string code = css_it->first;
    auto it = stock_map_.find(code);
    if (it != stock_map_.end()) {
      StockInfo &info = it->second;
      info.UpdateCssInfo(css_info);
    } else {
      StockInfo info(code);
      info.set_css_info(css_info);
      stock_map_[code] = info; 
    }
    css_it++;
  }
  return true;
}

bool StockLatestInfo::UpdateCsqInfo(std::string code, const CsqInfo &info, StockInfo *stock_info) {
  std::lock_guard<std::mutex> lock_guard(mutex_);
  auto it = stock_map_.find(code);
  if (it == stock_map_.end()) {
      StockInfo new_info(code);
      new_info.set_csq_info(info);
      stock_map_[code] = new_info; 
  } else {
      StockInfo &csq_info = it->second;
      csq_info.UpdateCsqInfo(info);
  }
  *stock_info = stock_map_[code];
  return true;
}

} // namespace stock_info
