#pragma once

#include <boost/spirit/include/qi.hpp>

#include <iostream>

namespace qi = spirit::qi;

struct expression_printer {
    typedef void result_type;

    void operator()(qi::info::nil) const {}
    void operator()(int n) const { std::cout << n; }

    void operator()(ast::expression const& ast) const {
        boost::apply_visitor(*this, ast.expr);
    }

    void operator()(ast::binary_op const& expr) const {
        std::cout << "(";
        boost::apply_visitor(*this, expr.left.expr);
        std::cout << expr.op;
        boost::apply_visitor(*this, expr.right.expr);
        std::cout << ')';
    }

    void operator()(ast::unary_op const& expr) const {
        std::cout << expr.op << "(";
        boost::apply_visitor(*this, expr.subject.expr);
        std::cout << ')';
    }
};

struct program_printer {
    typedef void result_type;

    void operator()(ast::expression const& expr) const {
        expression_printer printer;
        printer(expr);
    }

    void operator()(ast::definition const& def) const {
        std::string args = "";
        ast::prototype proto = def.get<0>();

        for (auto& arg : proto.get<1>()) {
            args += arg + ',';
        }
        
        std::cout << "def " 
                  << proto.get<0>() 
                  << '(' << args << ") ";

        expression_printer printer;
        printer(def.get<1>());
    }
};
