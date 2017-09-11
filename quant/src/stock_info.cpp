#include "stock_info.h"

namespace stock_info {
StockLatestInfo *StockLatestInfo::instance_ = NULL;
std::mutex StockLatestInfo::mutex_;
}
