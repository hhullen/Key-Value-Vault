#include "cli.h"

namespace hhullen {

CLI::CLI() {}

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
  executors_["UPDATE"] = &VaultEngine::ExecuteUpdate;
  executors_["KEYS"] = &VaultEngine::ExecuteKeys;
  executors_["RENAME"] = &VaultEngine::ExecuteRename;
  executors_["TTL"] = &VaultEngine::ExecuteTTL;
  executors_["FIND"] = &VaultEngine::ExecuteFind;
  executors_["SHOWALL"] = &VaultEngine::ExecuteShowAll;
  executors_["UPLOAD"] = &VaultEngine::ExecuteUpload;
  executors_["EXPORT"] = &VaultEngine::ExecuteExport;
}

void CLI::Exec() {
  SetupEngineType();
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
        Str("Available types: binary_tree/BT, hash_table/HT\n"));
  }
}

void CLI::ListenStdin() {
  Str method, arguments;
  for (cin >> method; MakeUpper(method) != "EXIT"; cin >> method) {
    if (executors_.find(MakeUpper(method)) == executors_.end()) {
      cout << "> Unknown method: \"" << method << "\"\n";
      getline(cin, method);
      continue;
    }
    getline(cin, arguments);
    ExecuteMethod(method, arguments);
    PrintExecutionOutput();
  }
}

Str CLI::MakeUpper(Str str) {
  for (size_t i = 0; i < str.size(); ++i) {
    str[i] = std::toupper(str[i]);
  }
  return str;
}

void CLI::ExecuteMethod(Str method, Str arguments) {
  VaultData::Data payload = SplitArguments(arguments);
  (engine_.*(executors_[method]))(payload);
}

VaultData::Data CLI::SplitArguments(Str args) {
  VaultData::Data arg_list;
  size_t size = args.size();
  for (size_t i = 0; i < size; ++i) {
    if (args[i] != ' ') {
      char* token_start_ptr = &(args.data())[i];
      for (; i < size && args[i] != ' '; ++i) {
      }
      if (i < size) {
        args[i] = '\0';
      }
      arg_list.emplace_back(Str(token_start_ptr));
    }
  }
  return arg_list;
}

void CLI::PrintExecutionOutput() {
  optional<Str> output = engine_.Yield();
  for (; output; output = engine_.Yield()) {
    cout << *output << "\n";
  }
}

}  // namespace hhullen
