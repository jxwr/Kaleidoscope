#pragma once

#include <boost/spirit/include/qi.hpp>

#include <iostream>

namespace qi = spirit::qi;

struct ast_printer {
    typedef void result_type;

    void operator()(qi::info::nil) const {}
    void operator()(int n) const { std::cout << n; }

    void operator()(ast::expression const& ast) const {
        boost::apply_visitor(*this, ast.expr);
    }

    void operator()(ast::binary_op const& expr) const {
        std::cout << "op:" << expr.op << "(";
        boost::apply_visitor(*this, expr.left.expr);
        std::cout << ", ";
        boost::apply_visitor(*this, expr.right.expr);
        std::cout << ')';
    }

    void operator()(ast::unary_op const& expr) const {
        std::cout << "op:" << expr.op << "(";
        boost::apply_visitor(*this, expr.subject.expr);
        std::cout << ')';
    }

    void operator()(ast::program const& prog) const {
        std::cout << "program:\n";
        (*this)(prog.expr);
    }
};
