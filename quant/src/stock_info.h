#ifndef INCLUDE_STOCK_INFO
#define INCLUDE_STOCK_INFO
#include <mutex>
#include <string>
#include <unordered_map>
#include "glog/logging.h"
#include <set>
#define EAST_INDICTORS_NUM 49
#define NULL_INDICTOR  "--"


namespace stock_info {
enum StockIndictor {
  TIME,
  NOW,
  HIGH,
  LOW,
  OPEN,
  PRECLOSE,
  ROUNDLOT,
  VOLUME,
  AMOUNT
};

class StockInfo {
 public:
  StockInfo() : StockInfo("null_code") {
  }

  explicit StockInfo(const std::string &code) : code_(code), indictors_(EAST_INDICTORS_NUM, NULL_INDICTOR) {
  }
  
  bool SetIndex(int i, const std::string &value) {
    if (i >= 0 && i < indictors_.size()) {
      indictors_[i] = value;   
      return true;
    }
    return false;
  }

  bool GetIndex(int i, std::string *value) {
    if (i >= 0 && i < indictors_.size()) {
      *value = indictors_[i];
      return true;
    }
    return false;
  }
  
  void set_code(const std::string &code) {
    code_ = code;
  }
  
  std::string code() const {
    return code_;
  }


  std::string produce_send_message() const {
    std::string message;
    AddIndictor(&message, code_);
    int i = 0;
    for (const auto & indictor : indictors_) {
      AddIndictor(&message, indictor);
      switch (i) {
        case static_cast<int>(stock_info::StockIndictor::ROUNDLOT):
          AddIndictor(&message, NULL_INDICTOR);
          break;
        case static_cast<int>(stock_info::StockIndictor::AMOUNT):
          AddIndictor(&message, NULL_INDICTOR);
          AddIndictor(&message, NULL_INDICTOR);
          break;
        default:
          break;
      }
      i++;
    }
    return message;
  }

 private:
  void AddIndictor(std::string *message, const std::string &value) const {
    (*message) += value;
    (*message) += " ";
  }

  std::string code_;
  std::vector<std::string> indictors_; 
};

class StockLatestInfo {
 public:
  static StockLatestInfo *GetInstance(); 
 
  bool UpdateNonvariable(const StockInfo &info);

  bool MergeFullInfo(StockInfo *info); 

 private:
  bool MergeData(StockInfo *info,
                 StockInfo *store_info);
  static StockLatestInfo *instance_;
  std::unordered_map<std::string, StockInfo> stock_map_;
  static std::mutex mutex_;
};

}

#endif
