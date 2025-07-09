#include "move_sorter.hpp"

#include <cstdint>

void MoveSorter::Add(const uint64_t move, const int score) {
  unsigned int pos = size++;
  for (; pos != 0 && entries.at(pos - 1).score > score; --pos) {
    entries.at(pos) = entries.at(pos - 1);
  }

  entries.at(pos).move = move;
  entries.at(pos).score = score;
}

uint64_t MoveSorter::GetNext() {
  if (size != 0) {
    --size;
    return entries.at(size).move;
  }
  return 0;
}