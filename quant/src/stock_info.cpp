#include "stock_info.h"
#include "quant_kafka.h"
namespace stock_info {
std::mutex StockLatestInfo::mutex_;
bool StockLatestInfo::store(const std::string &message) {
  return store_->store(message);
}

bool StockLatestInfo::store(const std::string &key, const std::string &message) {
  return store_->store(key, message);
}
 
StockLatestInfo::StockLatestInfo() {
}

StockLatestInfo* StockLatestInfo::GetInstance() {
  static StockLatestInfo info;
  return &info;
}

bool StockLatestInfo::cssTimeoutSend() {
  time_t now = time(NULL);
  std::lock_guard<std::mutex> lock_guard(mutex_);
  auto it = stock_map_.begin();
  while (it != stock_map_.end()) {
    auto &stock_info = it->second;
    if (stock_info.cssTimeout(now)) {
      std::string message = stock_info.produce_send_message();
      store(stock_info.code(), message);
      LOG(INFO) << "css send " << stock_info.code() << message;
      stock_info.resetCssTimeout(now);
    }
    it++;
  }

  return true;
}

bool StockLatestInfo::UpdateCssInfo(IndictorInfoPtr css_info) {
  std::lock_guard<std::mutex> lock_guard(mutex_);
  std::string code = css_info->code();
  auto it = stock_map_.find(code);
  if (it != stock_map_.end()) {
    StockInfo &info = it->second;
    info.UpdateCssInfo(std::move(css_info));
  } else {
    StockInfo info(code);
    info.set_css_info(std::move(css_info));
    stock_map_[code] = info; 
  }
  return true;
}

bool StockLatestInfo::UpdateCsqInfo(IndictorInfoPtr info, StockInfo *stock_info) {
  std::lock_guard<std::mutex> lock_guard(mutex_);
  std::string code = info->code();
  time_t now = time(NULL);
  auto it = stock_map_.find(code);
  if (it == stock_map_.end()) {
      StockInfo new_info(code);
      new_info.set_csq_info(std::move(info));
      new_info.resetCssTimeout(now);
      stock_map_[code] = new_info; 
  } else {
      StockInfo &stock_old_info = it->second;
      stock_old_info.UpdateCsqInfo(std::move(info));
      stock_old_info.resetCssTimeout(now);
  }
  *stock_info = stock_map_[code];
  return true;
}

} // namespace stock_info
