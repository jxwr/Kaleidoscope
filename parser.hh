#pragma once

#include "ast.hh"
#include "skipper.hh"

namespace parser {

    template <typename Iterator>
    struct expression : qi::grammar<Iterator, ast::expression(), skipper<Iterator>> {
        expression() : expression::base_type(expr) {

            logical_or_op.add
                ("||", ast::op_or)
                ;

            logical_and_op.add
                ("&&", ast::op_and)
                ;

            equality_op.add
                ("==", ast::op_equal)
                ("!=", ast::op_not_equal)
                ;

            relational_op.add
                ("<", ast::op_less)
                ("<=", ast::op_less_equal)
                (">", ast::op_greater)
                (">=", ast::op_greater_equal)
                ;

            additive_op.add
                ("+", ast::op_plus)
                ("-", ast::op_minus)
                ;

            multiplicative_op.add
                ("*", ast::op_times)
                ("/", ast::op_divide)
                ;

            unary_op.add
                ("+", ast::op_positive)
                ("-", ast::op_negative)
                ("!", ast::op_not)
                ;

            keywords.add
                ("true")
                ("false")
                ("if")
                ("else")
                ("while")
                ("int")
                ("void")
                ("return")
                ;

            expr =
                logical_or_expr.alias()
                ;

            logical_or_expr =
                   logical_and_expr
                >> *(logical_or_op > logical_and_expr)
                ;

            logical_and_expr =
                   equality_expr
                >> *(logical_and_op > equality_expr)
                ;

            equality_expr =
                   relational_expr
                >> *(equality_op > relational_expr)
                ;
            
            relational_expr =
                   additive_expr
                >> *(relational_op > additive_expr)
                ;

            additive_expr =
                   multiplicative_expr
                >> *(additive_op > multiplicative_expr)
                ;

            multiplicative_expr =
                   unary_expr
                >> *(multiplicative_op > unary_expr)
                ;

            unary_expr =
                  primary_expr
                | (unary_op > unary_expr)
                ;

            primary_expr =
                  qi::uint_
                | function_call
                | identifier
                | qi::bool_
                | '(' > expr > ')'
                ;

            function_call =
                  (identifier >> '(')
                > argument_list
                > ')'
                ;

            argument_list = -(expr % ',');
            
            identifier =
                   !qi::lexeme[keywords >> !(qi::alnum | '_')]
                >> qi::raw[qi::lexeme[(qi::alpha | '_') >> *(qi::alnum | '_')]]
                ;
        }

        qi::rule<Iterator, ast::expression(), skipper<Iterator>>
            expr, equality_expr, relational_expr,
            logical_or_expr, logical_and_expr,
            additive_expr, multiplicative_expr
            ;

        qi::rule<Iterator, ast::operand(), skipper<Iterator> >
            unary_expr, primary_expr
            ;

        qi::rule<Iterator, ast::function_call(), skipper<Iterator> >
            function_call
            ;

        qi::rule<Iterator, std::list<ast::expression>(), skipper<Iterator> >
            argument_list
            ;

        qi::rule<Iterator, ast::identifier(), skipper<Iterator>> 
            identifier
            ;

        qi::symbols<char, ast::optoken>
            logical_or_op, logical_and_op,
            equality_op, relational_op,
            additive_op, multiplicative_op, unary_op
            ;

        qi::symbols<char>
            keywords
            ;
    };

}
