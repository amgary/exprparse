// exprparse.cpp
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

#include "exprparse.h"
#include <cstddef>
#include <cstring>
#include <list>
#include <math.h>
#include <queue>
#include <regex>
#include <sstream>
#include <stack>

using namespace std;

#define EXPRPARSE_VERSION_MAJOR 0
#define EXPRPARSE_VERSION_MINOR 1
#define EXPRPARSE_VERSION_PATCH 0

namespace exprparse {
    // Tolerance for determining if number is close to zero
    const double ALMOST_ZERO = 1.0E-10;

    typedef enum TokenType { NUMBER, OPERATOR, FUNCTION, LEFT_BRACKET, RIGHT_BRACKET } TokenType;


    typedef struct TokenRegex {
        TokenType ttype;
        regex regexpr;
        void* data;
    } TokenRegex;

    typedef Status (*Operation)(const double[], const size_t&, double*);

    typedef enum OperatorAssoc { RIGHT, LEFT } OperatorAssoc;

    typedef struct Operator {
        Operation eval;
        uint16_t precedance;
        size_t num_arg;
        OperatorAssoc op_assoc;
    } Operator;

    // Typedefs for parsed token data
    typedef struct Token {
        TokenType ttype;
        void* data;
    } Token;

    typedef struct OperatorData {
        Operator* op;
    } OperatorData;

    typedef struct NumberData {
        double number;
    } NumberData;

    // Declare all operations
    Status add(const double args[], const size_t& num_args, double* result);
    Status subtract(const double args[], const size_t& num_args, double* result);
    Status multiply(const double args[], const size_t& num_args, double* result);
    Status divide(const double args[], const size_t& num_args, double* result);
    Status power(const double args[], const size_t& num_args, double* result);
    Status unary_minus(const double args[], const size_t& num_args, double* result);
    Status unary_plus(const double args[], const size_t& num_args, double* result);

    // Declare helper functions
    void destroy_tokens(list<Token*>& tokens);

    Operator g_add_op = { add, 1, 2, OperatorAssoc::LEFT };
    Operator g_sub_op = { subtract, 1, 2, OperatorAssoc::LEFT };
    Operator g_mult_op = { multiply, 2, 2, OperatorAssoc::LEFT };
    Operator g_divide_op = { divide, 2, 2, OperatorAssoc::LEFT };
    Operator g_power_op = { power, 3, 2, OperatorAssoc::RIGHT };
    Operator g_unary_minus = { unary_minus, 3, 1, OperatorAssoc::RIGHT };
    Operator g_unary_plus = { unary_plus, 3, 1, OperatorAssoc::RIGHT };

    // This could be put in an initialization function to avoid hardcoding
    const size_t MAX_OPERATOR_ARGS = 2;

    TokenRegex g_tokens_reg[] = { { NUMBER, regex(R"(^([0-9]+\.?|\.[0-9]+)[0-9]*([eE][+-]?[0-9]+)?)"), NULL },
                                  { OPERATOR, regex(R"(^\*\*)"), &g_power_op },
                                  { OPERATOR, regex(R"(^\^)"), &g_power_op },
                                  { OPERATOR, regex(R"(^\*)"), &g_mult_op },
                                  { OPERATOR, regex(R"(^/)"), &g_divide_op },
                                  { OPERATOR, regex(R"(^\+)"), &g_add_op },
                                  { OPERATOR, regex(R"(^\-)"), &g_sub_op },
                                  { LEFT_BRACKET, regex(R"(^\()"), NULL },
                                  { LEFT_BRACKET, regex(R"(^\[)"), NULL },
                                  { RIGHT_BRACKET, regex(R"a(^\))a"), NULL },
                                  { RIGHT_BRACKET, regex(R"(^\])"), NULL } };

    // Function to skip whitespace characters in input string
    // This function will offset the input character pointer such
    // that it points to first character that is not whitespace
    //
    // Returns: 1 if whitespace successfully skipped, 0 if end of string was hit
    short skip_whitespace(string::const_iterator& str_itr, const string::const_iterator& end_iter) {
        while (str_itr != end_iter) {
            if (*str_itr == ' ' || *str_itr == '\t' || *str_itr == '\r' || *str_itr == '\f' || *str_itr == '\n') {
                str_itr++;
            } else {
                return 1;
            }
        }
        return 0;
    }

