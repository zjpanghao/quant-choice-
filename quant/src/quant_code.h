#ifndef INCLUDE_QUANT_CODE_H
#define INCLUDE_QUANT_CODE_H
#include "predis/redis_pool.h"
#include <string>
#include <set>
#include <algorithm>

namespace quant {
class AcodesControl {
 public:
  AcodesControl() {
  }

  void set_pool(RedisPool *pool) {
    pool_ = pool;
  }

  void setMaxStockNumber(int maxStockNumber) {
    this->maxStockNumber = maxStockNumber;
  }
  
  bool GetAllCodes(std::string *codes); 

  bool GetSingleCodes(std::string *codes); 
  
  bool valid(const std::string &acodes) {
    return acodes.length() > 300;
  }

  bool updateAcodes(); 

  static AcodesControl* GetInstance() {
    static AcodesControl control;
    return &control;
  }

  std::set<std::string> GetAcodesRemote();
 
 private:
  std::string  GetAcodes(); 
  int maxStockNumber{4000};

  std::list<std::string> acode_;
  std::mutex lock_;
  RedisPool *pool_;
  const std::string INDEX_CODES {"399001.SZ, 399006.SZ, 000300.SH, 000001.SH"};
};

}
#endif
