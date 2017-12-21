#include "asyn_handle.h"
#include "quant_pack.h"
#include "glog/logging.h"

namespace quant {

bool AsynHandle::UpdateCodes() {
  std::string codes;
  if (!AcodesControl::GetInstance()->GetAllCodes(&codes)) {
    return false;
  }
  codes_ = codes;
  return true;
}

bool AsynHandle::Update() {
  if (!UpdateCodes())
    return false;
  LOG(INFO) << "prepare update";
  if (id_) {
    if (-1 == cancelReg()) {
      LOG(INFO) << "cancel error";
      return false;
    }
  }
  return -1 != reg();
}

int AsynHandle::cancelReg() {
  return 0;
}

void AsynHandle::send(QuantTaskPtr task) {
  service_->Execute(std::move(task));     
}

} // namespace quant
