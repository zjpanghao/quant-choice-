#ifndef QUANT_LOGIN_H
#define QUANT_LOGIN_H
#include <string>
#include "EmQuantAPI.h"
#include "quant_util.h"
#include "login.h"
namespace quant {
class Login {
 public:
  bool Activate(const std::string &user_name, 
                const std::string &password) {
    return true;
  }

  Login() {
  }
 
  bool start() {
    EQErr error = EQERR_SUCCESS;
    EQLOGININFO logInfo;
    memset(&logInfo, 0, sizeof(logInfo));
    char buf[64];
    snprintf(buf, sizeof(buf), "TestLatency=%d,ForceLogin=%d", 
        (int)speed_test_, (int)force_);
    return ::start(&logInfo, buf, quant_util::quantWriteLog);
  }
 
  Login(bool force, bool speed_test) 
    : force_(force), 
      speed_test_(speed_test) {

  }

  bool LoginByFile() {
    bool r = false;
    return r;
  }

 private:
  const std::string user_name_;
  const std::string password_;
  bool force_ = true;
  bool speed_test_ = false;
};

} // namespace quant
#endif
