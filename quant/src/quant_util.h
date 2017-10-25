#ifndef QUANT_UTIL_H
#define QUANT_UTIL_H
#include <atomic>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <thread>
#include <mutex>
#include <glog/logging.h>

namespace quant_util {

class AcodesControl {
 public:
  std::list<std::string> acode() {
    std::lock_guard<std::mutex> guard(lock_);
    return acode_;
  }
  void set_acode(std::list<std::string> acode) {
    std::lock_guard<std::mutex> guard(lock_);
    acode_ = acode;
  }

  static AcodesControl* GetInstance() {
    static AcodesControl control;
    return &control;
  }

 private:
  std::list<std::string> acode_;
  std::mutex lock_;
};

class DateControl {
 public:
  DateControl(); 
  
  std::string date() {
    std::lock_guard<std::mutex> guard(lock_);
    return date_;
  }

  static DateControl* GetInstance() {
    static DateControl control;
    return &control;
  }

  bool IsTradeDay(const struct tm &current);

 private:
  std::string  GetSystemDate();

  void UpdateThd(); 
  void UpdateTradeDate();
  void UpdateDate() {
    std::lock_guard<std::mutex> guard(lock_);
    date_ = GetSystemDate();
    LOG(INFO) << date_;
  }
  void LoadFromFile();

  int day_;
  std::mutex lock_;
  std::string date_;
  std::set<std::string> close_day_;
};

class LibUtil {
 public:
  static void SplitString(const std::string& src, std::set<std::string> *dest, const std::string& flag); 
};

}
#endif
