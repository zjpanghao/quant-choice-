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
class SynHandle {
 public:
  SynHandle(const std::string &indictors) 
    : indictors_(indictors) {
    service_ = Executors::NewFixPool(FIX_THREADS);
  }

  SynHandle(const std::vector<std::string> &indictors) {
    indictors_ = Indictors::getStrFromVec(indictors);
    service_ = Executors::NewFixPool(FIX_THREADS);
  }

  virtual ~SynHandle() {}
  
  virtual QuantTaskPtr fetchData(const std::string &indictors) = 0;

  bool  Update(); 

 protected:
  std::string indictors_;
  IndictorInfoPacks packs_;
  
 private:
  static constexpr int FIX_THREADS = 1;
  ExecutorService  *service_;
};

}
#endif
