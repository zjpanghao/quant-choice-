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
             const std::string &topic,
             int total_partitions) {
    server_ = server;
    topic_ = topic;
    total_partitions_ = total_partitions;
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

  bool store(const std::string &key, const std::string &value) {
    int partition = getPartition(key);
    return 0 == producer_->Send((char*)value.c_str(), value.length(), partition);
  }

 private:

  int getPartition(const std::string &key) {
    int sum = 0;
    if (key.length() >= 6) {
      for (int i = 0; i < 6; i++) {
        sum += key[i];
      }
    }
    return sum % total_partitions_;
  }

  std::unique_ptr<KafkaProducer> producer_;
  std::string server_;
  std::string topic_;
  std::map<std::string, int>  partition_map_;
  int total_partitions_;
};

}
#endif
