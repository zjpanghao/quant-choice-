#ifndef CSQ_HANDLE_H
#define CSQ_HANDLE_H
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
class CsqHandle : public AsynHandle {
 public:
  CsqHandle(const std::vector<std::string> &indictors, 
            int push_type) 
    : AsynHandle(indictors), 
      push_type_(push_type) {
  }
  
  virtual int cancelReg();
   

  virtual int reg(); 
  
  static int csqCallback(const EQMSG* pMsg, LPVOID lpUserParam); 

 private:
  int push_type_;
};

}
#endif
