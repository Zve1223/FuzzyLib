cmake_minimum_required(VERSION 3.5.0)
project(
    Fuzzy
    VERSION 0.1.0
    LANGUAGES C CXX
)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall)
add_compile_options(-Wextra)

# add_subdirectory(lib)
# add_subdirectory(bin)

add_executable(${PROJECT_NAME} main.cpp)