#ifndef INCLUDE_STOCK_INFO
#define INCLUDE_STOCK_INFO
#include <mutex>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include "glog/logging.h"
#include <set>
#include "quant_util.h"
#include "indictor_info.h"
#include "indictors.h"
namespace quant {
class QuantStore;
}
namespace stock_info {

class StockInfo {
 public:
  StockInfo() : StockInfo("null_code") {
  }

  explicit StockInfo(const std::string &code) 
      : code_(code), 
        css_info_(code),
        csq_info_(code) {
  }
  
  void set_code(const std::string &code) {
    code_ = code;
  }
  
  std::string code() const {
    return code_;
  }


  std::string produce_send_message() const {
    std::string message = code_ + " ";
    auto indic = quant::Indictors::getInstance();
    message += csq_info_.produce_send_message(indic->getCsqSend());
    message += css_info_.produce_send_message(indic->getCssState());
    std::string date = 
        quant_util::DateControl::GetInstance()->date();
    message +=  date;
    return message;
  }
  
  void set_css_info(IndictorInfoPtr info) {
    css_info_ = *info;
  }

  void set_csq_info(IndictorInfoPtr info) {
    csq_info_ = *info;
  }

  void UpdateCssInfo(IndictorInfoPtr info) {
    css_info_.MergeData(std::move(info));
  } 

  void UpdateCsqInfo(IndictorInfoPtr info) {
    csq_info_.MergeData(std::move(info));
  }

 private:

  std::string code_;
  std::vector<std::string> indictors_; 
  IndictorInfo css_info_;
  IndictorInfo csq_info_;
};

class StockLatestInfo {
 public:
  StockLatestInfo();
  static StockLatestInfo *GetInstance(); 

  bool UpdateCssInfo(IndictorInfoPtr css_info);

  bool UpdateCsqInfo(IndictorInfoPtr info, StockInfo *stock_info);
  
  bool store(const std::string &message); 

  bool store(const std::string &key, const std::string &message); 

  void set_store(quant::QuantStore *store) {
    store_ = store;
  }
  

 private:
  std::unordered_map<std::string, StockInfo > stock_map_;
  static std::mutex mutex_;
  quant::QuantStore *store_;
};

}

#endif
