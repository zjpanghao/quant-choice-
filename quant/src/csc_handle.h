#ifndef CSC_HANDLE_H
#define CSC_HANDLE_H
#include <stdio.h>
#include <atomic>
#include "EmQuantAPI.h"
#include <string>
#include <memory>
#include "quant_task.h"
#include "glog/logging.h"
#include "syn_handle.h"
#include "quant_code.h"
#include "quant_pack.h"
namespace quant {
class CscHandle : public SynHandle{
 public:
  CscHandle(const std::string &indictors, 
            const std::string &start,
            const std::string &end) 
    : SynHandle(indictors, NULL) {
  }
 
 virtual QuantTaskPtr fetchData(const std::string &indictors) {
   EQDATA* pCtrData = NULL;
   std::string codes;
   if (!AcodesControl::GetInstance()->GetSingleCodes(&codes))
     return NULL;
   int rc = csc(codes.c_str(), indictors.c_str(),start_.c_str(), end_.c_str(), "", pCtrData);
   if (rc) {
     LOG(ERROR) << "csc error" << rc;
     return NULL;
   }
   if (!pCtrData) {
     return NULL;
   }
   QuantTaskPtr task(new CscTask());
   if (GetMarketDataPacks(pCtrData, 
                          task->getPack()) == false) {
     return NULL;
   }
   releasedata(pCtrData);
   return task;
 }
 private:
  std::string start_;
  std::string end_;

};

}
#endif
