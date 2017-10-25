#ifndef INCLUDE_CSS_H
#define INCLUDE_CSS_H
#include <string>
#include <vector>
#include "indictor_info.h"

class CssInfo : public IndictorInfo{
 public:
  CssInfo() : IndictorInfo(css_nums_) {
    SetIndex(0, "待查询");
  }

 private:
  static constexpr int css_nums_ = 1;
};
#endif
