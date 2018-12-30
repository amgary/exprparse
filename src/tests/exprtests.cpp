// exprtests.cpp
//
// Library of tests for exprparse.cpp
//
// Author: Alex Gary
// Copyright 2018 Alex Gary
//
#include "exprtests.h"
#include "gtest/gtest.h"

#include <cstddef>
#include <math.h>

using namespace std;

namespace exprparse {

    void common_success_test_eval(string expression, double expected_value) {
        double result_value;
        Status ret_val;
        cerr << "[          ]     Expr = " << expression << endl;
        ret_val = parse_expression(expression, &result_value);
        EXPECT_EQ(ret_val, Status::SUCCESS);
        EXPECT_DOUBLE_EQ(result_value, expected_value);
    }

    void common_error_test(string expression, Status expected_status) {
        double result_value;
        Status ret_val;
        cerr << "[          ]     Expr = " << expression << endl;
        ret_val = parse_expression(expression, &result_value);
        EXPECT_EQ(ret_val, expected_status);
    }

    TEST(ParseNumber, PositiveSpace) {
        string expression = string(" 10.0");
        double expected_value = 10.0;
        common_success_test_eval(expression, expected_value);
    }

    TEST(ParseNumber, NegativeNumber) {
        string expression = string("-10.0");
        double expected_value = -10.0;
        common_success_test_eval(expression, expected_value);
    }

    TEST(ParseNumber, Exponential) {
        common_success_test_eval("10.0e5", 10.0e5);
        common_success_test_eval("10.0E5", 10.0E5);
        common_success_test_eval("10.0E+05", 10.0E5);
    }

    TEST(ParseNumber, NegativeExp) {
        common_success_test_eval("10.0e-5", 10.0e-5);
        common_success_test_eval("10.0E-5", 10.0E-5);
        common_success_test_eval("10.0E-05", 10.0E-5);
    }

    TEST(Operators, BinaryAdd) {
        common_success_test_eval("10.0 + 5.0", 15.0);
        common_success_test_eval("10.0+5.0", 15.0);
    }

    TEST(Operators, BinarySubtract) {
        common_success_test_eval("10.0 - 5.0", 5.0);
        common_success_test_eval("10.0-5.0", 5.0);
    }

    TEST(Operators, Multiply) {
        common_success_test_eval("2.5*5.0", 2.5 * 5.0);
        common_success_test_eval("10e+5*2.0", 10.0e5 * 2.0);
    }

    TEST(Operators, Divide) {
        common_success_test_eval("10.0/5.0", 2.0);
        common_success_test_eval("15.0/5.0", 3.0);
    }

    TEST(Operators, Power) {
        common_success_test_eval("2.0**3.0", 8.0);
        common_success_test_eval("2.0^4.0", 16.0);
        common_success_test_eval("4.0^0.5", 2.0);
    }

    TEST(Operators, Unary) {
        common_success_test_eval("-2.0", -2.0);
        common_success_test_eval("-10.0/-2.0", 5.0);
        common_success_test_eval("-10.0/+3.0", -10.0 / 3.0);
    }

    TEST(Precedence, Multiply) {
        common_success_test_eval("5.0-3.0*5.0", -10.0);
    }

    TEST(Precedence, Divide) {
        common_success_test_eval("5.0-10.0/-5.0", 7.0);
    }

    TEST(Precedence, Power) {
        common_success_test_eval("5.0+4.0^-0.5", 5.5);
        common_success_test_eval("5.0+4.0**-0.5", 5.5);
        common_success_test_eval("5.0-4.0**2.0", -11.0);
        common_success_test_eval("3.0^2.0^3.0", 6561.0);
    }

    TEST(Precedence, Grouping) {
        common_success_test_eval("(12.0+4.0)^-0.5", 0.25);
        common_success_test_eval("(12.0+4.0)^0.5/5.0", 0.8);
    }

    TEST(InvalidExpression, UnmatchedBrackets) {
        common_error_test("(", Status::UNMATCHED_BRACKETS);
        common_error_test(")", Status::UNMATCHED_BRACKETS);
        common_error_test("(1-2", Status::UNMATCHED_BRACKETS);
        common_error_test("((1-2)+1/2", Status::UNMATCHED_BRACKETS);
    }

    TEST(InvalidExpression, DivideByZero) {
        common_error_test("2/0", Status::DIVIDE_BY_ZERO);
        common_error_test("5/(1-1)", Status::DIVIDE_BY_ZERO);
    }

    TEST(InvalidExpression, UnknownToken) {
        common_error_test("abc", Status::UNKNOWN_TOKEN);
        common_error_test("$", Status::UNKNOWN_TOKEN);
        common_error_test("&", Status::UNKNOWN_TOKEN);
    }

    TEST(InvalidExpression, TooManyArguments) {
        common_error_test("1.0 2.0", Status::TOO_MANY_ARGUMENTS);
        common_error_test("5.0(1.0+2.0)", Status::TOO_MANY_ARGUMENTS);
    }

    TEST(InvalidExpression, TooFewArguments) {
        common_error_test("*1.0", Status::TOO_FEW_ARGUMENTS);
        common_error_test("3.0/", Status::TOO_FEW_ARGUMENTS);
        common_error_test("4.0^", Status::TOO_FEW_ARGUMENTS);
    }
} // namespace exprparse
