#include "cli.h"

namespace hhullen {

CLI::CLI() {}

void CLI::Init(int argc, const char* argv[]) {
  Argument vault_type("vault type", Argument::Type::String);
  cmd_line.AddArguments({vault_type});
  cmd_line.Read(argc, argv);
}

int CLI::Exec() {
  Str command;
  for (cin >> command; command != "EXIT"; cin >> command) {
    cout << "OUT: " << command << "\n";
  }
  return 0;
}

}  // namespace hhullen
