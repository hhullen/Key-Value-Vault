#ifndef SRC_CLI_CLI_H_
#define SRC_CLI_CLI_H_

#include <include/cmd_args/cmd_args.h>
#include <lib/vault/engine/vault_engine.h>

#include <iostream>
#include <map>

using std::cin;
using std::cout;
using std::getline;
using std::invalid_argument;

namespace hhullen {

class CLI {
  using Executors = std::map<Str, void (VaultEngine::*)(VaultData::Data)>;

 public:
  CLI();
  ~CLI() {}
  void Init(int argc, const char* argv[]);
  void Exec();

 private:
  CMDArgs cmd_line_;
  Executors executors_;
  VaultEngine engine_;

  void SetupExecutors();
  void SetupEngineType();
  void ListenStdin();
  Str MakeUpper(Str str);
  void ExecuteMethod(Str method, Str arguments);
  VaultData::Data SplitArguments(Str command);
  void PrintExecutionOutput();
};

}  // namespace hhullen

#endif  // SRC_CLI_CLI_H_