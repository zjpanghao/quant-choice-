#ifndef QUANT_LOGIN_H
#define QUANT_LOGIN_H
#include <string>
#include <atomic>
#include "EmQuantAPI.h"
#include "quant_util.h"
namespace quant {
class Login {
 public:
  static Login* getInstance() {
    static Login login;
    return &login;
  }

  bool Activate(const std::string &user_name, 
                const std::string &password) {
    return true;
  }

  Login() {
  }
 
  bool start() {
    EQErr error = EQERR_SUCCESS;
    EQLOGININFO logInfo;
    bool rc = 0;
    memset(&logInfo, 0, sizeof(logInfo));
    char buf[64];
    snprintf(buf, sizeof(buf), "TestLatency=%d,ForceLogin=%d", 
        (int)speed_test_, (int)force_);
    rc = ::start(&logInfo, buf, quant_util::quantWriteLog);
    LOG(INFO) << "user login start" << rc;
    return rc == 0;
  }
 
  Login(bool force, bool speed_test) 
    : force_(force), 
      speed_test_(speed_test) {

  }

  bool LoginByFile() {
    bool r = false;
    return r;
  }

  bool checkStart() {
    if (!login_) {
      login_ = start();
    }
    return login_;
  }

  void setLogin() {
    login_ = false;
  }

  bool login() {
    return login_;
  }

 private:
  const std::string user_name_;
  const std::string password_;
  bool force_ = true;
  bool speed_test_ = false;
  std::atomic<bool> login_{false};
};

} // namespace quant
#endif
