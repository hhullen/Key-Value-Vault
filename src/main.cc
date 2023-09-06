#include <cli.cc>
#include <iostream>
#include <memory>

using hhullen::CLI;
using std::cerr;
using std::cout;
using std::exception;
using std::invalid_argument;
using std::string;

int main(int argc, const char* argv[]) {
  int exit_code = 1;
  CLI cli;
  try {
    cli.Init(argc, argv);
    exit_code = cli.Exec();
  } catch (const invalid_argument& ex) {
    cerr << "Input command error: " << ex.what() << "\n";
  } catch (const exception& ex) {
    cerr << "Exception occurred: " << ex.what() << "\n";
  }
  return exit_code;
}
