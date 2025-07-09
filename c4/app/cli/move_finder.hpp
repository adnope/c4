#pragma once

#include <cstdint>
#include <string>

#include "core/solver.hpp"

class MoveFinder {
 public:
  MoveFinder(const std::string &ob_path, const std::string &wb_path);

  void FindBestMove(const std::string &sequence);
  void Run();

 private:
  Solver solver;

  static void Log(int best_move, int score, int number_of_moves,
                  uint64_t nodes_explored, double time_taken,
                  const std::string &sequence);
};