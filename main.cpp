#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <iostream>
#include <string>

#include "helper.h"
#include "ast.h"
#include "grammar.h"
#include "visitor.h"

namespace spirit = boost::spirit;
namespace qi = spirit::qi;

int main()
{
    std::cout << "Type [q or Q] to quit\n\n";

    using boost::spirit::ascii::space;

    typedef std::string::const_iterator iterator_type;
    typedef calculator<iterator_type> calculator;

    calculator calc; // Our grammar

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        expression_ast ast;
        ast_printer printer;
        bool r = phrase_parse(iter, end, calc, space, ast);

        if (r && iter == end) {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            printer(ast);
            std::cout << "\n-------------------------\n";
        }
        else {
            std::string rest(iter, end);
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "stopped at: \": " << rest << "\"\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}
