#pragma once

#include <utility>
#include <string>
#include <vector>

#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/boost_tuple.hpp> 

namespace ast {

struct binary_op;
struct unary_op;
struct nil {};

struct expression;

typedef boost::tuple<
    std::string,
    std::vector<std::string>
    > prototype;

typedef boost::tuple<
    std::string,
    std::vector<expression>
    > call;

typedef boost::tuple<
    prototype, 
    expression
    > definition;

typedef boost::tuple<
    expression,
    expression,
    expression
    > ifexpr;

typedef boost::variant<
    nil,
    expression,
    definition
    > program;

struct expression {
    typedef
    boost::variant<
        nil,
        unsigned int,
        std::string,
        boost::recursive_wrapper<ifexpr>,
        boost::recursive_wrapper<expression>,
        boost::recursive_wrapper<binary_op>,
        boost::recursive_wrapper<unary_op>,
        boost::recursive_wrapper<call>
        > type;

    expression()
        : expr(nil()) {}

    template <typename Expr>
    expression(Expr const& expr)
        : expr(expr) {}

    expression& operator+=(expression const& rhs);
    expression& operator-=(expression const& rhs);
    expression& operator*=(expression const& rhs);
    expression& operator/=(expression const& rhs);

    type expr;
};

struct binary_op {
    binary_op(char op,
              expression const& left,
              expression const& right)
        : op(op), left(left), right(right) {}

    char op;
    expression left;
    expression right;
};

struct unary_op {
    unary_op(char op,
             expression const& subject)
        : op(op), subject(subject) {}

    char op;
    expression subject;
};

struct negate_expr {
    template <typename T>
    struct result { typedef T type; };

    expression operator()(expression const& expr) const {
        return expression(unary_op('-', expr));
    }
};

}
