# shushtable
A fast implementation of a hashtable on short strings.

## Efficiency analysis
The original results:
|![](img/O2g-orig.png)|![](img/03g-orig.png)|
|---------------------|---------------------|
|-O2 -g               |               -O3 -g|

In `KCacheGrind` we can notice two things:
1. There are too many cache misses.

![](img/for-orig.png)

2. There is quite some time spent on calculating crc.

![](img/crc-orig.png)

After applying some intrinsics to the code, we yield the following result:
|![](img/02g-intrin.png)|![](img/O3g-intrin.png)|
|-----------------------|-----------------------|
|-O2 -g                 |              -O3 -g   |

After allocating nodes in chunks of `PAGE_SIZE` length, we get:
|![](img/02g-intrin-cache.png)|![](img/03g-intrin-cache.png)|
|-----------------------------|-----------------------------|
|-O2 -g                       |         -O3 -g              |

Final result:

![](img/final.png)


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
