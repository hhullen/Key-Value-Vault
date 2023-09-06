#ifndef SRC_CMD_ARGS_H_
#define SRC_CMD_ARGS_H_

#include <list>

#include "arguments/argument.h"
#include "arguments/flag.h"

namespace hhullen {

class CMDArgs {
  using StrStrPair = std::pair<Str, Str>;
  using Tokens = std::list<Str>;

  using ArgumentsStruct = std::list<Argument>;
  using ArgumentsStructIterator = ArgumentsStruct::iterator;
  using ParsedArguments = std::map<Str, Str>;
  using ArgumentsListIterator = std::list<Argument>::const_iterator;

  using FlagsStruct = std::map<StrStrPair, Flag>;
  using FlagsStructElement = std::pair<StrStrPair, Flag>;
  using ParsedFlags = std::map<StrStrPair, std::list<Str>>;
  using ParsedFlagsElement = std::pair<StrStrPair, std::list<Str>>;
  using FlagsStructIterator = FlagsStruct::iterator;
  using ParsedFlagsIterator = ParsedFlags::iterator;

 public:
  CMDArgs();
  ~CMDArgs();

  void AddArguments(const std::initializer_list<Argument>& args);
  void AddFlags(const std::initializer_list<Flag>& flags);
  Str GetArgument(const Str& name);
  std::list<Str> GetFlagValues(const Str& name);
  void Read(int argc, const char* argv[]);

 private:
  ParsedArguments positional_;
  ParsedFlags optional_;

  ArgumentsStruct arguments_;
  ArgumentsStructIterator arg_iter_;
  FlagsStruct flags_;

  Tokens tokens_;

  void CopyToThis(int argc, const char* argv[]);
  void CheckIfInCMDStruct(const Str& name);
  void CheckIsArgExists(const Str& name);
  Flag GetFlagFromToken(const Str& token);
  void ReadFlag(Flag& flag);
  void CheckFlagValuesAbsence(Flag& flag);
  void SetParsedValueForFlag(const Str& value, Flag& flag);
  void CheckFlagUniqueness(const Str& name_long, const Str& name_short);
  void ReadArgumentFromToken(Argument& argument, const Str& token);
  void CheckRemainsArguments();
  void ThrowNoSpecidiedName(const Str& name);
};

CMDArgs::CMDArgs() { arg_iter_ = arguments_.end(); }
CMDArgs::~CMDArgs() {}

void CMDArgs::AddArguments(const std::initializer_list<Argument>& args) {
  for (Argument arg : args) {
    arguments_.push_back(arg);
  }
}

void CMDArgs::AddFlags(const std::initializer_list<Flag>& flags) {
  for (Flag flag : flags) {
    Str name_long = Str("--") + flag.GetLongName();
    Str name_short = Str("-") + flag.GetShortName();
    flags_.insert({{name_long, name_short}, flag});
  }
}

Str CMDArgs::GetArgument(const Str& name) {
  CheckIfInCMDStruct(name);
  CheckIsArgExists(name);
  return positional_[name];
}

std::list<Str> CMDArgs::GetFlagValues(const Str& name) {
  FlagsStructIterator struct_iter = std::find_if(
      flags_.begin(), flags_.end(), [name](const FlagsStructElement& element) {
        return element.first.first == name || element.first.second == name;
      });

  if (struct_iter == flags_.end()) {
    throw std::invalid_argument("Struct error: Attempt get flag \'" + name +
                                "\' did not initialized.");
  }

  ParsedFlagsIterator parsed_iter = std::find_if(
      optional_.begin(), optional_.end(),
      [name](const ParsedFlagsElement& element) {
        return element.first.first == name || element.first.second == name;
      });

  if (parsed_iter == optional_.end()) {
    ThrowNoSpecidiedName(name);
  }
  return (*parsed_iter).second;
}

void CMDArgs::Read(int argc, const char* argv[]) {
  positional_.clear();
  optional_.clear();
  arg_iter_ = arguments_.begin();
  CopyToThis(argc, argv);
  for (; !tokens_.empty();) {
    Str token = tokens_.front();
    if (Argument::IsArgument(token) && arg_iter_ != arguments_.end()) {
      Argument argument = *arg_iter_;
      ReadArgumentFromToken(argument, token);
      ++arg_iter_;
      tokens_.pop_front();
    } else if (Flag::IsFlag(token)) {
      Flag flag = GetFlagFromToken(token);
      ReadFlag(flag);
    } else {
      throw std::invalid_argument("Unknown argument \'" + token +
                                  "\' specified.");
    }
  }
  CheckRemainsArguments();
}

void CMDArgs::CopyToThis(int argc, const char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    tokens_.push_back(argv[i]);
  }
}

