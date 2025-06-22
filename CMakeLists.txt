# CMake project settings
cmake_minimum_required(VERSION 3.28)
project(graphics)

# Set source files
set( SRC_FILES
    "src/main.cpp"
)

# Set executable
add_executable(graphics ${SRC_FILES})

# Set include directories
target_include_directories(graphics PUBLIC "src/")
target_include_directories(graphics PUBLIC "lib/")
target_include_directories(graphics PUBLIC "lib/Eigen")