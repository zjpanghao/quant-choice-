#ifndef QUANT_PACK_H
#define QUANT_PACK_H
#include <stdio.h>
#include "EmQuantAPI.h"
#include <string>
#include "indictor_info.h"

namespace quant {
std::string eqvalue2string(const EQVARIENT* pEQVarient);
bool GetMarketDataPacks(EQDATA *pData, IndictorInfoPacks *market_data); 
}
#endif
