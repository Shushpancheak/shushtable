/**
 * The following file contains the source code for analysing the shushtable.
 */
#include "shushtable/shushtable.hpp"
#include <chrono>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "sys/stat.h"
#include "sys/mman.h"
#include "fcntl.h"
#include "unistd.h"

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using milli_s = std::chrono::milliseconds;

const char TEXT_PATH[] = "../text.txt";

int main() {
  int fd = open(TEXT_PATH, O_RDONLY);
  if (!fd) {
    std::cout << "Could not open file" << std::endl;
    return 1;
  }

  auto time_init = steady_clock::now();
  std::cout << "Inserting into the table..." << std::endl;

  ShushTable table;

  struct stat file_stat {};
  lstat(TEXT_PATH, &file_stat);
  const size_t file_size = file_stat.st_size;
  const size_t mmap_size = ((file_size / 4096) + 1) * 4096;

  char* data = static_cast<char*>(mmap(nullptr, mmap_size, PROT_WRITE, MAP_PRIVATE, fd, 0));
  if (data == MAP_FAILED) {
    std::cout << "Could not map file: " << std::endl;
    perror("mmap");
    return 1;
  }

  size_t start = 0;
  for (size_t i = 0; i <= file_size; ++i) {
    if (isalpha(data[i])) {
      continue;
    }

    data[i] = '\0';

    if (start < i - 1) {
      table.Add(data + start);
    }

    start = i + 1;
  }

  auto time_table_filled = steady_clock::now();
  std::cout << "Time took to insert all words: " << duration_cast<milli_s>(time_table_filled - time_init).count() << " ms." << std::endl <<
                "Starting Find series..." << std::endl;

  for (size_t i = 0; i < 10; ++i) {
    for (size_t j = 0; j < file_size; ++j) {
      if (data[i] == '\0' && data[i + 1] != '\0') {
        assert(table.Find(data + i + 1) == ShushTableError::NO_ERROR);
      }
    }
  }

  auto time_end = steady_clock::now();
  std::cout << "Time for Find: " << duration_cast<milli_s>(time_end - time_table_filled).count() << " ms." << std::endl;

  munmap(data, mmap_size);
  close(fd);
  return 0;
}