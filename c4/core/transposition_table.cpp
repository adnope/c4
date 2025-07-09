#include "transposition_table.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

void TranspositionTable::Reset() {
  std::memset(memoi_table.data(), 0, memoi_table.size() * sizeof(Entry));
  entries_count = 0;
  collisions = 0;
}

void TranspositionTable::Put(const uint64_t key, const uint8_t val) {
  if (entries_count >= static_cast<int>(memoi_table.size() / 2)) {
    Reset();
  }
  size_t idx = index(key);
  while (memoi_table[idx].key != 0 && memoi_table[idx].key != key) {
    idx = (idx + 1) % memoi_table.size();
    collisions++;
  }
  if (memoi_table[idx].key == 0) {
    entries_count++;
  }
  memoi_table[idx] = {key, val};
}

uint8_t TranspositionTable::Get(const uint64_t key) const {
  if (opening_table.contains(key)) {
    return opening_table.at(key);
  }
  size_t idx = index(key);
  while (memoi_table[idx].key != 0) {
    if (memoi_table[idx].key == key) {
      return memoi_table[idx].val;
    }
    idx = (idx + 1) % memoi_table.size();
  }
  return 0;
}