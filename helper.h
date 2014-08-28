#pragma once

#include <boost/spirit/include/qi.hpp>

#include <typeinfo>
#include <cxxabi.h>

namespace spirit = boost::spirit;

std::string demangle(const char* symbol) {
    size_t size;
    int status;
    char* demangled;

    if (NULL != (demangled = abi::__cxa_demangle(symbol, NULL, &size, &status))) {
        std::string result(demangled);
        free(demangled);
        return result;
    }
 
    return symbol;
}

template <typename Expr, typename Iterator = spirit::unused_type>
struct attribute_of_parser {
    typedef typename spirit::result_of::compile<
    spirit::qi::domain, Expr
        >::type parser_expression_type;

    typedef typename spirit::traits::attribute_of<
    parser_expression_type, spirit::unused_type, Iterator
        >::type type;
};

template <typename T>
void display_attribute_of_parser(T const&) {
    typedef typename attribute_of_parser<T>::type attribute_type;
    std::cout << demangle(typeid(attribute_type).name()) << std::endl;
}
