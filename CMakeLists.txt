cmake_minimum_required(VERSION 3.10)

project(MyProject)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Specify the include directory
include_directories('include')

# Add all source files, including their implementation files
file(GLOB_RECURSE SOURCES "src/*.cpp")  # Automatically include all .cpp files in src

add_executable(MyProject ${SOURCES})

target_link_libraries(MyProject sfml-graphics sfml-window sfml-system)

