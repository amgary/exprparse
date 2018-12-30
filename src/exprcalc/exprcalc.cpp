// exprcalc.cpp
//
// Example program using exprparse library
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

#include "exprparse.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "ExprCalc - Simple Calculator" << endl;
    cout << "    Version " << exprparse::get_version() << endl;

    // Main loop
    for (;;) {
        double result;
        exprparse::Status res_stat;
        string expr;
        cout << "Enter simple math expression: ";
        getline(cin, expr);
        res_stat = exprparse::parse_expression(expr, &result);
        if (res_stat == exprparse::Status::SUCCESS) {
            cout << result << endl;
        } else {
            cout << exprparse::get_status_string(res_stat) << endl;
        }
    }

    exit(1);
}
