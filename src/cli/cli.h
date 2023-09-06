#ifndef SRC_CLI_CLI_H_
#define SRC_CLI_CLI_H_

#include <cmd_args/cmd_args.h>

#include <iostream>

using std::cin;
using std::cout;

namespace hhullen {

class CLI {
 public:
  CLI();
  ~CLI() {}
  void Init(int argc, const char* argv[]);
  int Exec();

 private:
  CMDArgs cmd_line;
};

}  // namespace hhullen

#endif  // SRC_CLI_CLI_H_