#pragma once
#include "shushnode.hpp"
#include "util/asserts.hpp"

enum class ShushTableError {
  NO_ERROR = 0,
  ENTRY_ALREADY_EXISTS,
  ENTRY_NOT_FOUND,
  ALLOCATION_ERROR
};

class ShushTable {
  const size_t INITIAL_SIZE    = 256;
  const double MAX_LOAD_FACTOR = 0.75;
public:
  ShushTable();
  ~ShushTable();

  ShushTableError Add(const char* c_string);
  ShushTableError Find(const char* c_string);
  ShushTableError Rehash();

  /**
   * Checks consistency of the class.
   */
  bool Check();

private:
  void RecalculateLoadFactor();
  /**
   * Uses CRC32C algorithm.
   */
  static uint64_t Hash(const ShushString& shush_string);
  /**
   * Adds the potential_node to the hashtable. It must be allocated with operator new.
   */
  ShushTableError AddNode(ShushNode* potential_node);
  /**
   * Adds a node to the bucket list and updates all table's variables.
   */
  void InsertNewNode(ShushNode** where_to_place, ShushNode* new_node);
  /**
   * Allocates buckets_ array according to size_.
   */
  ShushTableError AllocateBuckets();
  /**
   * Finds the node containing the string in the table.
   * @return pointer to the node.
   */
  ShushNode* FindNode(const ShushString& shush_string);

  double load_factor_;
  size_t nodes_cnt_;
  size_t size_;
  ShushNode** buckets_;
};