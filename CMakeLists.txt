cmake_minimum_required(VERSION 3.8)
project(kmac)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
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

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
