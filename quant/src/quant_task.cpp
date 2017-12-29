#include "quant_task.h"
#include "stock_info.h"
namespace quant {
void QuantTask::Process(const std::string &date, IndictorInfoPtr  indic) {
  ProcessVirtual(date, indic);
}

void QuantTask::ProcessVirtual(const std::string &date, IndictorInfoPtr  indic) {
}

void QuantTask::Run() {
  auto &info_date = pack_.getInfoList();
  for (auto &indics : info_date) {
    //std::string origin = indic->produce_send_message();
    //LOG(INFO) << pthread_self() << "origin: " << code <<" "<<origin;
    const std::string &date = indics.getDate();
    for (auto &indic : indics.getInfos()) {
      Process(date, indic);
    }
  }
}


void CssTask::ProcessVirtual(const std::string &date, IndictorInfoPtr indic) {
  stock_info::StockLatestInfo::GetInstance()->
      UpdateCssInfo(indic);
  LOG(INFO) << "css info";
}

void CsqTask::ProcessVirtual(const std::string &date, IndictorInfoPtr indic) {
  stock_info::StockInfo stock_info;
  
  auto latest = stock_info::StockLatestInfo::GetInstance();
  bool r = latest->
      UpdateCsqInfo(indic, &stock_info);
  std::string message;
  auto indictors = Indictors::getInstance()->getCsqSend();
  if (r) {
    message = stock_info.produce_send_message();
    message += "\n";
    latest->store(message, stock_info.code());
  }
  // LOG(INFO) << message;
}

void CstTask::ProcessVirtual(const std::string &date, IndictorInfoPtr indic) {
  auto indictors = Indictors::getInstance()->getCst();
  LOG(INFO) << "cst task" << indic->produce_send_message(indictors);
}

void CscTask::ProcessVirtual(const std::string &date, IndictorInfoPtr indic) {
  auto indictors = Indictors::getInstance()->getCsc();
  LOG(INFO) << "csc task" << indic->produce_send_message(indictors);
}
  
}
