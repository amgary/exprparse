CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(EXPRPARSE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to exprparse headers")

ADD_LIBRARY(exprparse
    exprparse.h
    exprparse.cpp
)

install(TARGETS exprparse
	LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
