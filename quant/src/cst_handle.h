#ifndef CST_HANDLE_H
#define CST_HANDLE_H
#include <stdio.h>
#include <atomic>
#include "EmQuantAPI.h"
#include <string>
#include <memory>
#include "quant_task.h"
#include "glog/logging.h"
#include "quant_code.h"
#include "asyn_handle.h"
namespace quant {
class CstHandle : public AsynHandle {
 public:
  CstHandle(const std::string &indictors) 
    : AsynHandle(indictors) {
  }

  CstHandle(const std::vector<std::string> &indictors) 
    : AsynHandle(indictors) {
  }
  
  virtual bool UpdateCodes();

  virtual int reg(); 
  
  static int cstCallback(const EQMSG* pMsg, LPVOID lpUserParam); 

  void setTime(int startHour, int startMin, int endHour, int endMin) {
    startHour_ = startHour;
    startMin_ = startMin;
    endHour_ = endHour;
    endMin_ = endMin;
  }

 private:
  int startHour_;
  int startMin_;
  int endHour_;
  int endMin_;
  int push_type_;
};

}
#endif
