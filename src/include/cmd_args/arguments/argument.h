#ifndef SRC_ARGUMENT_ARGUMENT_H_
#define SRC_ARGUMENT_ARGUMENT_H_

#include <map>
#include <regex>
#include <string>

namespace hhullen {

using Str = std::string;

class Argument {
  using RegEx = std::regex;

 public:
  enum class Type { Int, UInt, Float, String, Path };

  Argument() = delete;
  Argument(const Str &name = "default-init", Argument::Type type = Type::String,
           const Str &help = "argument");
  ~Argument();

  void ReadArgument(const Str &arg);
  const Str GetValue() const;
  const Str GetName() const;

  static bool IsArgument(const Str &arg) {
    return arg.size() > 0 && arg[0] != '-';
  }

 private:
  std::map<Type, RegEx> regex_;
  const Str name_, help_;
  Argument::Type type_;
  Str value_;

  void ValidateArg(const Str &arg, Type type);
  void InitializeRegex();
};

Argument::Argument(const Str &name, Argument::Type type, const Str &help)
    : name_(name), help_(help), type_(type) {
  InitializeRegex();
}

Argument::~Argument() {}

void Argument::ReadArgument(const Str &arg) {
  ValidateArg(arg, type_);
  value_ = arg;
}

const Str Argument::GetValue() const { return value_; }

const Str Argument::GetName() const { return name_; }

void Argument::ValidateArg(const Str &arg, Type type) {
  if (!std::regex_match(arg, regex_[type])) {
    throw std::invalid_argument("Incorrect value type \"" + arg +
                                "\" of option \"" + name_ + "\"");
  }
}

void Argument::InitializeRegex() {
  regex_[Type::Int] = RegEx("^[-]{0,1}[0-9]+$");
  regex_[Type::UInt] = RegEx("^[0-9]+$");
  regex_[Type::Float] = RegEx("^[-]{0,1}[0-9]+\\.[0-9]+$");
  regex_[Type::String] = RegEx("^[a-zA-Z]+[0-9a-zA-z_-]*[a-zA-Z]*$");
  regex_[Type::Path] =
      RegEx("^[\\w\\/\\.]+[\\.\\\\/\\d\\w\\s\\+\\=\\#\\!\\@\\$\\(\\)\\:_-]*$");
}

}  // namespace hhullen

#endif  //  SRC_ARGUMENT_ARGUMENT_H_