void CMDArgs::CheckIfInCMDStruct(const Str& name) {
  ArgumentsStructIterator iter = std::find_if(
      arguments_.begin(), arguments_.end(),
      [name](const Argument& arg) { return arg.GetName() == name; });
  if (iter == arguments_.end()) {
    throw std::invalid_argument("Struct error: Attempt get argument \'" + name +
                                "\' did not initialized.");
  }
}

void CMDArgs::CheckIsArgExists(const Str& name) {
  if (positional_.find(name) == positional_.end()) {
    ThrowNoSpecidiedName(name);
  }
}

Flag CMDArgs::GetFlagFromToken(const Str& token) {
  FlagsStructIterator iter = std::find_if(
      flags_.begin(), flags_.end(), [token](const FlagsStructElement& element) {
        return element.first.first == token || element.first.second == token;
      });

  if (iter == flags_.end()) {
    throw std::invalid_argument("Unknown flag \'" + token + "\'specified.");
  }
  return (*iter).second;
}

void CMDArgs::ReadFlag(Flag& flag) {
  const std::list<Argument>& arguments = flag.GetArguments();
  ArgumentsListIterator iter = arguments.begin();
  tokens_.pop_front();

  CheckFlagValuesAbsence(flag);
  for (; !tokens_.empty() && iter != arguments.end(); tokens_.pop_front()) {
    Argument argument = *iter;
    Str token = tokens_.front();
    argument.ReadArgument(token);

    Str value = argument.GetValue();
    SetParsedValueForFlag(value, flag);
    ++iter;
  }

  if (iter != arguments.end()) {
    Argument argument = *iter;
    throw std::invalid_argument("Value \'" + argument.GetName() +
                                "\' of flag [--" + flag.GetLongName() + " -" +
                                flag.GetShortName() + "] was not specified.");
  }
}

void CMDArgs::CheckFlagValuesAbsence(Flag& flag) {
  const std::list<Argument>& arguments = flag.GetArguments();
  if (arguments.empty()) {
    SetParsedValueForFlag("true", flag);
  }
}

void CMDArgs::SetParsedValueForFlag(const Str& value, Flag& flag) {
  Str name_long = Str("--") + flag.GetLongName();
  Str name_short = Str("-") + flag.GetShortName();
  CheckFlagUniqueness(name_long, name_short);
  optional_[{name_long, name_short}].push_back(value);
}

void CMDArgs::CheckFlagUniqueness(const Str& name_long, const Str& name_short) {
  ParsedFlagsIterator iter_name_long =
      std::find_if(optional_.begin(), optional_.end(),
                   [name_long](const ParsedFlagsElement& element) {
                     return element.first.first == name_long ||
                            element.first.second == name_long;
                   });

  ParsedFlagsIterator iter_name_short =
      std::find_if(optional_.begin(), optional_.end(),
                   [name_short](const ParsedFlagsElement& element) {
                     return element.first.first == name_short ||
                            element.first.second == name_short;
                   });

  if (iter_name_long != optional_.end() || iter_name_short != optional_.end()) {
    throw std::invalid_argument(
        "CMD structure error: Double flag definition [" + name_long + " " +
        name_short + "].");
  }
}

void CMDArgs::ReadArgumentFromToken(Argument& argument, const Str& token) {
  argument.ReadArgument(token);
  Str value = argument.GetValue();
  Str name = argument.GetName();
  if (positional_.find(name) != positional_.end()) {
    throw std::invalid_argument(
        "CMD structure error: Double argument definition \'" + name + "\'.");
  }
  positional_[name] = value;
}

void CMDArgs::CheckRemainsArguments() {
  if (arg_iter_ != arguments_.end()) {
    Str name = (*arg_iter_).GetName();
    ThrowNoSpecidiedName(name);
  }
}

void CMDArgs::ThrowNoSpecidiedName(const Str& name) {
  throw std::invalid_argument("\'" + name + "\' was not specified.");
}

}  // namespace hhullen

#endif  //  SRC_CMD_ARGS_H_
