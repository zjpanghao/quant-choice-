#include "quant_task.h"
#include "stock_info.h"
namespace quant {
void QuantTask::Process(IndictorInfoPtr  indic) {
  ProcessVirtual(indic);
}

void QuantTask::ProcessVirtual(IndictorInfoPtr  indic) {
}

void QuantTask::Run() {
  auto &info_map = pack_.getInfoMap();
  for (auto &indic : info_map) {
    //std::string origin = indic->produce_send_message();
    //LOG(INFO) << pthread_self() << "origin: " << code <<" "<<origin;
    Process(indic.second);
  }
}


void CssTask::ProcessVirtual(IndictorInfoPtr indic) {
  stock_info::StockLatestInfo::GetInstance()->
      UpdateCssInfo(indic);
}

void CsqTask::ProcessVirtual(IndictorInfoPtr indic) {
  stock_info::StockInfo stock_info;
  
  auto latest = stock_info::StockLatestInfo::GetInstance();
  bool r = latest->
      UpdateCsqInfo(indic, &stock_info);
  std::string message;
  auto indictors = Indictors::getInstance()->getCsq();
  if (r) {
    message = stock_info.produce_send_message();
    message += "\n";
    latest->store(message);
  }
  // LOG(INFO) << message;
}

void CstTask::ProcessVirtual(IndictorInfoPtr indic) {
  auto indictors = Indictors::getInstance()->getCst();
  LOG(INFO) << "cst task" << indic->produce_send_message(indictors);
}

void CscTask::ProcessVirtual(IndictorInfoPtr indic) {
  auto indictors = Indictors::getInstance()->getCsc();
  LOG(INFO) << "csc task" << indic->produce_send_message(indictors);
}
  
}
