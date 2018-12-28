// exprtests.cpp
//
// Library of tests for exprparse.cpp
//
// Author: Alex Gary
// Copyright 2018 Alex Gary
//
#include "exprtests.h"

#include <cstddef>
#include <math.h>

using namespace std;

namespace exprparse {
    //********************* Define all tests *****************************//
    void print_test_log_header()
    {
        fprintf(stdout, "%-30s%15s%15s%-15s\r\n", "Expression", "Expected", "Actual", " Status");
    }

    void log_test_result(const string & expression, double expected, double actual, Status & status)
    {
        if (status == Status::SUCCESS && fabs(expected - actual) > 1.0e-10)
        {
            status = Status::ERROR;
        }

        string status_message = get_status_string(status);
        fprintf(stdout, "%-29s %15.5f %14.5f %s\r\n", expression.c_str(), expected, actual, status_message.c_str());
    }

    Status test_simple_number()
    {
        string simple_number = string(" 10.0");
        double expected_value = 10.0;
        double result_value;
        Status ret_val;

        ret_val = parse_expression(simple_number, &result_value);
        log_test_result(simple_number, expected_value, result_value, ret_val);
        return Status::SUCCESS;
    }

    Status test_simple_negative_number()
    {
        string simple_number = string("-10.0");
        double expected_value = -10.0;
        double result_value;
        Status ret_val;

        ret_val = parse_expression(simple_number, &result_value);
        log_test_result(simple_number, expected_value, result_value, ret_val);
        return Status::SUCCESS;
    }

    Status test_add_numbers()
    {
        string simple_number = string(" 10.0+0.5E1");
        double expected_value = 10.0 + 0.5E1;
        double result_value;
        Status ret_val;

        ret_val = parse_expression(simple_number, &result_value);
        log_test_result(simple_number, expected_value, result_value, ret_val);
        return Status::SUCCESS;
    }

    Status test_add_negative_numbers()
    {
        string simple_number = string("-10.0+-0.5E1");
        double expected_value = -10.0 - 0.5E1;
        double result_value;
        Status ret_val;

        ret_val = parse_expression(simple_number, &result_value);
        log_test_result(simple_number, expected_value, result_value, ret_val);
        return Status::SUCCESS;
    }

    Status test_divide()
    {
        string simple_number = string("10.0/0.5E1");
        double expected_value = 10.0 / 0.5E1;
        double result_value;
        Status ret_val;

        ret_val = parse_expression(simple_number, &result_value);
        log_test_result(simple_number, expected_value, result_value, ret_val);
        return Status::SUCCESS;
    }

    Status test_divide_negative_numbers()
    {
        string simple_number = string("-10.0/-0.5E1");
        double expected_value = -10.0 / -0.5E1;
        double result_value;
        Status ret_val;

        ret_val = parse_expression(simple_number, &result_value);
        log_test_result(simple_number, expected_value, result_value, ret_val);
        return Status::SUCCESS;
    }

    Status test_multiply()
    {
        string simple_number = string("+1.0e2*-0.5");
        double expected_value = +1.0e2*-0.5;
        double result_value;
        Status ret_val;

        ret_val = parse_expression(simple_number, &result_value);
        log_test_result(simple_number, expected_value, result_value, ret_val);
        return Status::SUCCESS;
    }

    Status test_multiply_negative()
    {
        string simple_number = string("-1.0e2*-0.5");
        double expected_value = -1.0e2*-0.5;
        double result_value;
        Status ret_val;

        ret_val = parse_expression(simple_number, &result_value);
        log_test_result(simple_number, expected_value, result_value, ret_val);
        return Status::SUCCESS;
    }

    Status test_power()
    {
        string simple_number = string("4**3.5");
        double expected_value = pow(4.0, 3.5);
        double result_value;
        Status ret_val;

        ret_val = parse_expression(simple_number, &result_value);
        log_test_result(simple_number, expected_value, result_value, ret_val);
        return Status::SUCCESS;
    }

    Status run_tests()
    {
        print_test_log_header();
        test_simple_number();
        test_simple_negative_number();
        test_add_numbers();
        test_add_negative_numbers();
        test_divide();
        test_divide_negative_numbers();
        test_multiply();
        test_multiply_negative();
        return test_power();
    }
}
