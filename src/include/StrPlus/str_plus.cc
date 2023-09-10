#include "str_plus.h"

namespace hhullen {

StrList StrPlus::Split(const Str& src, char delimiter) {
  StrList str_list;
  size_t size = src.size();
  for (size_t i = 0; i < size; ++i) {
    if (src[i] != ' ') {
      const char* token_start_ptr = &(src.data())[i];
      size_t token_len = 0;
      for (; i < size && src[i] != ' '; ++i, ++token_len) {
      }
      str_list.emplace_back(Str(token_start_ptr, token_len));
    }
  }
  return str_list;
}

void StrPlus::MakeUpper(Str& src) {
  for (size_t i = 0; i < src.size(); ++i) {
    src[i] = static_cast<char>(std::toupper(src[i]));
  }
}

void StrPlus::MakeLower(Str& src) {
  for (size_t i = 0; i < src.size(); ++i) {
    src[i] = static_cast<char>(std::tolower(src[i]));
  }
}

Str StrPlus::GetUpper(const Str& src) {
  Str returnalbe(src);
  MakeUpper(returnalbe);
  return returnalbe;
}

Str StrPlus::GetLower(const Str& src) {
  Str returnalbe(src);
  MakeLower(returnalbe);
  return returnalbe;
}

bool StrPlus::IsUpper(const Str& src) {
  for (size_t i = 0; i < src.size(); ++i) {
    if (src[i] >= 'a' && src[i] <= 'z') {
      return false;
    }
  }
  return true;
}

bool StrPlus::IsLower(const Str& src) {
  for (size_t i = 0; i < src.size(); ++i) {
    if (src[i] >= 'A' && src[i] <= 'Z') {
      return false;
    }
  }
  return true;
}

bool StrPlus::IsValid(const Str& src, Type type) {
  if (!std::regex_match(src, regex_.find(type)->second)) {
    return false;
  }
  return true;
}

}  // namespace hhullen
