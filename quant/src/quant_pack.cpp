#include "quant_pack.h"
#include <stdlib.h>
#include <glog/logging.h>
#include "indictor_info.h"
namespace quant {
std::string eqvalue2string(const EQVARIENT* pEQVarient) {
  if(!pEQVarient)
  {
    LOG(INFO) << "NULL";
    return "--";
  }

  char z[64] = {0};
  // memset(z, 0, 4096);
  std::string s = "--";
  switch(pEQVarient->vtype)
  {
    //以下为了输出便于观察，做了字符宽度限定处理，实际可参考需求处理
    case eVT_null:
      snprintf(z, sizeof(z), "%s", "--");
      LOG(INFO) << "TYPE NULL";
      s = z;
      break;
    case eVT_char:
      snprintf(z, sizeof(z), "%c", pEQVarient->unionValues.charValue);
      s = z;
      break; 
    case eVT_bool:
      snprintf(z, sizeof(z), "%c", pEQVarient->unionValues.boolValue ? "true" : "false");
      s = z;
      break;
    case eVT_int:
      snprintf(z, sizeof(z), "%d", pEQVarient->unionValues.intValue);
      s = z;
      break;
    case eVT_uInt:
      snprintf(z, sizeof(z), "%u", pEQVarient->unionValues.uIntValue);
      s = z;
      break;
    case eVT_int64: 
      snprintf(z, sizeof(z), "%lld", pEQVarient->unionValues.int64Value);
      s = z;
      break;
    case eVT_uInt64: 
      snprintf(z, sizeof(z), "%llu", pEQVarient->unionValues.uInt64Value);
      s = z;
      break;
    case eVT_float:  
      snprintf(z, sizeof(z), "%.6f", pEQVarient->unionValues.floatValue);
      s = z;
      break;
    case eVT_double: 
      snprintf(z, sizeof(z), "%.6lf", pEQVarient->unionValues.doubleValue);
      s = z;
      break;
    case eVT_asciiString:
      snprintf(z, sizeof(z), "%s", pEQVarient->eqchar.pChar);
      s = z;
      break;
    case eVT_unicodeString: 
      snprintf(z, sizeof(z), "%s", pEQVarient->eqchar.pChar);
      s = z;
      break;
    case eVT_short:     
      snprintf(z, sizeof(z), "%d", pEQVarient->unionValues.shortValue);
      s = z;
      break;
    case eVT_ushort:     
      snprintf(z, sizeof(z), "%u", pEQVarient->unionValues.uShortValue);
      s = z;
      break;

    default:
      break;
  }

  return s;
}

bool GetMarketDataPacks(EQDATA *pData, IndictorInfoPacks *market_data) {
  EQDATA *pEQData = pData;
  if (!pEQData)
    return false;
  int ndate = pEQData->dateArray.nSize;
  for (int k = 0; k < ndate; k++) {
    const char* date = pEQData->dateArray.pChArray[k].pChar;
    if (!date)
      continue;
    IndictorDateInfo  dateInfo(date);
    for(int i=0;i< pEQData->codeArray.nSize;i++) {
      const char *stock_code = pEQData->codeArray.pChArray[i].pChar;
      if (!stock_code) {
        continue;
      }
      IndictorInfoPtr indic_info(IndictorFactory::newInfo(stock_code));
      for(int j=0;j<pEQData->indicatorArray.nSize;j++) {
        const char* indic = 
            pEQData->indicatorArray.pChArray[j].pChar;
        if (!indic)
          continue;
        EQVARIENT* pEQVarient = (*pEQData)(i, j, k);
        const std::string value = pEQVarient ?
            eqvalue2string(pEQVarient) : NULL_INDICTOR;
        indic_info->setIndic(indic, value);
      }
      dateInfo.addInfo(indic_info);
    }
    market_data->addDateInfo(dateInfo);
  }
  LOG(INFO) << "The code size" << pEQData->codeArray.nSize;
  LOG(INFO) << "The date size" << pEQData->dateArray.nSize;
  LOG(INFO) << "The indict size" << pEQData->indicatorArray.nSize;
  return true;
}

bool GetCtrMarketDataPacks(EQCTRDATA *pData, 
                           IndictorInfoPacks *market_data) {
  EQCTRDATA *pEQData = pData;
  if (!pEQData)
    return false;
  int ndate = 1;
  std::string date = "--";
  IndictorDateInfo  dateInfo(date);
  for(int i=0; i< pEQData->row;i++) {
    IndictorInfoPtr indic_info(IndictorFactory::newInfo("--"));
    for(int j=0; j < pEQData->column; j++) {
      const char* indic = 
        pEQData->indicatorArray.pChArray[j].pChar;
      if (!indic)
        continue;
      EQVARIENT* pEQVarient = (*pEQData)(i, j);
      const std::string value = pEQVarient ?
        eqvalue2string(pEQVarient) : NULL_INDICTOR;
      indic_info->setIndic(indic, value);
    }
    dateInfo.addInfo(indic_info);
  }
  market_data->addDateInfo(dateInfo);
  return true;
}

} // namespace
