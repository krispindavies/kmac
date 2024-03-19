cmake_minimum_required(VERSION 3.8)
project(kmac)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

# If we're using clang, enable additional warning flags.
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find the various packages we depend on.
find_package(ament_cmake REQUIRED)

# Add the include directory.
include_directories(include)

# ------------------------------------------------
# Build/link all the targets.
# ------------------------------------------------

add_executable(
  kmac
  src/kmac.cpp
)
ament_target_dependencies(
  kmac
)

# ------------------------------------------------
# Install everything into the correct directories.
# ------------------------------------------------

install(
  DIRECTORY
    include/
  DESTINATION
    include/
)

install(
  TARGETS
    kmac
  DESTINATION
    lib/${PROJECT_NAME}
)

# ------------------------------------------------
# Set up unit testing.
# ------------------------------------------------

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)

  ament_add_gtest_executable(
    kmac_test
    test/kmac_test.cpp
  )
  ament_target_dependencies(
    kmac_test
  )
endif()

ament_package()
