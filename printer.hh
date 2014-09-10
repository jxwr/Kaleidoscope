#pragma once

#include <boost/spirit/include/qi.hpp>

#include <iostream>

namespace qi = spirit::qi;

struct expression_printer {
    typedef void result_type;

    void operator()(qi::info::nil) const {}
    void operator()(int n) const { std::cout << n; }
    void operator()(std::string const& s) const { std::cout << s; }

    void operator()(ast::expression const& ast) const {
        boost::apply_visitor(*this, ast.expr);
    }

    void operator()(ast::assign const& expr) const {
        std::cout << expr.get<0>() << " = ";
        boost::apply_visitor(*this, expr.get<1>().expr);
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

    void operator()(ast::call const& call) const {
        std::string args = "";
        std::cout << call.get<0>() << '(';
        for (auto& arg : call.get<1>()) {
            expression_printer printer;
            printer(arg);
            std::cout << ',';
        }
        std::cout << ')';
    }

    void operator()(ast::ifexpr const& expr) const {
        expression_printer printer;
        std::cout << "(if ";
        printer(expr.get<0>());
        std::cout << " then ";
        printer(expr.get<1>());
        std::cout << " else ";
        printer(expr.get<2>());
        std::cout << ")";
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
