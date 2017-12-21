#include "user_app.h"
#include "kafka_client.h"
#include "glog/logging.h"
#include <set>
static wrapper_Info *info_g;
void set_wrapper_info(wrapper_Info *info) {
  info_g = info;
}

constexpr int KAF_PACKET_SIZE = 10 * 1024;

int user_update(const std::list<stock_info::StockInfo> &messages) {
  for (const auto &send : messages) {
  }
  return 0;
}
#if 0
int user_recv(const std::map<std::string, CsqInfo> &messages) {
  
  int rc = 0;
  std::string message;
  int count = 0;
  for (auto it = messages.begin(); it != messages.end(); it++) {
    const CsqInfo &csq = it->second;
    const std::string &code = it->first;
    std::string origin = csq.produce_send_message();
    LOG(INFO) << pthread_self() << "origin: " << code <<" "<<origin;
    continue;
    stock_info::StockInfo stock_info;
    bool r = stock_info::StockLatestInfo::GetInstance()->UpdateCsqInfo( csq, &stock_info);
    if (r) {
      message += stock_info.produce_send_message();
      message += "\n";
    }
    count++;
    if (message.length() > 0 && (message.length() >= KAF_PACKET_SIZE || count == messages.size())) {
      if (PUSH_DATA_SUCCESS == (rc = producer_push_data(message.c_str(), message.length(), info_g))) {
        LOG(INFO) << "Thread" << pthread_self() << "The size:" << message.size() << "Message:" << message;
      } else {
        LOG(ERROR) << "push data error rc " << rc << "  size" << message.length() << "  for " << message;
      }
      message.clear();
    }
  }
  return 0;
}
#endif
