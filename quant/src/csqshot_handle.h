#ifndef CSQSHOT_HANDLE_H
#define CSQSHOT_HANDLE_H
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
class CsqshotHandle : public SynHandle{
 public:
  CsqshotHandle(const std::vector<std::string> &indictors) 
    : SynHandle(indictors, NULL) {
  }
 
 virtual QuantTaskPtr fetchData(const std::string &indictors) {
   EQDATA* pCtrData = NULL;
   std::string codes;
   if (!AcodesControl::GetInstance()->GetAllCodes(&codes))
     return NULL;
   int rc = csqsnapshot(codes.c_str(), indictors.c_str(), "", pCtrData);
   if (rc) {
     LOG(ERROR) << "csqshot error" << rc;
     return NULL;
   }
   if (!pCtrData) {
     return NULL;
   }
   LOG(INFO) << "csqshot code size" << pCtrData->codeArray.nSize;
   QuantTaskPtr task(new CsqTask());
   if (!GetMarketDataPacks(pCtrData, 
                          task->getPack())) {
     return NULL;
   }
   releasedata(pCtrData);
   return task;
 }

};

}
#endif
