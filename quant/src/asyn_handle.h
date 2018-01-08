#ifndef ASYN_HANDLE_H
#define ASYN_HANDLE_H
#include <stdio.h>
#include <atomic>
#include "EmQuantAPI.h"
#include <string>
#include <memory>
#include "quant_task.h"
#include "glog/logging.h"
#include "quant_code.h"
#include "indictors.h"
namespace quant {
class AsynHandle {
 public:
  enum QuantHandleState {OFF, CONNECT, DOWN};
  AsynHandle(const std::string &indictors) 
    : indictors_(indictors) {
    service_ = Executors::NewFixPool(FIX_THREADS);
  }

  AsynHandle(const std::vector<std::string> &indictors_vec) {
    indictors_ = Indictors::getStrFromVec(indictors_vec);
    service_ = Executors::NewFixPool(FIX_THREADS);
  }

  virtual ~AsynHandle() {}
  
  virtual bool UpdateCodes();
 
  virtual int cancelReg();
   
  void set_state(QuantHandleState  state) {
    state_ = state;
  }

  QuantHandleState  state() {
    return state_;
  }

  virtual int reg() = 0; 
  
  // static int csqCallback(const EQMSG* pMsg, LPVOID lpUserParam); 

  void send(QuantTaskPtr task);

  bool Update(); 

  void checkState() {
    if (state_ == QuantHandleState::OFF) {
      LOG(INFO) << id_ << "state off now reg";
      if (codes_.length() == 0) {
        UpdateCodes();
      }
      cancelReg();
      reg();
    } else if (state_ == QuantHandleState::DOWN) {
      if (cancelReg() == 0) {
        state_ = QuantHandleState::OFF;
        LOG(INFO) << id_ << "state from down to off";
      }
    }
  }

 protected:

  IndictorInfoPacks packs_;
  ExecutorService  *service_;
  int push_type_;
  std::string indictors_;
  std::string codes_;
  std::atomic<QuantHandleState> state_{QuantHandleState::OFF};
  int id_{0};
  static constexpr int FIX_THREADS = 1;
};

}
#endif
