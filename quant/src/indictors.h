#ifndef INCLUDE_INDICTORS_H
#define INCLUDE_INDICTORS_H
#include <vector>
#include <string>
namespace quant {
class Indictors {
 public:

  static std::string getStrFromVec
     (const std::vector<std::string> &vec) {
    std::string res;
    for (auto &str : vec) {
      res += str;
      res += ",";
    }
    if (res.length()) {
      res = res.substr(0, res.length() - 1);
    }
    return res; 
  }

  static Indictors* getInstance() {
    static Indictors indictor;
    return &indictor;
  }

  const std::vector<std::string>& getCsq() {
    return csq_;
  }

  const std::vector<std::string> &getCsqSend() {
    return csq_send_;
  }

  const std::vector<std::string> &getCssState() {
    return css_state_;
  }

  const std::vector<std::string> &getCssHis() {
    return css_his_;
  }

  const std::vector<std::string> &getCst() {
    return cst_;
  }

  const std::vector<std::string> &getCsc() {
    return csc_;
  }

 private:
  Indictors() {
  }

  Indictors(const Indictors &indictor) = delete;
  Indictors& operator = (const Indictors &indictor) = delete;
  std::vector<std::string> csq_{"TIME","NOW", "HIGH", "LOW"};
  std::vector<std::string> css_state_{"TradeStatus"};
  std::vector<std::string> css_his_{"TIME"};
  std::vector<std::string> cst_{"TIME"};
  std::vector<std::string> csc_{"TIME"};
  const std::vector<std::string> csq_send_{"TIME","NOW", "HIGH", "LOW"};

};

}
#endif
