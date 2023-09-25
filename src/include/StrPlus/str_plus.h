#ifndef SRC_STR_PLUS_H_
#define SRC_STR_PLUS_H_

#include <map>
#include <regex>
#include <string>
#include <vector>

namespace hhullen {

using Str = std::string;
using StrList = std::vector<Str>;

namespace StrPlus {

using RegEx = std::regex;

enum class Type { Int, UInt, Float };
const std::map<Type, RegEx> regex_{
    {Type::Int, RegEx("^[-]{0,1}[0-9]+(e[+-][0-9]+){0,1}$")},
    {Type::UInt, RegEx("^[0-9]+(e[+-][0-9]+){0,1}$")},
    {Type::Float, RegEx("^[-]{0,1}[0-9]+\\.[0-9]+(e[+-][0-9]+){0,1}$")},
};

StrList Split(const Str &src, char delimiter = ' ');
void MakeUpper(Str &src);
void MakeLower(Str &src);
Str GetUpper(const Str &src);
Str GetLower(const Str &src);
bool IsUpper(const Str &src);
bool IsLower(const Str &src);
bool IsValid(const Str &src, Type type);

}  // namespace StrPlus

}  // namespace hhullen

#endif  // SRC_STR_PLUS_H_
