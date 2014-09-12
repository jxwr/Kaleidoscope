#pragma once

#include "ast.hh"
#include "skipper.hh"

namespace parser {

    template <typename Iterator>
    struct expression : qi::grammar<Iterator, ast::expression(), skipper<Iterator>> {
        expression() : expression::base_type(start) {
            start = 
                identifier
                ;

            identifier = 
                qi::raw[qi::lexeme[(qi::alpha | '_') >> *(qi::alnum | '_')]]
                ;
        }

        qi::rule<Iterator, ast::expression(), skipper<Iterator>> start;
        qi::rule<Iterator, ast::identifier(), skipper<Iterator>> identifier;
    };
}
