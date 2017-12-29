#ifndef INCLUDE_QUANT_PROCESS_H
#define INCLUDE_QUANT_PROCESS_H
#include <string>
#include "glog/logging.h"
#include "indictor_info.h"
class DBPool;
namespace quant {
class QuantProcess {
 public:
  virtual bool process(const std::string &date, IndictorInfo *indictorInfo) = 0;
};

class QuantDayLineProcess : public QuantProcess {
 public:
  QuantDayLineProcess(DBPool *pool) : pool_(pool) {
  }

  virtual bool process(const std::string &date, 
                       IndictorInfo *indictorInfo); 
 private:
  DBPool *pool_;
};

}
#endif
