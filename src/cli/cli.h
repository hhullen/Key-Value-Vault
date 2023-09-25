#ifndef SRC_CLI_CLI_H_
#define SRC_CLI_CLI_H_

#include <include/StrPlus/str_plus.h>
#include <include/cmd_args/cmd_args.h>
#include <lib/vault/engine/vault_engine.h>

#include <iostream>
#include <map>
#include <thread>

using std::cin;
using std::cout;
using std::getline;
using std::invalid_argument;

namespace hhullen {

class CLI {
  using Executors = std::map<Str, void (VaultEngine::*)(vector<Str>&)>;
  using Thread = std::thread;

 public:
  CLI();
  ~CLI();
  void Init(int argc, const char* argv[]);
  void Exec();

 private:
  Thread stream_thread_;
  Executors executors_;
  VaultEngine engine_;
  CMDArgs cmd_line_;

  void SetupExecutors();
  void SetupEngineType();
  void ListenStdin();
  void ExecuteMethod(Str method, Str arguments);
  void RunStdoutStreaming();
};

}  // namespace hhullen

#endif  // SRC_CLI_CLI_H_
