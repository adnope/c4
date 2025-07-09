#include "board_analyzer.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <ratio>
#include <string>

#include "core/solver.hpp"

namespace cli {
using std::max_element;

BoardAnalyzer::BoardAnalyzer(const std::string &ob_path,
                             const std::string &wb_path) {
  solver.GetReady(ob_path, wb_path);
}

void BoardAnalyzer::Run() {
  std::string line;
  while (std::cout << "\nEnter your sequence: ", std::getline(std::cin, line)) {
    Analyze(line);
  }
}

void BoardAnalyzer::FindBestMove(const std::string &sequence) {
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

void BoardAnalyzer::Analyze(const std::string &sequence) {
  using cl = std::chrono::high_resolution_clock;
  Position pos;
  if (pos.Play(sequence) != sequence.size()) {
    std::cout << "Invalid move: " << sequence << '\n';
  } else {
    auto start = cl::now();
    const auto result = solver.ScoreColumns(pos);
    auto end = cl::now();
    std::chrono::duration<double, std::milli> time_taken = end - start;

    auto best_move = std::max_element(result.begin(), result.end()) - result.begin();

    std::cout << "Sequence: " << sequence << '\n';
    PrintBoard(sequence);
    std::cout << "Scores: ";
    for (const int i : result) {
      std::cout << i << " ";
    }
    std::cout << "\nBest move: column " << best_move + 1 << ".\n";
    std::cout << "Nodes explored: " << solver.GetNodeCount() << ".\n";
    std::cout << "Time taken: " << time_taken.count() << " ms.\n";
  }
}

void BoardAnalyzer::Log(const int best_move, const int score,
                        const int number_of_moves, uint64_t nodes_explored,
                        double time_taken, const std::string &sequence) {
  std::cout << sequence << ": " << number_of_moves << " moves, "
            << "Score: " << score << ", Nodes: " << nodes_explored
            << ", Time: " << time_taken << " ms"
            << ", Best move: column " << best_move + 1 << '\n';
}

void BoardAnalyzer::PrintBoard(const std::string &sequence) {
  constexpr int ROWS = Position::HEIGHT;
  constexpr int COLS = Position::WIDTH;
  std::vector board(ROWS, std::vector<char>(COLS, 0));

  for (auto &row : board) {
    for (char &col : row) {
      col = '.';
    }
  }

  for (size_t i = 0; i < sequence.size(); i++) {
    const int col = sequence[i] - '1';
    if (col < 0 || col >= COLS) {
      std::cout << "Invalid column: " << sequence[i] << "\n";
      return;
    }

    int row = ROWS - 1;
    while (row >= 0 && board[row][col] != '.') {
      row--;
    }

    if (row < 0) {
      std::cerr << "Column " << col + 1 << " is already full!" << "\n";
      return;
    }

    board[row][col] = (i % 2 == 0) ? 'x' : 'o';
  }

  for (const auto &row : board) {
    std::cout << "|";
    for (const char col : row) {
      std::cout << col << "|";
    }
    std::cout << '\n';
  }

  std::cout << " ";
  for (int j = 1; j <= COLS; j++) {
    std::cout << j << " ";
  }
  std::cout << '\n';
}
}  // namespace cli