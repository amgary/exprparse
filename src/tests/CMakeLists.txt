cmake_minimum_required(VERSION 2.8)

include_directories(${EXPRPARSE_INCLUDE_DIR})
add_executable(exprtests main.cpp)
target_link_libraries(exprtests
exprparse
)