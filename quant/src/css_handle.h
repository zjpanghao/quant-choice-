#ifndef CSS_HANDLE_H
#define CSS_HANDLE_H
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
class CssHandle : public SynHandle{
 public:
  CssHandle(const std::string &indictors) 
    : SynHandle(indictors) {
  }

  CssHandle(const std::vector<std::string> &indictors) 
    : SynHandle(indictors) {
  }
 
 virtual QuantTaskPtr fetchData(const std::string &indictors) {
   EQDATA* pCtrData = NULL;
   std::string codes;
   if (!AcodesControl::GetInstance()->GetSingleCodes(&codes))
     return NULL;
   int rc = css(codes.c_str(), indictors.c_str(), "", pCtrData);
   if (rc) {
     LOG(ERROR) << "css error" << rc;
     return NULL;
   }
   if (!pCtrData) {
     return NULL;
   }
   QuantTaskPtr task(new CssTask());
   if (GetMarketDataPacks(pCtrData, 
                          task->getPack()) == false) {
     return NULL;
   }
   releasedata(pCtrData);
   return task;
 }

};

}
#endif
