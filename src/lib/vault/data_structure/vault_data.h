#ifndef SRC_LIB_VAULT_DATA_VAULT_DATA_H_
#define SRC_LIB_VAULT_DATA_VAULT_DATA_H_

#include <include/StrPlus/str_plus.h>

#include <iostream>
#include <string>
#include <vector>

namespace hhullen {

class Err {
 public:
  Err(const Str message = "") : message_{message} {}
  Str What() { return message_; }
  bool HasError() { return message_ != ""; }

 private:
  Str message_;
};

using Str = std::string;
using std::vector;

class VaultData {
 public:
  static constexpr char kHeader[] =
      "> №  |   Фамилия   |     Имя     |     Год     |    Город    |  "
      "Количество коинов  |";
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

  VaultData(const VaultData& src) { *this = src; }

  VaultData(VaultData&& src) = default;

  void Clear() {
    row_.clear();
    row_.resize(kMaxFields, "-");
    row_.shrink_to_fit();
  }

  Err SetField(size_t idx, Str value) {
    if (idx >= kMaxFields) {
      return Err("invalid index.");
    }
    if (validators_[idx] && !(this->*(validators_[idx]))(value)) {
      return Err("unable to cast value \"" + value + "\" to type uint.");
    }
    row_[idx] = value;
    return Err();
  }

  void SetDeathTimeMark(size_t seconds) { time_mark_ = seconds; }

  size_t GetDeathTimeMark() { return time_mark_; }

  Str GetField(const size_t idx) { return row_[idx]; }

  Err ReadPayload(const vector<Str>& arguments, size_t shift) {
    Clear();
    for (size_t i = shift;
         i < arguments.size() && i - shift < VaultData::kMaxFields; ++i) {
      Err err = SetField(i - shift, arguments[i]);
      if (err.HasError()) {
        return err.What();
      }
    }
    return Err();
  }

  VaultData& operator=(const VaultData& src) {
    time_mark_ = src.time_mark_;
    if (row_.size() != src.row_.size()) {
      row_.resize(src.row_.size(), "-");
      row_.shrink_to_fit();
    }
    for (size_t i = 0; i < kMaxFields; ++i) {
      if (src.row_[i] != "-") {
        row_[i] = src.row_[i];
      }
    }
    return *this;
  }

  bool operator==(const VaultData& src) {
    for (size_t i = 0; i < kMaxFields; ++i) {
      if (src.row_[i] != "-" && row_[i] != "-" && src.row_[i] != row_[i]) {
        return false;
      }
    }
    return true;
  }

  Str GetRowAsString(char sep = '\t') {
    Str data_str;
    for (size_t i = 0; i < VaultData::kMaxFields; ++i) {
      data_str += row_[i] + sep;
    }
    return data_str;
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
