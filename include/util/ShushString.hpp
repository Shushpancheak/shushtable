#pragma once

/**
 * A string class that stores character arrays in static buffers.
 * It also provides at least 8 additional null bytes after a string stored.
 * (Useful for CRC64 algorithms)
 */
#include <cstring>

class ShushString {
  // -1 for the size_ byte.
  static const size_t MAX_SIZE = 255;
  static const size_t TERMINATING_NULLS_COUNT = 8;
public:
  /**
   * Construct ShushString from a c-style string.
   * The string must be maximum to MAX_SIZE long.
   * If this is not the case, the string will be stripped.
   *
   * @param c_string c-style string to construct from.
   */
  explicit ShushString(const char* c_string);

  [[nodiscard]] unsigned char size() const;
  char* c_str();
  [[nodiscard]] const char* c_str() const;

private:
  unsigned char size_;
  char data_[MAX_SIZE] {};

  // So that if string was MAX_SIZE size,
  // it would still have 8 terminating nulls.
  char null_[TERMINATING_NULLS_COUNT] {};
};

inline bool operator==(const ShushString& s_1, const ShushString& s_2) {
  if (s_1.size() != s_2.size()) {
    return false;
  }

  return !strcmp(s_1.c_str(), s_2.c_str());
}

inline ShushString::ShushString(const char* c_string)
  : size_(0) {
  // TODO redo this for a more efficient realization.
  strncpy(data_, c_string, MAX_SIZE);
  size_ = strlen(data_);
}

inline unsigned char ShushString::size() const {
  return size_;
}

inline char* ShushString::c_str() {
  return data_;
}

inline const char* ShushString::c_str() const {
  return data_;
}