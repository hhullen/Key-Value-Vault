#ifndef SRC_LIB_VAULT_DATA_VAULT_DATA_H_
#define SRC_LIB_VAULT_DATA_VAULT_DATA_H_

#include <include/StrPlus/str_plus.h>

#include <iostream>
#include <string>
#include <vector>

namespace hhullen {

using Str = std::string;
using std::vector;

class VaultData {
 public:
  static constexpr int kMaxFields = 5;
  VaultData() : time_mark_(0) {
    row_.resize(kMaxFields, "-");
    row_.shrink_to_fit();
    validators_.resize(kMaxFields);
    for (size_t i = 0; i < kMaxFields; ++i) {
      validators_[i] = nullptr;
    }
    validators_[2] = &VaultData::IsNum;
    validators_[4] = &VaultData::IsNum;
  }

  Str SetField(size_t idx, Str value) {
    if (idx >= kMaxFields) {
      return Str("invalid index.");
    }
    if (validators_[idx] && !(this->*(validators_[idx]))(value)) {
      return Str("unable to cast value \"" + value + "\" to type uint.");
    }
    row_[idx] = value;
    return Str("");
  }

  void SetDeathTimeMark(size_t seconds) { time_mark_ = seconds; }

  size_t GetDeathTimeMark() { return time_mark_; }

  Str GetField(const size_t idx) { return row_[idx]; }

  VaultData& operator=(const VaultData& src) {
    for (size_t i = 0; i < kMaxFields; ++i) {
      // if
    }
  }

 private:
  vector<Str> row_;
  size_t time_mark_;
  vector<bool (VaultData::*)(const Str&)> validators_;

  bool IsNum(const Str& str) {
    return StrPlus::IsValid(str, StrPlus::Type::UInt);
  }
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_DATA_VAULT_DATA_H_
