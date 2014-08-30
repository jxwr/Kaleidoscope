#pragma once

#include <boost/variant/recursive_variant.hpp>

struct binary_op;
struct unary_op;
struct nil {};

struct expression_ast {
    typedef
    boost::variant<
    nil // can't happen!
    , unsigned int
        , boost::recursive_wrapper<expression_ast>
        , boost::recursive_wrapper<binary_op>
        , boost::recursive_wrapper<unary_op>
        >
        type;

expression_ast()
: expr(nil()) {}

    template <typename Expr>
    expression_ast(Expr const& expr)
    : expr(expr) {}

    expression_ast& operator+=(expression_ast const& rhs);
    expression_ast& operator-=(expression_ast const& rhs);
    expression_ast& operator*=(expression_ast const& rhs);
    expression_ast& operator/=(expression_ast const& rhs);

    type expr;
};

struct binary_op {
binary_op(
          char op
          , expression_ast const& left
          , expression_ast const& right)
: op(op), left(left), right(right) {}

    char op;
    expression_ast left;
    expression_ast right;
};

struct unary_op {
unary_op(
         char op
         , expression_ast const& subject)
: op(op), subject(subject) {}

    char op;
    expression_ast subject;
};

expression_ast& expression_ast::operator+=(expression_ast const& rhs) {
    expr = binary_op('+', expr, rhs);
    return *this;
}

expression_ast& expression_ast::operator-=(expression_ast const& rhs) {
    expr = binary_op('-', expr, rhs);
    return *this;
}

expression_ast& expression_ast::operator*=(expression_ast const& rhs) {
    expr = binary_op('*', expr, rhs);
    return *this;
}

expression_ast& expression_ast::operator/=(expression_ast const& rhs) {
    expr = binary_op('/', expr, rhs);
    return *this;
}

struct negate_expr {
    template <typename T>
    struct result { typedef T type; };

    expression_ast operator()(expression_ast const& expr) const {
        return expression_ast(unary_op('-', expr));
    }
};
