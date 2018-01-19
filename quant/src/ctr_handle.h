#ifndef CTR_HANDLE_H
#define CTR_HANDLE_H
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
class QuantProcess;
class CtrHandle : public SynHandle{
 public:
  CtrHandle(const std::vector<std::string> &indictors) 
    : SynHandle(indictors, NULL) {
  }
  CtrHandle(const std::vector<std::string> &indictors, 
            QuantProcess *process,
           const std::string &tableName) 
    : SynHandle(indictors, process), tableName_(tableName) {
  }
 
  virtual QuantTaskPtr fetchData(const std::string &indictors) {
    EQCTRDATA* pCtrData = NULL;
    int rc = ctr(tableName_.c_str(), "", 
        "secucode=002668.SZ,StartDate=20001010,EndDate=20180118",
        pCtrData);
    if (rc) {
      LOG(ERROR) << "ctr error" << rc;
      return NULL;
    }
    if (!pCtrData) {
      return NULL;
    }
    QuantTaskPtr task(new CtrTask());
    if (GetCtrMarketDataPacks(pCtrData, 
          task->getPack()) == false) {
      return NULL;
    }
    auto &info_date = task->getPack()->getInfoList();
    for (auto &indics : info_date) {
      const std::string &date = indics.getDate();
      for (auto &indic : indics.getInfos()) {
        indic->setIndic("table", tableName_);
      }
    }
    releasedata(pCtrData);
    return task;
  }
 private:
  std::string tableName_;

};

}
#endif
