#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

#include <typeinfo>
#include <cxxabi.h>
#include <iostream>
#include <string>
#include <vector>

namespace client
{
    namespace spirit = boost::spirit;
    namespace qi = spirit::qi;
    namespace ascii = spirit::ascii;

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

    boost::phoenix::function<negate_expr> neg;

    struct ast_print {
        typedef void result_type;

        void operator()(qi::info::nil) const {}
        void operator()(int n) const { std::cout << n; }

        void operator()(expression_ast const& ast) const {
            boost::apply_visitor(*this, ast.expr);
        }

        void operator()(binary_op const& expr) const {
            std::cout << "op:" << expr.op << "(";
            boost::apply_visitor(*this, expr.left.expr);
            std::cout << ", ";
            boost::apply_visitor(*this, expr.right.expr);
            std::cout << ')';
        }

        void operator()(unary_op const& expr) const {
            std::cout << "op:" << expr.op << "(";
            boost::apply_visitor(*this, expr.subject.expr);
            std::cout << ')';
        }
    };

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
}

int main()
{
    std::cout << "Type [q or Q] to quit\n\n";

    using boost::spirit::ascii::space;
    using client::expression_ast;
    using client::ast_print;

    typedef std::string::const_iterator iterator_type;
    typedef client::calculator<iterator_type> calculator;

    calculator calc; // Our grammar

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        expression_ast ast;
        ast_print printer;
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
