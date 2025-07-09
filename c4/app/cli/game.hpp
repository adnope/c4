#pragma once

#include <string>

#include "core/solver.hpp"

class Game {
public:
  explicit Game(const std::string& ob_book, const std::string& wb_book);

  void StartPlayerVsBotGame();

  void StartBotGame();

  void StartTraining();

private:
  Solver solver;

  static void printConnectFourBoard(const std::string &sequence);
};
