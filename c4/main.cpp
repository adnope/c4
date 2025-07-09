#include "app/cli/argparser.hpp"

int main(const int argc, const char **argv) {
  ArgParser parser(argc, argv);
  parser.Run();

  return 0;
}
