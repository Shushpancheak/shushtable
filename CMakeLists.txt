cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

project (
  "shushtable"
  VERSION 1.0.0
  DESCRIPTION ""
)
set (
  SOURCES
  src/main.cpp
        src/shushtable.cpp
)
set (
  PUBLIC_HEADERS
        include/shushtable/shushtable.hpp
        include/shushtable/shushnode.hpp
        include/util/ShushString.hpp
        include/util/asserts.hpp
)

include_directories(include)

add_executable(${PROJECT_NAME}-exe ${SOURCES} ${PUBLIC_HEADERS})

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - LIBRARY INITIALIZATION- - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

set (
  LIBRARY_NAME
  ${PROJECT_NAME}-lib
)
set (
  LIBRARY_TYPE STATIC
  CACHE STRING "Type of library to build. STATIC or SHARED."
)

add_definitions(" -mfpmath=sse -mmmx -msse -msse4.2")

#add_library (
#  ${LIBRARY_NAME} ${LIBRARY_TYPE}
#  ${SOURCES}
#)
#set_target_properties (
# ${LIBRARY_NAME} PROPERTIES
#  VERSION         ${PROJECT_VERSION}
#  PUBLIC_HEADER   ${PUBLIC_HEADERS}
#)
#target_include_directories(${LIBRARY_NAME} PUBLIC  include)
#target_include_directories(${LIBRARY_NAME} PRIVATE src)


set(BUILD_TESTS OFF CACHE BOOL "Build tests")
if (BUILD_TESTS)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - GTEST INITIALIZATION- - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

add_subdirectory(googletest)
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include_directories(${gtest_SOURCE_DIR})

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - TESTS - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

set(UNIT_TESTS_NAME "run-tests-${PROJECT_NAME}")
set(UNIT_TESTS_FILE "test/test.cpp")

add_executable(${UNIT_TESTS_NAME} ${UNIT_TESTS_FILE})

# Link test executable against gtest & gtest_main
target_link_libraries(${UNIT_TESTS_NAME} gtest_main ${LIBRARY_NAME})
add_test(${UNIT_TESTS_NAME} ${UNIT_TESTS_NAME})

# For next libraries.
set(BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)

endif() # BUILD_TESTS

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - DEPENDENCIES- - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
