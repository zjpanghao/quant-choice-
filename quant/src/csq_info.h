#ifndef INCLUDE_CSQ_H
#define INCLUDE_CSQ_H
#include <string>
#include <vector>
#include "indictor_info.h"
#define EAST_INDICTORS_NUM 49

enum CsqIndictor {
  TIME,
  NOW,
  HIGH,
  LOW,
  OPEN,
  PRECLOSE,
  ROUNDLOT,
  VOLUME,
  AMOUNT
};

class CsqInfo : public IndictorInfo{
 public:
  CsqInfo() : IndictorInfo(csq_nums_) {
  }

  std::string produce_send_message() const {
    std::string message;
    for (int i = 0; i < indictors_num(); i++) {
      std::string indictor = GetIndex(i);
      AddIndictor(&message, indictor);
      switch (i) {
        case static_cast<int>(CsqIndictor::ROUNDLOT):
          AddIndictor(&message, NULL_INDICTOR);
          break;
        case static_cast<int>(CsqIndictor::AMOUNT):
          AddIndictor(&message, NULL_INDICTOR);
          AddIndictor(&message, NULL_INDICTOR);
          break;
        default:
          break;
      }
   }
   return message;
 }

 private:
  static constexpr int csq_nums_ = 49;
};
#endif
