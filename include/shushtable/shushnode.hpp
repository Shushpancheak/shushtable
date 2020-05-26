#pragma once
#include "util/ShushString.hpp"

/**
 * The class for a node in a hashtable's bucket.
 */
struct ShushNode {
  /**
   * Refer to ShushString's constructor
   */
  ShushNode(const char* c_string);

  ShushNode* next = nullptr;
  ShushString str_;
};

inline ShushNode::ShushNode(const char* c_string)
  : str_(c_string) {
}