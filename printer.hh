#pragma once

#include <boost/variant/apply_visitor.hpp>

#include <iostream>

#include "ast.hh"

namespace visitor {

    struct printer {
        typedef bool result_type;

        bool operator()(const ast::nil x) {
            std::cout << "nil";
            return true;
        }

        bool operator()(unsigned int x) {
            std::cout << x;
            return true;
        }

        bool operator()(bool x) {
            std::cout << x;
            return true;
        }

        bool operator()(const ast::identifier& x) {
            std::cout << x;
            return true;
        }

        bool operator()(const ast::operation& x) {
            switch (x.operator_) {
            case ast::op_plus: 
                std::cout << "+"; break;
            case ast::op_minus: 
                std::cout << "-"; break;
            case ast::op_times: 
                std::cout << "*"; break;
            case ast::op_divide: 
                std::cout << "/"; break;
            case ast::op_equal: 
                std::cout << "=="; break;
            case ast::op_not_equal: 
                std::cout << "!="; break;
            case ast::op_less:
                std::cout << "<"; break;
            case ast::op_less_equal:
                std::cout << "<="; break;
            case ast::op_greater:
                std::cout << ">"; break;
            case ast::op_greater_equal:
                std::cout << ">="; break;
            case ast::op_and:
                std::cout << "&&"; break;
            case ast::op_or:
                std::cout << "||"; break;
            default: 
                return false;
            }
            std::cout << '(';
            if (!boost::apply_visitor(*this, x.operand_))
                return false;
            std::cout << ')';
            return true;
        }

        bool operator()(const ast::unary& x) {
            switch (x.operator_) {
            case ast::op_negative: 
                std::cout << '-'; break;
            case ast::op_not: 
                std::cout << '!'; break;
            case ast::op_positive: 
                std::cout << '+'; break;
            default:
                return false;
            }
            if (!boost::apply_visitor(*this, x.operand_))
                return false;
            return true;
        }

        bool operator()(const ast::function_call& x) {
            std::cout << x.function_name << '(';
            for (auto& arg : x.args) {
                (*this)(arg);
                std::cout << ',';
            }
            std::cout << ')';
            return true;
        }

        bool operator()(const ast::expression& x) {
            if (!boost::apply_visitor(*this, x.first))
                return false;
            for(auto& oper : x.rest) {
                if (!(*this)(oper))
                    return false;
            }
            return true;
        }

        bool operator()(const ast::assignment& x) {
            std::cout << x.lhs << "=";
            (*this)(x.rhs);
            return true;
        }

        bool operator()(const ast::variable_declaration& x) {
            std::cout << x.lhs << "=";
            return true;
        }

        bool operator()(const ast::statement& x) {
            boost::apply_visitor(*this, x);
        }

        bool operator()(const ast::statement_list& x) {
            for (const auto& stmt : x) {
                (*this)(stmt);
                std::cout << ';' << std::endl;
            }
        }

        bool operator()(const ast::expression_statement& x) {
            (*this)(x.expr);
        }

        bool operator()(const ast::if_statement& x) {
        }

        bool operator()(const ast::while_statement& x) {
        }

        bool operator()(const ast::function& x) {
        }

        bool operator()(const ast::return_statement& x) {
        }

    };

}
