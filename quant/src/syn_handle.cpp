#include "syn_handle.h"
#include "quant_pack.h"
#include "glog/logging.h"

namespace quant {
bool SynHandle::Update() {
  LOG(INFO) << "prepare update";
  QuantTaskPtr task = fetchData(indictors_);
  if (!task)
    return false;
  task->setProcess(process_);
  service_->Execute(std::move(task));     
  return true;
}

} // namespace quant