    // Function to convert a string expression into a list of tokens
    //
    // Caller must call destroy_tokens to clean up list when done with the tokens
    Status tokenize_expr(const string& expression, list<Token*>& tokens) {
        // Check for empty expression
        if (expression.empty()) return Status::EMPTY_EXPRESSION;

        // Initialize string iterator
        string::const_iterator expr_iter = expression.begin();

        // Clear out tokens
        tokens.clear();

        // Enter the parsing loop
        Status ret_value = Status::SUCCESS;
        smatch matches;
        while (skip_whitespace(expr_iter, expression.end()) && ret_value == Status::SUCCESS) {
            // Try to match each possible token
            bool match_found = false;
            for (size_t itok = 0; itok < sizeof(g_tokens_reg) / sizeof(g_tokens_reg[0]); itok++) {
                TokenRegex* tok_reg = &g_tokens_reg[itok];
                if (regex_search(expr_iter, expression.end(), matches, tok_reg->regexpr)) {
                    expr_iter += matches[0].length();
                    Token* tok = new Token;
                    tok->ttype = tok_reg->ttype;
                    if (tok->ttype == TokenType::NUMBER) {
                        NumberData* num = new NumberData;
                        num->number = atof(matches[0].str().c_str());
                        tok->data = num;
                    } else if (tok->ttype == TokenType::OPERATOR) {
                        bool isUnary = tokens.empty() || (tokens.back()->ttype != TokenType::NUMBER &&
                                                          tokens.back()->ttype != TokenType::RIGHT_BRACKET);
                        OperatorData* data = new OperatorData;
                        if (!isUnary || (tok_reg->data != &g_sub_op && tok_reg->data != &g_add_op))
                            data->op = (Operator*)tok_reg->data;
                        else {
                            if (tok_reg->data == &g_sub_op)
                                data->op = &g_unary_minus;
                            else
                                data->op = &g_unary_plus;
                        }
                        tok->data = data;
                    } else {
                        tok->data = tok_reg->data;
                    }
                    tokens.push_back(tok);
                    match_found = true;
                    break;
                }
            }
            if (!match_found) ret_value = Status::UNKNOWN_TOKEN;
        }

        // Return
        return ret_value;
    }

    // Parses list of tokens into reverse polish notation
    //
    // Arguments:
    //  tokens: List of tokens using infix notation
    //  rpn_tokens: stack containing tokens in reverse polish notation
    Status convert_tokens_to_rpn(const list<Token*>& tokens, queue<Token*>& rpn_tokens) {
        stack<Token*> operator_stack;

        for (auto iter = tokens.begin(); iter != tokens.end(); iter++) {
            Token* tok = *iter;
            if (tok->ttype == TokenType::NUMBER) {
                rpn_tokens.push(tok);
            } else if (tok->ttype == TokenType::FUNCTION || tok->ttype == TokenType::LEFT_BRACKET) {
                operator_stack.push(tok);
            } else if (tok->ttype == TokenType::OPERATOR) {
                OperatorData* op_data_current = (OperatorData*)tok->data;
                while (!operator_stack.empty() && operator_stack.top()->ttype != TokenType::LEFT_BRACKET) {
                    OperatorData* op_data_top = NULL;
                    if (operator_stack.top()->ttype == OPERATOR)
                        op_data_top = (OperatorData*)operator_stack.top()->data;

                    if (operator_stack.top()->ttype == TokenType::FUNCTION ||
                        op_data_current->op->precedance < op_data_top->op->precedance ||
                        (op_data_current->op->precedance == op_data_top->op->precedance &&
                         op_data_top->op->op_assoc == OperatorAssoc::LEFT)) {
                        rpn_tokens.push(operator_stack.top());
                        operator_stack.pop();
                    } else {
                        break;
                    }
                }
                operator_stack.push(tok);
            } else if (tok->ttype == TokenType::RIGHT_BRACKET) {
                while (!operator_stack.empty() && operator_stack.top()->ttype != TokenType::LEFT_BRACKET) {
                    rpn_tokens.push(operator_stack.top());
                    operator_stack.pop();
                }
                if (operator_stack.empty()) {
                    return Status::UNMATCHED_BRACKETS;
                }
                // Remove the left bracket from the stack
                operator_stack.pop();
            }
        }

        // No more tokens, remove all remaining operators
        while (!operator_stack.empty()) {
            Token* tok = operator_stack.top();
            operator_stack.pop();
            if (tok->ttype == TokenType::LEFT_BRACKET || tok->ttype == TokenType::RIGHT_BRACKET) {
                return Status::UNMATCHED_BRACKETS;
            }
            rpn_tokens.push(tok);
        }

        return Status::SUCCESS;
    }

