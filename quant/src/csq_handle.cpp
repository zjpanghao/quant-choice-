#include "csq_handle.h"
#include "quant_pack.h"
#include "glog/logging.h"

namespace quant {

int CsqHandle::reg() {
  std::string option = "Pushtype=";
  option += push_type_;
  int id;
  if ((id = csq(codes_.c_str(), indictors_.c_str(), 
    option.c_str(), CsqHandle::csqCallback, this)) < 0) {
    return -1;
  }
  id_ = id;
  return 0;
}

int CsqHandle::cancelReg() {
  if (!id_)
    return 0;
  if (0 != csqcancel(id_)) {
    return -1;
  }
  id_ = 0;
  state_ = QuantHandleState::OFF;
  return 0;
}

int CsqHandle::csqCallback(const EQMSG* pMsg, LPVOID lpUserParam) {
  static int times = 1;
  time_t tt = time(NULL);
  struct tm current;
  localtime_r(&tt, &current);
  tm *t = &current;
  char buf[512];
  CsqHandle  *handle = static_cast<CsqHandle*>(lpUserParam);
  snprintf(buf, sizeof(buf), "[%02d:%02d:%02d]Received [%d] callback: msgType=%d, err=%d, requestID=%d, serialID=%d.\n", t->tm_hour, t->tm_min, t->tm_sec, times++, pMsg->msgType, pMsg->err, pMsg->requestID, pMsg->serialID);
  LOG(INFO) << buf;
  if (pMsg->err == 10002004 || pMsg->err == 10002007) {
    LOG(ERROR) << "recv error" << pMsg->err;
    handle->set_state(QuantHandleState::DOWN);
    return -1;
  }

  EQDATA *pEQData = pMsg->pEQData;
  if (!pEQData) {
    return 0;
  }
  if(pEQData->dateArray.nSize != 1) {
    LOG(ERROR) << "error date size";
    return 0;
  } 
  handle->set_state(QuantHandleState::CONNECT);
  QuantTaskPtr task(new CsqTask());
  if (GetMarketDataPacks(pEQData, task->getPack()) == false) {
    return 0;
  }
  handle->send(std::move(task));
  return 0;
}

} // namespace quant
