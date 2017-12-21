#include "cst_handle.h"
#include "quant_pack.h"
#include "glog/logging.h"

namespace quant {
bool CstHandle::UpdateCodes() {
  std::string codes;
  if (!AcodesControl::GetInstance()->GetSingleCodes(&codes)) {
    return false;
  }
  codes_ = codes;
  codes_ = "300413.SZ";
  return true;
}

int CstHandle::reg() {
  int id;
  struct tm current;
  time_t now = time(NULL) - 60*30;
  localtime_r(&now, &current);
  if (current.tm_hour >= 15) {
    current.tm_hour = 15;
    current.tm_min = 0;
  }
  char start[64];
  snprintf(start, sizeof(start), "20170101%02d%02d00", current.tm_hour, current.tm_min);
  LOG(INFO) << codes_ << start << indictors_;
  if ((id = cst(codes_.c_str(), indictors_.c_str(), 
    start, "20170101150000", NULL, CstHandle::cstCallback, this))< 0) {
    return -1;
  }
  id_ = id;
  return 0;
}

int CstHandle::cstCallback(const EQMSG* pMsg, LPVOID lpUserParam) {
  static int times = 1;
  time_t tt = time(NULL);
  struct tm current;
  localtime_r(&tt, &current);
  tm *t = &current;
  char buf[512];
  CstHandle  *handle = static_cast<CstHandle*>(lpUserParam);
  snprintf(buf, sizeof(buf), "[%02d:%02d:%02d]Received [%d] callback: msgType=%d, err=%d, requestID=%d, serialID=%d.\n", t->tm_hour, t->tm_min, t->tm_sec, times++, pMsg->msgType, pMsg->err, pMsg->requestID, pMsg->serialID);
  LOG(INFO) << buf;
  if (pMsg->err == 10002004 || pMsg->err == 10002007) {
    LOG(ERROR) << "recv error" << pMsg->err;
    handle->set_state(QuantHandleState::DOWN);
    return -1;
  }

  EQDATA *pEQData = pMsg->pEQData;
  handle->set_state(QuantHandleState::CONNECT);
  if (!pEQData) {
    LOG(INFO) << "cst no data";
    return 0;
  }
  QuantTaskPtr task(new CstTask());
  if (GetMarketDataPacks(pEQData, task->getPack()) == false) {
    return 0;
  }
  handle->send(std::move(task));
  return 0;
}

} // namespace quant
