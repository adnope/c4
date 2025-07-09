#include "move_finder.hpp"

#include <chrono>
#include <iostream>
#include <ratio>
#include <string>

#include "core/solver.hpp"

MoveFinder::MoveFinder(const std::string &ob_path, const std::string &wb_path) {
  solver.GetReady(ob_path, wb_path);
}

void MoveFinder::Run() {
  std::string line;
  while (std::getline(std::cin, line)) {
    FindBestMove(line);
  }
}

void MoveFinder::FindBestMove(const std::string &sequence) {
  using cl = std::chrono::high_resolution_clock;
  Position pos;
  if (pos.Play(sequence) != sequence.size()) {
    std::cout << "Invalid move: " << sequence << '\n';
  } else {
    auto start = cl::now();
    const int score = solver.Solve(pos);
    const int best_move = solver.FindBestMove(pos);
    auto end = cl::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    Log(best_move, score, pos.NumMoves(), solver.GetNodeCount(),
        duration.count(), sequence);
  }
}

void MoveFinder::Log(const int best_move, const int score,
                     const int number_of_moves, uint64_t nodes_explored,
                     double time_taken, const std::string &sequence) {
  std::cout << sequence << ": " << number_of_moves << " moves, "
            << "Score: " << score << ", Nodes: " << nodes_explored
            << ", Time: " << time_taken << " ms"
            << ", Best move: column " << best_move + 1 << '\n';
}