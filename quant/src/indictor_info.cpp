#include "indictor_info.h"

std::string IndictorInfo::produce_send_message(
    const std::vector<std::string> &indictors) const {
  std::string message;
  for (const auto &indic : indictors) {
    std::string indictor;
    bool r = getIndic(indic, &indictor);
    if (!r) 
      indictor = indic == "TRADESTATUS" ? TRADESTATE_DEFAULT_INDICTOR_VALUE 
          : DEFAULT_INDICTOR_VALUE;
    AddIndictor(&message, indictor);
  }
  return message;
}

bool IndictorInfo::MergeData(IndictorInfoPtr info) {
  bool r = false;
  r = info_.mergeData(info->info_);
  return r;
}

IndictorInfo* IndictorFactory::newInfo(const std::string &code) {
  return new IndictorInfo(code);
}
