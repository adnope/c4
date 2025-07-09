#include "app.hpp"
#include "board_analyzer.hpp"
#include "game.hpp"

namespace cli {
  void App::Analyze() {
    cli::BoardAnalyzer board_analyzer(opening_book, warmup_book);
    board_analyzer.Run();
  }

  void App::FindBestMove() {
    cli::BoardAnalyzer board_analyzer(opening_book, warmup_book);
    board_analyzer.Run();
  }

  void App::StartGame() {
    Game game(opening_book, warmup_book);
    game.StartPlayerVsBotGame();
  }

  void App::StartBotGame() {
    Game game(opening_book, warmup_book);
    game.StartBotGame();
  }

  void App::StartTraining() {
    Game game(opening_book, warmup_book);
    game.StartTraining();
  }
}  // namespace cli