    // Function to evaluate a queue of tokens in reverse polish notation
    //
    // Arguments:
    //  rpn_tokens: queue of tokens in reverse polish notation, will be modified
    //  result: double to store result of calculation
    Status eval_rpn_tokens(queue<Token*>& rpn_tokens, double* result) {
        stack<double> argument_stack;
        double* args = new double[MAX_OPERATOR_ARGS];
        *result = 0.0;
        Status ret_val = Status::SUCCESS;

        while (!rpn_tokens.empty() && ret_val == Status::SUCCESS) {
            Token* tok = rpn_tokens.front();
            rpn_tokens.pop();

            if (tok->ttype == TokenType::NUMBER) {
                argument_stack.push(((NumberData*)tok->data)->number);
            } else if (tok->ttype == TokenType::OPERATOR) {
                Operator* op = ((OperatorData*)tok->data)->op;
                for (int iarg = (int)op->num_arg - 1; iarg >= 0; iarg--) {
                    if (argument_stack.empty()) {
                        return Status::TOO_FEW_ARGUMENTS;
                    }
                    args[iarg] = argument_stack.top();
                    argument_stack.pop();
                }
                if (ret_val == Status::SUCCESS) {
                    double eval_result;
                    ret_val = op->eval(args, op->num_arg, &eval_result);
                    argument_stack.push(eval_result);
                }
            } else {
                ret_val = Status::UNKNOWN_TOKEN;
            }
        }

        if (ret_val == Status::SUCCESS) {
            if (argument_stack.size() == 1) {
                *result = argument_stack.top();
            } else if (argument_stack.size() > 1) {
                ret_val = Status::TOO_MANY_ARGUMENTS;
            } else {
                ret_val = Status::TOO_FEW_ARGUMENTS;
            }
        }

        delete[] args;
        return ret_val;
    }

    Status parse_expression(const string& expression, double* result) {
        list<Token*> tokens;
        Status ret_val;
        ret_val = tokenize_expr(expression, tokens);

        // Now that the tokens exist, parse into reverse polish notation
        queue<Token*> output_stack;
        if (ret_val == Status::SUCCESS) {
            ret_val = convert_tokens_to_rpn(tokens, output_stack);
        }

        // Now evaluate the reverse polish tokens
        if (ret_val == Status::SUCCESS) {
            ret_val = eval_rpn_tokens(output_stack, result);
        }

        // Done with tokens, clean them up
        destroy_tokens(tokens);
        return ret_val;
    }

    std::string get_status_string(const Status& status) {
        switch (status) {
        case Status::SUCCESS:
            return string("Success");
        case Status::ERROR:
            return string("Error");
        case Status::EMPTY_EXPRESSION:
            return string("Empty input expression");
        case Status::DIVIDE_BY_ZERO:
            return string("Divide by zero");
        case Status::UNKNOWN_TOKEN:
            return string("Unreckonized token");
        case Status::UNMATCHED_BRACKETS:
            return string("Brackets not matched");
        case Status::TOO_FEW_ARGUMENTS:
            return string("Not enough arguments found for operator");
        case Status::TOO_MANY_ARGUMENTS:
            return string("Too many arguments found for operations");
        default:
            return string("Unknown Status");
        }
    }

    std::string get_version() {
        ostringstream o;
        o << EXPRPARSE_VERSION_MAJOR << "." << EXPRPARSE_VERSION_MINOR;
        o << "." << EXPRPARSE_VERSION_PATCH;
        return o.str();
    }

    // Method to free all tokens in list
    void destroy_tokens(list<Token*>& tokens) {
        Token* tok;
        while (tokens.size() > 0) {
            tok = tokens.front();
            tokens.pop_front();

            if (tok->ttype == TokenType::NUMBER && tok->data != NULL) {
                delete static_cast<NumberData*>(tok->data);
            } else if (tok->ttype == TokenType::OPERATOR && tok->data != NULL) {
                delete static_cast<OperatorData*>(tok->data);
            }

            delete tok;
        }
    }

    //********************* Define all operations *****************************//

    // Addition operator
    Status add(const double args[], const size_t& num_args, double* result) {
        if (num_args != 2) return Status::ERROR;
        *result = args[0] + args[1];
        return Status::SUCCESS;
    }

    // Subtraction operator
    Status subtract(const double args[], const size_t& num_args, double* result) {
        if (num_args != 2) return Status::ERROR;
        *result = args[0] - args[1];
        return Status::SUCCESS;
    }

    // Multiplication Operation
    Status multiply(const double args[], const size_t& num_args, double* result) {
        if (num_args != 2) return Status::ERROR;
        *result = args[0] * args[1];
        return Status::SUCCESS;
    }

    // Define division operator
    Status divide(const double args[], const size_t& num_args, double* result) {
        if (num_args != 2) return Status::ERROR;
        if (fabs(args[1]) < ALMOST_ZERO) return Status::DIVIDE_BY_ZERO;
        *result = args[0] / args[1];
        return Status();
    }

    // Method to raise number to a power
    Status power(const double args[], const size_t& num_args, double* result) {
        if (num_args != 2) return Status::ERROR;
        *result = pow(args[0], args[1]);
        return Status::SUCCESS;
    }

    // Method to handle unary minus sign
    Status unary_minus(const double args[], const size_t& num_args, double* result) {
        if (num_args != 1) return Status::ERROR;
        *result = -1.0 * args[0];
        return Status::SUCCESS;
    }

    Status unary_plus(const double args[], const size_t& num_args, double* result) {
        if (num_args != 1) return Status::ERROR;
        *result = args[0];
        return Status::SUCCESS;
    }
} // namespace exprparse
