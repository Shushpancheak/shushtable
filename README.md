# shushtable
A fast implementation of a hashtable on short strings.

## Efficiency analysis
The original results:
|![](img/O2g-orig.png) |![](img/03g-orig.png) |
:-:-:
|-O2 -g| -O3 -g|

## Build
```shell
mkdir build && cd build
cmake .. # "-UBUILD_TESTS -DBUILD_TESTS=ON" to build tests, "-ULIBRARY_TYPE -DLIBRARY_TYPE=SHARED" for building shared library
make
```

## How to use
Download the repository and place it into your project directory. In your project's CMakeLists.txt file, insert the following lines:
```cmake
...
add_subdirectory(shushtable)
...
target_link_libraries(${PROJECT_NAME} shushtable)
...
```

## Build documentation
```shell
doxygen
```
Documentation will be generated in the directory named `docs`.
