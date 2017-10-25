#ifndef INCLUDE_STOCK_INFO
#define INCLUDE_STOCK_INFO
#include <mutex>
#include <string>
#include <unordered_map>
#include <map>
#include "glog/logging.h"
#include <set>
#include "css_info.h"
#include "csq_info.h"
#include "quant_util.h"

namespace stock_info {

class StockInfo {
 public:
  StockInfo() : StockInfo("null_code") {
  }

  explicit StockInfo(const std::string &code) : code_(code) {
  }
  
  void set_code(const std::string &code) {
    code_ = code;
  }
  
  std::string code() const {
    return code_;
  }


  std::string produce_send_message() const {
    std::string message = code_ + " ";
    message += csq_info_.produce_send_message();
    message += css_info_.produce_send_message();
    std::string date = 
        quant_util::DateControl::GetInstance()->date();
    message +=  date;
    return message;
  }
  
  void set_css_info(const CssInfo &info) {
    css_info_ = info;
  }

  void set_csq_info(const CsqInfo &info) {
    csq_info_ = info;
  }

  void UpdateCssInfo(const CssInfo &info) {
    css_info_.MergeData(info);
  } 

  void UpdateCsqInfo(const CsqInfo &info) {
    csq_info_.MergeData(info);
  }

 private:

  std::string code_;
  std::vector<std::string> indictors_; 
  CssInfo css_info_;
  CsqInfo csq_info_;
};

class StockLatestInfo {
 public:
  static StockLatestInfo *GetInstance(); 

  bool UpdateCssInfo(const std::map<std::string, CssInfo> &info_map);

  bool UpdateCsqInfo(std::string code, const CsqInfo &info, StockInfo *stock_info);

 private:
  std::unordered_map<std::string, StockInfo> stock_map_;
  static std::mutex mutex_;
};

}

#endif
