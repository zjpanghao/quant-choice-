#include "quant_process.h"
#include "hold.h"
#include "day_line.h"
namespace quant {
bool QuantDayLineProcess::process(const std::string &date, 
                                  IndictorInfo *indictorInfo) {
  int r = 0;
  DayLineDao dao(pool_);
  std::string origin_code = indictorInfo->code();
  std::string code = origin_code == "000001.SH" ? "sh000001" : origin_code.substr(0, 6);
  r = dao.InsertOrUpdate(code, date, *indictorInfo);
  LOG(INFO) <<"insert " << code << " :" << r;
  return r;
}

bool QuantCtrProcess::process(const std::string &date, 
                              IndictorInfo *indictorInfo) {
  int r = 0;
  HoldDao dao(pool_);
  r = dao.InsertOrUpdate(*indictorInfo);
  return r;
}

}
