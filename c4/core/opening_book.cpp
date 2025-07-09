#include "opening_book.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>

void OpeningBook::load(const std::string &book_file) const {
  std::ifstream binary_file(book_file, std::ios::binary);
  uint64_t move_key = 0;
  uint8_t score = 0;

  std::array<char, sizeof(move_key)> move_buf{};
  std::array<char, sizeof(score)> score_buf{};

  while (binary_file.read(move_buf.data(), move_buf.size()) &&
         binary_file.read(score_buf.data(), score_buf.size())) {
    std::memcpy(&move_key, move_buf.data(), move_buf.size());
    std::memcpy(&score, score_buf.data(), score_buf.size());

    T->PutOpeningMove(move_key, score);
  }
}
