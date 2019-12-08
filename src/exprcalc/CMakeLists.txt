cmake_minimum_required(VERSION 2.8)

include_directories(${EXPRPARSE_INCLUDE_DIR})
add_executable(exprcalc
    exprcalc.cpp
)
target_link_libraries(exprcalc
exprparse
)

install(TARGETS exprcalc
    RUNTIME DESTINATION bin)
