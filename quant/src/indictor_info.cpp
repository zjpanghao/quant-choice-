#include "indictor_info.h"
std::string IndictorInfo::produce_send_message() const {
  std::string message;
  // AddIndictor(&message, code_);
  int i = 0;
  for (int i = 0; i < indictors_num(); i++) {
    std::string indictor = GetIndex(i);
    AddIndictor(&message, indictor);
  }
  return message;
}

bool IndictorInfo::MergeData(const IndictorInfo &info) {
  bool r = false;
  for (int i = 0; i < indictors_num_; i++) {
    std::string store = info.GetIndex(i);
    if (store != NULL_INDICTOR) {
      SetIndex(i, store);
    } 
  }
  return true;
}
