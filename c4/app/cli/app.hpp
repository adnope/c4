#pragma once

#include <string>

namespace cli {
class App {
 public:
  explicit App(const std::string& opening_book, const std::string& warmup_book)
      : opening_book(opening_book), warmup_book(warmup_book) {}

  void Analyze();
  void FindBestMove();
  void StartGame();
  void StartBotGame();
  void StartTraining();

 private:
  std::string opening_book;
  std::string warmup_book;
};
}  // namespace cli
