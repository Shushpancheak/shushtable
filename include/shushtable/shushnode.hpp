#pragma once
#include "util/ShushString.hpp"

/**
 * The class for a node in a hashtable's bucket.
 */
struct ShushNode {
  static const size_t STRINGS_IN_NODE = 4096 / sizeof(ShushString);

  /**
   * Refer to ShushString's constructor
   */
  ShushNode(const char* c_string);

  ShushNode* next = nullptr;

  ShushString strings_;
};

inline ShushNode::ShushNode(const char* c_string)
  : strings_(c_string) {
}