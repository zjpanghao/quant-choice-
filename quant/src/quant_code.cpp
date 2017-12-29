#include "quant_code.h"
#include <set>
#include "EmQuantAPI.h"
#include "glog/logging.h"
#include "redis_pool.h"
namespace quant {
  
bool AcodesControl::GetSingleCodes(std::string *codes) {
  std::shared_ptr<RedisControl> cnt; 
  if (!pool_) {
    LOG(ERROR) << "must init redis pool first";
    goto REMOTE;
  }
  cnt = pool_->GetControl();
  if (!cnt) {
    LOG(INFO) << "redis connect failed";
    goto REMOTE;
  }

  if (!cnt->GetValue("east_acodes", codes)) {
    LOG(ERROR) << "fetch from redis error";
    pool_->ReturnControl(cnt);
    goto REMOTE;
  }
      
  if (valid(*codes)) {
    return true;
  } 

REMOTE:
  LOG(INFO) << "fetch from east...";
  *codes = GetAcodes();
  return valid(*codes);
}

bool AcodesControl::GetAllCodes(std::string *codes) {
  std::string single_code;
  if (!(GetSingleCodes(codes)))
    return false;
  if (!valid(*codes))
    return false;
  *codes +="," +  INDEX_CODES;
  return true;
}

bool AcodesControl::updateAcodes() {
  std::string codes = GetAcodes();
  if (valid(codes)) {
    std::shared_ptr<RedisControl> cnt 
      = pool_->GetControl();
    if (cnt == NULL) {
      LOG(ERROR) << "Get redis control NULL";
      return false;
    }
    if (cnt->SetValue("east_acodes", codes)) {
      pool_->ReturnControl(cnt);
      return true;      
    } else {
      LOG(ERROR) << "set acode failed";
    }
  }
  return false;
}

std::string AcodesControl::GetAcodes() {
  std::set<std::string> codes_set = GetAcodesRemote(); 
  std::string all;
  std::for_each(codes_set.begin(), codes_set.end(), [&all](const std::string &s)      {all += s + ",";});
  if (all.length())
    all = all.substr(0, all.length() - 1);
  return all;
}

std::set<std::string>  AcodesControl::GetAcodesRemote() {
  int max_codes = 100 - 4;
  EQDATA* Acodes = NULL;
  time_t now = time(NULL);
  struct tm current;
  localtime_r(&now, &current);
  char date[16];
  snprintf(date, sizeof(date), "%04d%02d%02d", current.tm_year + 1900, current.tm_mon + 1, current.tm_mday);
  LOG(INFO) << "date:" << date;
  std::set<std::string> stock_codes;
  if (EQERR_SUCCESS == sector("001004",date, NULL, Acodes)) {
    EQDATA *pData = Acodes;
    for(int i=0;i<pData->codeArray.nSize;i++) {
      for(int k=0;k<pData->dateArray.nSize;k++) {
        if (stock_codes.size() < max_codes) {
          stock_codes.insert(pData->codeArray.pChArray[i].pChar);
        }

      }
    }
    releasedata(Acodes);
  }
  LOG(INFO) << stock_codes.size();
  return stock_codes;
}

} // quant
