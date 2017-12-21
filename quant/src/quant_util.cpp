#include "quant_util.h"
#include <future>
#include <unistd.h>
#include <fstream>
#include <streambuf>
#define STOCK_MARKET_CLOSE_FILE  "trade_date.txt"
namespace quant_util {

int quantWriteLog(const char *buf) {
  LOG(INFO) << buf;
  return 0;
}

DateControl::DateControl() : day_(0) {
  UpdateDate();
  UpdateTradeDate();
  std::thread update_thd(&DateControl::UpdateThd, this); 
  update_thd.detach();
}

void DateControl::UpdateThd() {
  while (true) {
    LOG(INFO) << "UpdateThd";
    sleep(600);
    time_t now = time(NULL);
    struct tm current;
    localtime_r(&now, &current);
    if (current.tm_mday != day_) {
      day_ = current.tm_mday;
      UpdateDate();
      UpdateTradeDate();
    }
  }    
}

std::string  DateControl::GetSystemDate()  {
  time_t now = time(NULL);
  struct tm current;
  localtime_r(&now, &current);
  char buf[16];
  snprintf(buf, sizeof(buf), "%04d%02d%02d",
      current.tm_year + 1900, current.tm_mon + 1, current.tm_mday);
  return std::string(buf);
}

void DateControl::UpdateTradeDate() {
  LoadFromFile();
}

void DateControl::LoadFromFile() {
  std::ifstream inputFile(STOCK_MARKET_CLOSE_FILE);
  std::string file_data((std::istreambuf_iterator<char>(inputFile)),
                        std::istreambuf_iterator<char>());
  LOG(INFO) << "The file:" << file_data;
  std::lock_guard<std::mutex> guard(lock_);
  close_day_.clear();
  LibUtil::SplitString(file_data, &close_day_, "\n");
  LOG(INFO) << close_day_.size();
}

bool DateControl::IsTradeDay(const struct tm &current) {
  if (current.tm_wday <1 || current.tm_wday > 5) 
    return false;
  char buf[16];
  snprintf(buf, sizeof(buf), "%4d%02d%02d", current.tm_year + 1900, 
      current.tm_mon + 1, current.tm_mday);
  std::lock_guard<std::mutex> guard(lock_);
  return 0 == close_day_.count(std::string(buf));
}

void LibUtil::SplitString(const std::string& src, std::set<std::string> *dest, const std::string& flag) {
  std::string::size_type pos1, pos2;
  pos2 = src.find(flag);
  pos1 = 0;
  while(std::string::npos != pos2) {
    dest->insert(src.substr(pos1, pos2 - pos1));
    pos1 = pos2 + flag.size();
    pos2 = src.find(flag, pos1);
  }
  if(pos1 != src.length())
    dest->insert(src.substr(pos1));
}

}
