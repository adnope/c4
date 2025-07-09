#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

#include "robin/robin_hood.h"

class TranspositionTable {
 public:
  explicit TranspositionTable(const size_t size) : memoi_table(size) {
    assert(size > 0);
    constexpr int OPENING_TABLE_SIZE = 10e6;
    opening_table.reserve(OPENING_TABLE_SIZE);
  }

  void Reset();

  void Put(uint64_t key, uint8_t val);

  uint8_t Get(uint64_t key) const;

  void PutOpeningMove(uint64_t key, uint8_t score) {
    opening_table.emplace(key, score);
  }

  int GetMemoiEntriesCount() const { return entries_count; }

  int GetNumOfCollisions() const { return collisions; }

  size_t GetMemoiTableSize() const { return memoi_table.size(); }

  size_t GetOpeningTableSize() const { return opening_table.size(); }

 private:
  struct Entry {
    uint64_t key;
    uint8_t val;
  };

  robin_hood::unordered_flat_map<uint64_t, uint8_t> opening_table;
  std::vector<Entry> memoi_table;

  size_t index(const uint64_t key) const { return key % memoi_table.size(); }

  int entries_count = 0;
  int collisions = 0;
};