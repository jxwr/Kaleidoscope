#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

#include "ast.hh"

namespace ascii = spirit::ascii;
namespace qi = spirit::qi;

boost::phoenix::function<negate_expr> neg;

template <typename Iterator>
struct expression_grammar : qi::grammar<Iterator, expression_ast(), ascii::space_type> {
    expression_grammar() : expression_grammar::base_type(expression) {
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
    
    qi::rule<Iterator, expression_ast(), ascii::space_type> expression, term, factor;
};

template <typename Iterator>
struct program_grammar : qi::grammar<Iterator, program_ast(), ascii::space_type> {
    program_grammar() : program_grammar::base_type(program) {
        program %= expression;
    }

    qi::rule<Iterator, program_ast(), ascii::space_type> program;
    expression_grammar<Iterator> expression;
};
