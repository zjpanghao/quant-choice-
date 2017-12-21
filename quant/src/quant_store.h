#ifndef INCLUDE_QUANT_STORE
#define INCLUDE_QUANT_STORE
namespace quant {
class QuantStore {
 public:
  virtual bool store(const std::string &value) = 0;
  virtual bool init() = 0;
};

}
#endif
