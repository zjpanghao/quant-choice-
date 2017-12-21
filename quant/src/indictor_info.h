#ifndef INCLUDE_INDICTOR_INFO_H
#define INCLUDE_INDICTOR_INFO_H
#include <string>
#include <vector>
#include <list>
#include <memory>
#include "indic_info.h"
#define DEFAULT_INDICTOR_VALUE "--"
#define TRADESTATE_DEFAULT_INDICTOR_VALUE "待查询"
#define EAST_SPLIT_SYMBOL " "
class IndictorInfo;
class IndictorInfos;
typedef std::shared_ptr<IndictorInfo> IndictorInfoPtr;
typedef IndictorInfos IndictorInfoPacks; 


class IndictorInfo {
 public:
  IndictorInfo() :code_("null_code") {
  }
  IndictorInfo(const std::string &code) :code_(code) {
  }

  virtual ~IndictorInfo() {
  }

  void setIndic(const std::string &key, 
              const std::string &value) {
    info_.setIndic(key, value);
  }

  bool getIndic(const std::string &key, 
              std::string *value) const {
    return info_.getIndic(key, value);
  }

  static void AddIndictor(std::string *message, 
                          const std::string &value) {
    *message += value;
    *message += EAST_SPLIT_SYMBOL;
  }

  std::string produce_send_message(const std::vector<std::string> &indictors) const ;

  bool MergeData(IndictorInfoPtr store_info);

  void set_code(const std::string &code) {
    code_ = code;
  }

  std::string code() {
    return code_;
  }

 private:
  IndicInfo  info_;
  std::string code_;
};

enum IndictorType {
  CSQ,
  CSS,
  CST,
  CSC
};

class IndictorInfos {
 public:
  void updateInfo(const std::string &date,
                  IndictorInfoPtr  ptr) {
    date_infos_[date] = ptr;
  }

  IndictorInfoPtr getInfo(const std::string &date) {
    auto it = date_infos_.find(date);
    if (it == date_infos_.end()) {
      return NULL;
    }
    return it->second;
  } 

  const std::map<std::string, IndictorInfoPtr> &getInfoMap() {
    return date_infos_;
  }
 
 private:
  std::map<std::string, IndictorInfoPtr> date_infos_; 
 
};

class IndictorFactory {
 public:
  static IndictorInfo* newInfo(const std::string &code); 
};
#endif
