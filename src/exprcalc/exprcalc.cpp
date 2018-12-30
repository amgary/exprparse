// exprcalc.cpp
//
// Example program using exprparse library
//
// Author: Alex Gary
// Copyright 2018 Alex Gary
#include <iostream>
#include <string>
#include "exprparse.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "ExprCalc - Simple Calculator" << endl;
    cout << "    Version " << exprparse::get_version() << endl;

    // Main loop
    for (;;)
    {
        double result;
        exprparse::Status res_stat;
        string expr;
        cout << "Enter simple math expression: ";
        getline(cin, expr);
        res_stat = exprparse::parse_expression(expr, &result);
        if (res_stat == exprparse::Status::SUCCESS) {
            cout << result << endl;
        }
        else {
            cout << exprparse::get_status_string(res_stat) << endl;
        }
    }

    exit(1);
}
