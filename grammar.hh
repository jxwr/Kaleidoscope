#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

#include "ast.hh"

namespace grammar {

namespace ascii = spirit::ascii;
namespace qi = spirit::qi;

boost::phoenix::function<ast::negate_expr> neg;

template <typename Iterator>
struct expression : qi::grammar<Iterator, ast::expression(), ascii::space_type> {
    expression() : expression::base_type(expr) {
        using qi::_val;
        using qi::_1;
        using qi::uint_;

        expr =
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
            |   '(' >> expr                 [_val = _1] >> ')'
            |   ('-' >> factor              [_val = neg(_1)])
            |   ('+' >> factor              [_val = _1])
            ;
    }
    
    qi::rule<Iterator, ast::expression(), ascii::space_type> expr, term, factor;
};

template <typename Iterator>
struct program : qi::grammar<Iterator, ast::program(), ascii::space_type> {
    program() : program::base_type(prog) {
        prog %= expr;
    }

    qi::rule<Iterator, ast::program(), ascii::space_type> prog;
    expression<Iterator> expr;
};

}
