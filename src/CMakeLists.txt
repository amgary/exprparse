CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (EXPRPARSE)

include(CTest)

add_subdirectory(exprparse)
add_subdirectory(exprcalc)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif(BUILD_TESTING)
