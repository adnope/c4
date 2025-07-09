#pragma once

#include "cxxopts/cxxopts.hpp"

class ArgParser {
 public:
  explicit ArgParser(int argc, const char **argv);

  void Run();

 private:
  cxxopts::Options options;
  int argc;
  const char **argv;

  cxxopts::ParseResult Parse();

  void HandleArgs(const cxxopts::ParseResult &result);

  void Log();
};