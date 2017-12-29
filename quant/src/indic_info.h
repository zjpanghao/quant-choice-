#ifndef INCLUDE_INDIC_INFO_H
#define INCLUDE_INDIC_INFO_H
#include <string>
#include <map>
#define NULL_INDICTOR  "--"

class IndicInfo {
 public:
  IndicInfo(){}
  bool mergeData(const IndicInfo &info) {
    bool r = false;
    for (const auto &mdata : info.indic_map_) {
      const std::string &key = mdata.first;
      auto it = indic_map_.find(key);
      if (it != indic_map_.end()) {
        if (it->second == mdata.second ||
            mdata.second == NULL_INDICTOR) {
          continue;
        } else {
          it->second = mdata.second;
          r = true;
        }
      } else {
        setIndic(mdata.first, mdata.second);     
        r = true;
      }
      
    }
    return r;
  }

  void setIndic(const std::string &key, const std::string &value) {
    indic_map_[key] = value;
  }
  
  bool getIndic(const std::string &key,
                std::string *value) const {
    auto it = indic_map_.find(key);
    if (it == indic_map_.end())
      return false;
    *value = it->second;
    return true;
  }

 private:
  std::map<std::string, std::string> indic_map_;
};
#endif
