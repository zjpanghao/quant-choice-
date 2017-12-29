#ifndef SYN_HANDLE_H
#define SYN_HANDLE_H
#include <stdio.h>
#include <atomic>
#include "EmQuantAPI.h"
#include <string>
#include <memory>
#include "quant_task.h"
#include "glog/logging.h"
#include "quant_code.h"
#include "indictors.h"
namespace quant {
class QuantProcess;
class SynHandle {
 public:
  SynHandle(const std::string &indictors, QuantProcess *process) 
    : indictors_(indictors), process_(process) {
    service_ = Executors::NewFixPool(FIX_THREADS);
  }

  SynHandle(const std::vector<std::string> &indictors,
            QuantProcess *process) {
    indictors_ = Indictors::getStrFromVec(indictors);
    service_ = Executors::NewFixPool(FIX_THREADS);
    process_ = process;
  }

  virtual ~SynHandle() {}
  
  virtual QuantTaskPtr fetchData(const std::string &indictors) = 0;

  bool  Update(); 

  void setCodes(const std::string &codes) {
    codes_ = codes;
  }

 protected:
  std::string indictors_;
  IndictorInfoPacks packs_;
  std::string codes_;
  
 private:
  static constexpr int FIX_THREADS = 1;
  ExecutorService  *service_;
  QuantProcess *process_;
};

}
#endif
