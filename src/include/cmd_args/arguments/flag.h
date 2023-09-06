#ifndef SRC_ARGUMENT_FLAG_H_
#define SRC_ARGUMENT_FLAG_H_

#include <list>

#include "argument.h"

namespace hhullen {

class Flag {
  using RegEx = std::regex;

 public:
  Flag() = delete;
  Flag(const Str& long_name = "default-init", const char short_name = '\0',
       const Str& help = "flag",
       const std::initializer_list<Argument>& args = {});

  Str GetLongName();
  char GetShortName();
  const std::list<Argument>& GetArguments();

  static bool IsFlag(const Str& token) {
    return std::regex_match(token, RegEx("(^-\\w$)|(^--\\w+$)"));
  }

 private:
  std::list<Argument> arguments_;
  const Str long_name_;
  const char short_name_;
  const Str help_;
};

Flag::Flag(const Str& long_name, const char short_name, const Str& help,
           const std::initializer_list<Argument>& args)
    : arguments_(args),
      long_name_(long_name),
      short_name_(short_name),
      help_(help) {}

Str Flag::GetLongName() { return long_name_; }

char Flag::GetShortName() { return short_name_; }

const std::list<Argument>& Flag::GetArguments() { return arguments_; }

}  // namespace hhullen

#endif  //  SRC_ARGUMENT_FLAG_H_
