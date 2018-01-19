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
class IndictorDateInfo{
 public:
  IndictorDateInfo(const std::string &date) {
    date_ = date;
  }
  void addInfo(IndictorInfoPtr ptr) {
    indictor_info_.push_back(ptr); 
  }

  const std::string & getDate() const {
    return date_;
  }

  const std::list<IndictorInfoPtr> & getInfos() const  {
    return indictor_info_;
  }

 private:
  std::string date_;
  std::list<IndictorInfoPtr> indictor_info_;
};


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
  void addDateInfo(IndictorDateInfo info) {
    date_infos_.push_back(info);
  }

  const std::list<IndictorDateInfo> &getInfoList() {
    return date_infos_;
  }

 private:
  std::list<IndictorDateInfo> date_infos_; 
};

class IndictorFactory {
 public:
  static IndictorInfo* newInfo(const std::string &code); 
};
#endif
