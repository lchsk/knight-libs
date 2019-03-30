CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Name of the project
project(knight-libs)

# Set default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type (Debug or Release)" FORCE)
endif()

message(STATUS "CMake build type: " ${CMAKE_BUILD_TYPE})

# Version of the project
set(PROJECT_VERSION "1.0.0")

# If build type is Debug, generate debugging information and don't optimise
if (CMAKE_BUILD_TYPE STREQUAL Debug)
  add_definitions(-O0)
  set(CMAKE_C_FLAGS "-g -Wall -pedantic")
else()
  add_definitions(-O3)
  set(CMAKE_C_FLAGS "-Wall -pedantic")
endif()

# C standard for the compiler to use
set(CMAKE_CXX_STANDARD 17)

# Source code files, all .hpp and .cpp files from src/ directory are included
# file(GLOB SOURCES "src/*.hpp" "src/*.cpp")

# Executable name
add_executable(example_animation examples/animation.cpp src/animation.cpp src/loader.cpp)


############################################
#                                          #
#              Dependencies                #
#                                          #
############################################

find_package(PkgConfig)

pkg_check_modules(PKG_CONFIG "pkg-config")

pkg_check_modules(SFML sfml-all REQUIRED)
include_directories(
  ${SFML_INCLUDE_DIRS}
  )

link_directories(
  ${SFML_LIBRARY_DIRS}
  )

add_definitions(
  ${SFML_CXXFLAGS_OTHER}
  )

target_link_libraries(example_animation
  ${SFML_LIBRARIES}
  )


############################################
#                                          #
#                  Tests                   #
#                                          #
############################################

pkg_check_modules(GTEST "gtest")

enable_testing()

add_executable(test_loader
  tests/test_loader.cpp
  ../src/loader.cpp
  )

add_executable(test_animation
  tests/test_animation.cpp
  ../src/loader.cpp
  ../src/animation.cpp
  )

# add_test(NAME
  # test_knight_libs
  # COMMAND ./test_loader
  # )

target_link_libraries(test_loader
  ${GTEST_LIBRARIES}
  ${SFML_LIBRARIES}
  )

target_link_libraries(test_animation
  ${GTEST_LIBRARIES}
  ${SFML_LIBRARIES}
  )
