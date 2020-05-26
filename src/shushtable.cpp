#include "shushtable/shushtable.hpp"

ShushTable::ShushTable()
  : load_factor_(0)
  , nodes_cnt_(0)
  , size_(INITIAL_SIZE)
  , buckets_(nullptr) {
  assert(AllocateBuckets() == ShushTableError::NO_ERROR);
}

ShushTable::~ShushTable() {
  delete[] buckets_;
}

ShushTableError ShushTable::Add(const char* c_string) { $
  auto* potential_node = new ShushNode(c_string);
  return AddNode(potential_node);
}

bool ShushTable::Check() {
  bool load_factor_ok = (load_factor_ <= MAX_LOAD_FACTOR * 1.1);
  return load_factor_ok;
}

void ShushTable::RecalculateLoadFactor() {
  load_factor_ = static_cast<double>(nodes_cnt_) / size_;
}

uint64_t ShushTable::Hash(const ShushString& shush_string) {
  //TODO Use intel's intrinsics for crc32c computation.
  static const auto SIZE_T_ONE = static_cast<size_t>(1);
  static const auto SIZE       = sizeof(uint64_t);
  static const auto MSB        = SIZE_T_ONE << (SIZE - 1);
  static const auto POLYNOMIAL = 0x11EDC6F41;
  uint64_t crc = 0;

  auto* data = reinterpret_cast<const uint64_t*>(shush_string.c_str());
  for (; *data != 0; ++data) {
    for (int8_t i = SIZE - 1; i > 0; --i) {
      auto msb_set = crc & MSB;

      crc <<= 1;
      crc |= (*data & (SIZE_T_ONE << i)) ? 1 : 0;
      if (!msb_set) {
        continue;
      }

      crc ^= POLYNOMIAL;
    }
  }

  return crc;
}

void ShushTable::InsertNewNode(ShushNode** where_to_place, ShushNode* new_node) { $
  *where_to_place = new_node;
  new_node->next = nullptr;
  ++nodes_cnt_;

  RecalculateLoadFactor();
  if (load_factor_ >= MAX_LOAD_FACTOR) {
    Rehash();
  }
}

ShushTableError ShushTable::Rehash() {
  const size_t old_size = size_;
  auto*        old_buckets = buckets_;
  const double old_load_factor = load_factor_;

  size_       *= 2;
  load_factor_ = static_cast<double>(nodes_cnt_) / size_;
  if (AllocateBuckets() == ShushTableError::ALLOCATION_ERROR) {
    delete[] old_buckets;
    return ShushTableError::ALLOCATION_ERROR;
  }

  for (size_t bucket_id = 0; bucket_id < old_size; ++bucket_id) {
    if (old_buckets[bucket_id] == nullptr) {
      continue;
    }

    for (ShushNode* node = buckets_[bucket_id]; node != nullptr; node = node->next) {
      AddNode(node);
    }
  }

  delete[] old_buckets;
  return ShushTableError::NO_ERROR;
}

ShushTableError ShushTable::AddNode(ShushNode* potential_node) {
  assert(potential_node);

  uint64_t id = Hash(potential_node->str_) % size_;

  ShushNode* node = buckets_[id];
  ShushNode* prev_node = node;

  if (!node) {
    InsertNewNode(buckets_ + id, potential_node);
    return ShushTableError::NO_ERROR;
  }

  for (; node != nullptr; prev_node = node, node = node->next) {
    if (node->str_ == potential_node->str_) {
      delete potential_node;
      return ShushTableError::ENTRY_ALREADY_EXISTS;
    }
  }

  node = prev_node;

  InsertNewNode(&node->next, potential_node);
  return ShushTableError::NO_ERROR;
}

ShushTableError ShushTable::Find(const char* c_string) { $
  auto* node = FindNode(ShushString(c_string));
  return node == nullptr ? ShushTableError::ENTRY_NOT_FOUND : ShushTableError::NO_ERROR;
}

ShushTableError ShushTable::AllocateBuckets() {
  buckets_ = new ShushNode*[size_];
  memset(buckets_, 0, size_ * sizeof(void*));

  if (!buckets_) {
    return ShushTableError::ALLOCATION_ERROR;
  }

  return ShushTableError::NO_ERROR;
}

ShushNode* ShushTable::FindNode(const ShushString& shush_string) {
  uint64_t id = Hash(shush_string) % size_;

  for (ShushNode* node = buckets_[id]; node != nullptr; node = node->next) {
    if (node->str_ == shush_string) {
      return node;
    }
  }

  return nullptr;
}
