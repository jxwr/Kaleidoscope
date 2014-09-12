#pragma once

#include <boost/spirit/include/qi.hpp>

namespace parser {

    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    template <typename Iterator>
    struct skipper : qi::grammar<Iterator> {
        skipper() : skipper::base_type(start) {
            qi::char_type char_;
            ascii::space_type space;
      
            start =
                    space
                |   "/*" >> *(char_ - "*/") >> "*/"     // comments
                ;
        }

        qi::rule<Iterator> start;
    };

}
