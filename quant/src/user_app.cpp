#include "user_app.h"
#include "kafka_client.h"
#include "glog/logging.h"
static wrapper_Info *info_g;
void set_wrapper_info(wrapper_Info *info) {
  info_g = info;
}

constexpr int KAF_PACKET_SIZE = 10 * 1024;

int user_recv(const std::list<std::string> &messages) {
  int rc = 0;
  std::string message;
  int count = 0;
  for (auto &send : messages) {
    message += send;
    message += "\n";
    count++;
    if (message.length() >= KAF_PACKET_SIZE || count == messages.size()) {
      if (PUSH_DATA_SUCCESS == (rc = producer_push_data(message.c_str(), message.length(), info_g))) {
        LOG(INFO) << "The size:" << message.size() << "Message:" << message;
      } else {
        LOG(ERROR) << "push data error rc " << rc << "  size" << message.length() << "  for " << message;
      }
      message.clear();
    }
  }
  return -1;
}
