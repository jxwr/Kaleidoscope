#pragma once

#include <utility>
#include <string>
#include <vector>
#include <list>

#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/boost_tuple.hpp> 
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional.hpp>

namespace ast {

    struct nil {};
    struct unary;
    struct function_call;
    struct expression;

    typedef std::string identifier;

    typedef boost::variant<
            nil
          , bool
          , unsigned int
          , identifier
          , boost::recursive_wrapper<unary>
          , boost::recursive_wrapper<function_call>
          , boost::recursive_wrapper<expression>
        >
    operand;

    enum optoken {
        op_plus,
        op_minus,
        op_times,
        op_divide,
        op_positive,
        op_negative,
        op_not,
        op_equal,
        op_not_equal,
        op_less,
        op_less_equal,
        op_greater,
        op_greater_equal,
        op_and,
        op_or
    };

    struct unary {
        optoken operator_;
        operand operand_;
    };

    struct operation {
        optoken operator_;
        operand operand_;
    };

    struct function_call {
        identifier function_name;
        std::list<expression> args;
    };

    struct expression {
        operand first;
        std::list<operation> rest;
    };

    struct assignment {
        identifier lhs;
        expression rhs;
    };

    struct variable_declaration {
        identifier lhs;
        boost::optional<expression> rhs;
    };

    inline std::ostream& operator<<(std::ostream& out, nil) {
        out << "nil"; return out;
    }

    inline std::ostream& operator<<(std::ostream& out, const identifier& id) {
        out << id; return out;
    }

}

BOOST_FUSION_ADAPT_STRUCT(
    ast::unary,
    (ast::optoken, operator_)
    (ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::operation,
    (ast::optoken, operator_)
    (ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::function_call,
    (ast::identifier, function_name)
    (std::list<ast::expression>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::expression,
    (ast::operand, first)
    (std::list<ast::operation>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::variable_declaration,
    (ast::identifier, lhs)
    (boost::optional<ast::expression>, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::assignment,
    (ast::identifier, lhs)
    (ast::expression, rhs)
)
