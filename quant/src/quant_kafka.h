#ifndef INCLUDE_QUANT_KAFKA_H
#define INCLUDE_QUANT_KAFKA_H
#include <string>
#include <memory>
#include "quant_store.h"
#include "kafka_producer.h"
#include "glog/logging.h"
namespace quant {
class QuantKafka : public QuantStore{
 public:
  QuantKafka(const std::string &server,
             const std::string &topic) {
    server_ = server;
    topic_ = topic;
  }
  bool init() {
    producer_.reset(new KafkaProducer());
    
    return 0 == producer_->Init(server_.c_str(), 
                           topic_.c_str(),
                           "group");
  }

  bool store(const std::string &value) {
    LOG(INFO) << "store:" << value;
    return 0 == producer_->Send((char*)value.c_str(), value.length());
  }

 private:
  std::unique_ptr<KafkaProducer> producer_;
  std::string server_;
  std::string topic_;
};

}
#endif
