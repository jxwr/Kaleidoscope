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
struct prototype : qi::grammar<Iterator, ast::prototype(), ascii::space_type> {
    prototype() : prototype::base_type(proto) {
        ident %= qi::lexeme[qi::char_("a-zA-Z") >> *(qi::char_("a-zA-Z0-9_-"))];
        proto %= ident >> '(' >> (ident % ',') >> ')';
    }

    qi::rule<Iterator, ast::prototype(), ascii::space_type> proto;
    qi::rule<Iterator, std::string(), ascii::space_type> ident;
};

template <typename Iterator>
struct definition : qi::grammar<Iterator, ast::definition(), ascii::space_type> {
    definition() : definition::base_type(def) {
        def %= qi::lit("def") >> proto >> expr;
    }

    qi::rule<Iterator, ast::definition(), ascii::space_type> def;
    prototype<Iterator> proto;
    expression<Iterator> expr;
};

template <typename Iterator>
struct program : qi::grammar<Iterator, ast::program(), ascii::space_type> {
    program() : program::base_type(prog) {
        prog %= def | expr;
    }

    qi::rule<Iterator, ast::program(), ascii::space_type> prog;
    expression<Iterator> expr;
    definition<Iterator> def;
};

}
