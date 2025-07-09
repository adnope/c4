#include "game.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

Game::Game(const std::string &ob_book, const std::string &wb_book) {
  solver.GetReady(ob_book, wb_book);
}

void Game::printConnectFourBoard(const std::string &sequence) {
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

void Game::StartPlayerVsBotGame() {
  std::string sequence;
  Position pos;
  pos.Play(sequence);

  std::cout << "Choose your side:\n"
            << "[1]: Red\n"
            << "[2]: Yellow\n"
            << "Enter your choice: ";

  int choice = 0;
  while (std::cin >> choice) {
    if (choice == 1) {
      break;
    }
    if (choice == 2) {
      const int best_move = solver.FindBestMove(pos);
      pos.PlayCol(best_move);
      sequence += std::to_string(best_move + 1);
      break;
    }
    std::cout << "Invalid choice\n"
              << "Enter your choice: ";
  }

  std::cout << "=====================\n";
  std::cout << "The game has started!\n";
  std::cout << "=====================\n";

  int player_move = 0;
  while (true) {
    printConnectFourBoard(sequence);
    std::cout << "Enter your move: column: ";
    std::cin >> player_move;

    while (player_move < 1 || player_move > Position::WIDTH) {
      std::cout << "Invalid move\nEnter your move: ";
      std::cin >> player_move;
    }

    if (pos.IsWinningMove(player_move - 1)) {
      sequence += std::to_string(player_move);
      printConnectFourBoard(sequence);
      std::cout << "You win!\n";
      break;
    }

    sequence += std::to_string(player_move);
    pos.PlayCol(player_move - 1);

    const int ai_move = solver.FindBestMove(pos);
    if (pos.IsWinningMove(ai_move)) {
      std::cout << "Bot has played: column " << ai_move + 1 << '\n';
      sequence += std::to_string(ai_move + 1);
      printConnectFourBoard(sequence);
      std::cout << "You lose!\n";
      break;
    }
    pos.PlayCol(ai_move);
    sequence += std::to_string(ai_move + 1);
    std::cout << "Bot has played: column " << ai_move + 1 << '\n';
  }
}

void Game::StartBotGame() {
  std::cout << "\n====================\n"
            << "THE GAME HAS STARTED\n"
            << "====================\n";

  std::string sequence;
  Position pos;

  std::string player_name;
  bool is_red_turn = false;
  while (true) {
    is_red_turn = (sequence.size() % 2) == 0;
    std::cout << "Moves: " << sequence.size() << "\n";
    printConnectFourBoard(sequence);

    if (is_red_turn) {
      player_name = "Red";
    } else {
      player_name = "Yellow";
    }

    std::cout << player_name << " is thinking...\n";

    auto start = std::chrono::high_resolution_clock::now();
    const int move = solver.FindBestMove(pos);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << player_name << " has played: column " << move + 1 << ", "
              << duration.count() << " ms.\n";

    if (pos.IsWinningMove(move)) {
      sequence += std::to_string(move + 1);
      printConnectFourBoard(sequence);
      std::cout << player_name << " won!\n";
      return;
    }

    pos.PlayCol(move);
    sequence += std::to_string(move + 1);
  }
}

void Game::StartTraining() {
  using cl = std::chrono::high_resolution_clock;

  std::ofstream hard_moves_stream("hard_moves.txt");
  std::unordered_set<std::string> seen_lines;

  std::cout << "\nTRAINING SESSION STARTED!\n\n";

  std::string initial_sequence = "44444";
  std::string sequence = initial_sequence;
  Position pos;
  pos.Play(sequence);

  int move = 0;
  while (true) {
    constexpr int RESET_LIMIT = 20;
    if (pos.NumMoves() == RESET_LIMIT) {
      sequence = initial_sequence;
      pos = Position();
      pos.Play(sequence);
    }

    auto start = cl::now();
    // Play randomly at the 7th move of the board
    // if (P.nbMoves() == 7)
    // 	move = solver.RandomMove();
    // else
    move = solver.FindBestMove(pos);
    auto end = cl::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    constexpr int TIMEOUT = 2000;
    if (duration.count() >= TIMEOUT &&
        seen_lines.find(sequence) == seen_lines.end()) {
      std::cout << "HARD MOVE FOUND: " << sequence << ", " << duration.count()
                << " ms.\n";
      seen_lines.insert(sequence);
      hard_moves_stream << sequence << '\n';
      hard_moves_stream.flush();

      Position hard_pos;
      hard_pos.Play(sequence);
      for (int col = 0; col < Position::WIDTH; ++col) {
        Position next_pos(hard_pos);
        if (next_pos.CanPlay(col)) {
          next_pos.PlayCol(col);
          uint64_t key = next_pos.Key3();
          uint8_t score = solver.Solve(next_pos) - Position::MIN_SCORE + 1;
          solver.GetTranspositionTable().PutOpeningMove(key, score);
        }
      }
    }
    pos.PlayCol(move);
    sequence += std::to_string(move + 1);
  }
}