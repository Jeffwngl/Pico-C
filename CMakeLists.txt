cmake_minimum_required(VERSION 3.10)

project(picoc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(source_dir "${PROJECT_SOURCE_DIR}/src/")

add_executable(${CMAKE_PROJECT_NAME} 
    src/main.cpp
    src/lexer/lexer.cpp
)

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
)