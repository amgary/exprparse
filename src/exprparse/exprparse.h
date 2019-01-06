// exprparse.h
//
// Library for parsing simple math expressions
//
// MIT License
//
// Copyright (c) 2018 Alex Gary
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef EXPRPARSE_H
#define EXPRPARSE_H

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

#endif // !EXPRPARSE_H
