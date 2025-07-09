#include "argparser.hpp"

#include <iostream>
#include <string>

#include "cxxopts/cxxopts.hpp"
#include "game.hpp"
#include "move_finder.hpp"

ArgParser::ArgParser(const int argc, const char **argv)
    : options("c4", "A Connect Four game solver"), argc(argc), argv(argv) {
  options.add_options("MODE")(
      "f,find", "Find the best move for a given sequence of game state.",
      cxxopts::value<bool>()->default_value("false")->implicit_value("true"))(
      "p,play", "Play a game versus bot.",
      cxxopts::value<bool>()->default_value("false")->implicit_value("true"))(
      "b,botgame", "See a game between 2 bots.",
      cxxopts::value<bool>()->default_value("false")->implicit_value("true"))(
      "h,help", "Print this menu",
      cxxopts::value<bool>()->default_value("false")->implicit_value("true"))(
      "t,training", "Start a training session.",
      cxxopts::value<bool>()->default_value("false")->implicit_value("true"));

  options.add_options("BOOK")(
      "opening-book", "Specify an opening book.",
      cxxopts::value<std::string>()->default_value("data/opening.book"))(
      "warmup-book", "Specify a warmup book.",
      cxxopts::value<std::string>()->default_value("data/warmup.book"));

  options.parse_positional({"opening-book", "warmup-book"});

  constexpr int OPTION_LENGTH = 100;
  options.show_positional_help()
      .custom_help("MODE")
      .positional_help("[BOOK...]")
      .set_width(OPTION_LENGTH);
}

void ArgParser::Run() {
  if (argc == 1) {
    return;
  }
  HandleArgs(Parse());
}

cxxopts::ParseResult ArgParser::Parse() {
  cxxopts::ParseResult result;
  try {
    result = options.parse(argc, argv);
  } catch (cxxopts::exceptions::parsing &e) {
    std::cout << e.what() << '\n' << options.help();
    return result;
  }
  if (result.contains("help")) {
    std::cout << options.help();
  }

  return result;
}

void ArgParser::HandleArgs(const cxxopts::ParseResult &result) {
  const int find_option_count = static_cast<int>(result.count("find"));
  const int play_option_count = static_cast<int>(result.count("play"));
  const int botgame_option_count = static_cast<int>(result.count("botgame"));
  const int training_option_count = static_cast<int>(result.count("training"));

  if (find_option_count + play_option_count + botgame_option_count +
          training_option_count >
      1) {
    std::cerr << "Specify 1 option only.\n";
    std::cout << options.help();
    return;
  }

  const bool find = result["find"].as<bool>();
  const bool play = result["play"].as<bool>();
  const bool botgame = result["botgame"].as<bool>();
  const bool training = result["training"].as<bool>();
  const std::string opening_book = result["opening-book"].as<std::string>();
  const std::string warmup_book = result["warmup-book"].as<std::string>();

  if (find) {
    MoveFinder move_finder(opening_book, warmup_book);
    move_finder.Run();
  } else if (play || botgame || training) {
    Game game(opening_book, warmup_book);
    if (play) {
      game.StartPlayerVsBotGame();
    } else if (botgame) {
      game.StartBotGame();
    } else if (training) {
      game.StartTraining();
    }
  }
}