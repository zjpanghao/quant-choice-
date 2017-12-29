#ifndef QUANT_TASK_H
#define QUANT_TASK_H
#include "thread_pool.h"
#include "indictor_info.h"
#include "quant_kafka.h"
#include "quant_process.h"
namespace quant {
class QuantTask;
typedef std::unique_ptr<QuantTask> QuantTaskPtr;
class QuantTask : public Runnable {
 public:
  QuantTask() : process_(NULL) {
  }

  virtual ~QuantTask() {}
  virtual void Run(); 
  IndictorInfoPacks *getPack() {
    return &pack_;
  }

  void setProcess(QuantProcess *process) {
    process_ = process;
  }  

  void Process(const std::string &date, IndictorInfoPtr  ptr);
  virtual void ProcessVirtual(const std::string &date, IndictorInfoPtr ptr);

 private:
  QuantProcess *process_;
  IndictorInfoPacks pack_;
};

class CsqTask : public QuantTask {
 public:
  CsqTask() {
  }
  virtual void ProcessVirtual(const std::string &date, IndictorInfoPtr ptr);
};

class CssTask : public QuantTask {
 public:
  virtual void ProcessVirtual(const std::string &date, IndictorInfoPtr ptr);
};

class CstTask : public QuantTask {
 public:
  virtual void ProcessVirtual(const std::string &date, IndictorInfoPtr ptr);
};

class CscTask : public QuantTask {
 public:
  virtual void ProcessVirtual(const std::string &date, IndictorInfoPtr ptr);
};

} // namespace quant
#endif
