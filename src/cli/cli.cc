#include "cli.h"

namespace hhullen {

CLI::CLI() {}

CLI::~CLI() {
  engine_.StopStreaming();
  if (stream_thread_.joinable()) {
    stream_thread_.join();
  }
}

void CLI::Init(int argc, const char* argv[]) {
  Argument vault_type("vault type", Argument::Type::String);
  cmd_line_.AddArguments({vault_type});
  cmd_line_.Read(argc, argv);
  SetupExecutors();
}

void CLI::SetupExecutors() {
  executors_["SET"] = &VaultEngine::ExecuteSet;
  executors_["GET"] = &VaultEngine::ExecuteGet;
  executors_["EXISTS"] = &VaultEngine::ExecuteExists;
  executors_["DEL"] = &VaultEngine::ExecuteDelete;
  executors_["DELETE"] = &VaultEngine::ExecuteDelete;
  executors_["UPDATE"] = &VaultEngine::ExecuteUpdate;
  executors_["KEYS"] = &VaultEngine::ExecuteKeys;
  executors_["RENAME"] = &VaultEngine::ExecuteRename;
  executors_["TTL"] = &VaultEngine::ExecuteTTL;
  executors_["FIND"] = &VaultEngine::ExecuteFind;
  executors_["SHOWALL"] = &VaultEngine::ExecuteShowAll;
  executors_["UPLOAD"] = &VaultEngine::ExecuteUpload;
  executors_["IMPORT"] = &VaultEngine::ExecuteUpload;
  executors_["EXPORT"] = &VaultEngine::ExecuteExport;
}

void CLI::Exec() {
  SetupEngineType();
  stream_thread_ = Thread(&CLI::RunStdoutStreaming, this);
  ListenStdin();
}

void CLI::SetupEngineType() {
  Str vault_type = cmd_line_.GetArgument("vault type");
  if (vault_type == "binary_tree" || vault_type == "BT") {
    engine_.Init(VaultEngine::VaultType::BIN_TREE);
  } else if (vault_type == "hash_table" || vault_type == "HT") {
    engine_.Init(VaultEngine::VaultType::HASH_TABLE);
  } else {
    throw invalid_argument(
        Str("Incorrent vault type have been specified.\n") +
        Str("Available types: binary_tree or BT, hash_table or HT\n"));
  }
}

void CLI::ListenStdin() {
  Str method, arguments;
  cin >> method;
  StrPlus::MakeUpper(method);
  for (; method != "EXIT"; cin >> method, StrPlus::MakeUpper(method)) {
    if (executors_.find(method) == executors_.end()) {
      cout << "> Unknown method: \"" << method << "\"\n";
      getline(cin, arguments);
      continue;
    }
    getline(cin, arguments);
    ExecuteMethod(method, arguments);
  }
  std::cout << "END LISTENING";
}

void CLI::ExecuteMethod(Str method, Str arguments) {
  vector<Str> payload = StrPlus::Split(arguments, ' ');
  (engine_.*(executors_[method]))(payload);
}

void CLI::RunStdoutStreaming() {
  optional<Str> output = engine_.Yield();
  for (; output; output = engine_.Yield()) {
    cout << *output << "\n";
  }
  std::cout << "END STREAMING\n";
}

}  // namespace hhullen
