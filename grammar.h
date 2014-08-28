#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

#include "ast.h"

namespace ascii = spirit::ascii;
namespace qi = spirit::qi;

boost::phoenix::function<negate_expr> neg;

template <typename Iterator>
struct calculator : qi::grammar<Iterator, expression_ast(), ascii::space_type> {
    calculator() : calculator::base_type(expression) {
        using qi::_val;
        using qi::_1;
        using qi::uint_;

        expression =
            term                            [_val = _1]
            >> *(   ('+' >> term            [_val += _1])
                |   ('-' >> term            [_val -= _1])
                )
            ;

        term =
            factor                          [_val = _1]
            >> *(   ('*' >> factor          [_val *= _1])
                |   ('/' >> factor          [_val /= _1])
                )
            ;

        factor =
            uint_                           [_val = _1]
            |   '(' >> expression           [_val = _1] >> ')'
            |   ('-' >> factor              [_val = neg(_1)])
            |   ('+' >> factor              [_val = _1])
            ;
        }

    qi::rule<Iterator, expression_ast(), ascii::space_type>
    expression, term, factor;
};
