CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (EXPRPARSE)

include(CTest)

set (CMAKE_CXX_STANDARD 11)
add_subdirectory(exprparse)
add_subdirectory(exprcalc)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif(BUILD_TESTING)

install(DIRECTORY ${EXPRPARSE_INCLUDE_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.h")
