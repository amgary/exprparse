// exprparse.h
//
// Library for parsing simple math expressions
//
// Author: Alex Gary
// Copyright 2018 Alex Gary

#include <string>

namespace exprparse {
    typedef enum {
        SUCCESS,
        ERROR,
        DIVIDE_BY_ZERO,
        EMPTY_EXPRESSION,
        UNKNOWN_TOKEN,
        UNMATCHED_BRACKETS,
        TOO_FEW_ARGUMENTS,
        TOO_MANY_ARGUMENTS
    } Status;

    // Function to parse a simple match expression and compute
    // its value.
    //
    // Arguments:
    //  expression: string that contains a mathematical expression
    //  result: double used to store the result of the computation
    //
    Status parse_expression(const std::string& expression, double* result);

    // Returns string name of the status enum
    std::string get_status_string(const Status& status);

    // Returns version string
    std::string get_version();
} // namespace exprparse
