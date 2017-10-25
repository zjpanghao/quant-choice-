#ifndef INCLUDE_INDICTOR_INFO_H
#define INCLUDE_INDICTOR_INFO_H
#include <string>
#include <vector>
#define DEFAULT_INDICTOR_VALUE "0"
#define EAST_SPLIT_SYMBOL " "
#define NULL_INDICTOR  "--"
class IndictorInfo {
 public:
  explicit IndictorInfo(int nums) : indictors_num_(nums), indictors_(indictors_num_, DEFAULT_INDICTOR_VALUE) {
  }
  virtual ~IndictorInfo() {
  }

  void SetIndex(int i, std::string value) {
    if (i >= 0 && i < indictors_num_)
      indictors_[i] = value;
  }

  std::string GetIndex(int i) const {
    if (i >=0 && i < indictors_num_)
      return indictors_[i];
    return DEFAULT_INDICTOR_VALUE;
  }
  
  int indictors_num() const {
    return indictors_num_;
  }

  void AddIndictor(std::string *message, const std::string &value) const {
    (*message) += value;
    (*message) += EAST_SPLIT_SYMBOL;
  }

  virtual std::string produce_send_message() const;

  bool MergeData(const IndictorInfo &store_info);

 private:
  int indictors_num_; 
  std::vector<std::string> indictors_;
};
#endif
