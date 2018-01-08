#ifndef INCLUDE_QUANT_MON_H
#define INCLUDE_QUANT_MON_h
#include <list>
#include <memory>
#include "csq_handle.h"
namespace quant {
class QuantMon {
 public:
  void start() {
    std::thread t(&QuantMon::checkThd, this);
    t.detach();
  }

  void addMon(AsynHandle*handle) {
    handles_.push_back(handle);
  }

  void checkThd() {
    while (true) {
      time_t now = time(NULL);
      struct tm current;
      localtime_r(&now, &current);
      for (auto handle : handles_) {
        handle->checkState();
        if (force_update_) {
          force_update_ = !handle->Update();
        }
      }
      sleep(30);
    }
  }

  std::list<AsynHandle*> handles_;

  std::atomic<bool> force_update_{false};

  void forceUpdate() {
    force_update_ = true;
  }

};

}
#endif
