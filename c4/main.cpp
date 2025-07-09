#include <iostream>
#include <map>
#include <string>

#include "app/cli/app.hpp"
#include "cxxopts/cxxopts.hpp"

static cxxopts::Options initOptions(
    const std::map<std::string, std::string>& option_list) {
  cxxopts::Options options("c4", "A Connect Four game solver");

  for (const auto& [option, description] : option_list) {
    options.add_options("MODE")(
        option, description,
        cxxopts::value<bool>()->default_value("false")->implicit_value("true"));
  }

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

  return options;
}

static void parseArgs(const int argc, const char** argv) {
  // Add new options here
  const std::map<std::string, std::string> option_list = {
      {"a,analyze", "Analyze a game state"},
      {"p,play", "Play a game versus bot"},
      {"b,botgame", "Watch a game between 2 bots"},
      {"h,help", "Print this help menu"},
      {"t,training", "Start a training session"}};

  auto options = initOptions(option_list);

  cxxopts::ParseResult result;
  try {
    result = options.parse(argc, argv);
  } catch (cxxopts::exceptions::parsing& e) {
    std::cout << e.what() << '\n' << options.help();
    return;
  }

  if (result.contains("help")) {
    std::cout << options.help();
    return;
  }

  size_t option_count = 0;
  for (const auto& [option, description] : option_list) {
    const std::string option_name = option.substr(option.find(',') + 1);
    option_count += result.count(option_name);
  }
  if (option_count > 1) {
    std::cerr << "Specify 1 option only.\n";
    return;
  }

  const auto opening_book = result["opening-book"].as<std::string>();
  const auto warmup_book = result["warmup-book"].as<std::string>();

  cli::App cli_app(opening_book, warmup_book);

  // Specify actions for new options here
  for (const auto& [option, description] : option_list) {
    const std::string option_name = option.substr(option.find(',') + 1);
    if (result[option_name].as<bool>()) {
      if (option_name == "analyze") {
        cli_app.Analyze();
      }
      if (option_name == "botgame") {
        cli_app.StartBotGame();
      }
      if (option_name == "play") {
        cli_app.StartGame();
      }
      if (option_name == "training") {
        cli_app.StartTraining();
      }
    }
  }
}

int main(const int argc, const char** argv) {
  parseArgs(argc, argv);

  /**
  TODO:
  - CLI:
  - 2 player game
  - handle the default ob and wb book better
  - finalize training mode, auto contribute to warmup book
  - GUI:
  - Main menu
  - Play game vs bot
  - 2 player game
  - Multiplayer (LAN)
  - Multiplayer (Online)
  */

  return 0;
}
