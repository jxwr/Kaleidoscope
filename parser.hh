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

        qi::rule<Iterator, ast::operand(), skipper<Iterator>>
            unary_expr, primary_expr
            ;

        qi::rule<Iterator, ast::function_call(), skipper<Iterator>>
            function_call
            ;

        qi::rule<Iterator, std::list<ast::expression>(), skipper<Iterator>>
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

    template <typename Iterator>
    struct statement : qi::grammar<Iterator, ast::statement_list(), skipper<Iterator>> {
        statement() : statement::base_type(statement_list) {
            statement_list =
                +statement_
                ;

            statement_ =
                  variable_declaration
                | assignment
                | compound_statement
                | if_statement
                | while_statement
                | return_statement
                | expression_statement
                ;

            identifier =
                   !expr.keywords
                >> qi::raw[qi::lexeme[(qi::alpha | '_') >> *(qi::alnum | '_')]]
                ;

            variable_declaration =
                  qi::lexeme["int" >> !(qi::alnum | '_')]
                > identifier
                > -('=' > expr)
                > ';'
                ;

            assignment =
                  identifier
                > '='
                > expr
                > ';'
                ;

            expression_statement = 
                   -expr
                >> ';'
                ;

            if_statement =
                  qi::lit("if")
                > '('
                > expr
                > ')'
                > statement_
                >
                -(
                    qi::lexeme["else" >> !(qi::alnum | '_')]
                  > statement_
                  )
                ;

            while_statement =
                  qi::lit("while")
                > '('
                > expr
                > ')'
                > statement_
                ;

            compound_statement =
                '{' >> -statement_list >> '}'
                ;

            return_statement =
                  qi::lexeme["return" >> !(qi::alnum | '_')]
                > -expr
                > ';'
                ;
        }

        expression<Iterator> expr;

        qi::rule<Iterator, ast::statement_list(), skipper<Iterator>>
            statement_list, compound_statement
            ;

        qi::rule<Iterator, ast::statement(), skipper<Iterator>> 
            statement_
            ;

        qi::rule<Iterator, ast::variable_declaration(), skipper<Iterator>> 
            variable_declaration
            ;

        qi::rule<Iterator, ast::assignment(), skipper<Iterator>> 
            assignment
            ;

        qi::rule<Iterator, ast::if_statement(), skipper<Iterator>> 
            if_statement
            ;

        qi::rule<Iterator, ast::while_statement(), skipper<Iterator>> 
            while_statement
            ;

        qi::rule<Iterator, ast::return_statement(), skipper<Iterator>> 
            return_statement
            ;

        qi::rule<Iterator, ast::expression_statement(), skipper<Iterator>> 
            expression_statement
            ;

        qi::rule<Iterator, std::string(), skipper<Iterator>> 
            identifier
            ;
    };

    template <typename Iterator>
    struct function : qi::grammar<Iterator, ast::function(), skipper<Iterator>> {
        function() : function::base_type(start) {
            identifier =
                   !body.expr.keywords
                >> qi::raw[qi::lexeme[(qi::alpha | '_') >> *(qi::alnum | '_')]]
                ;

            argument_list = 
                -(identifier % ',')
                ;

            start =
                  qi::lexeme[(qi::string("void") | qi::string("int")) 
                             >> !(qi::alnum | '_')]
                > identifier
                > '(' > argument_list > ')'
                > '{' > body > '}'
                ;
        }

        statement<Iterator> body;

        qi::rule<Iterator, ast::identifier(), skipper<Iterator>> 
            identifier
            ;

        qi::rule<Iterator, std::list<ast::identifier>(), skipper<Iterator>> 
            argument_list
            ;

        qi::rule<Iterator, ast::function(), skipper<Iterator>>
            start
            ;
    };

}
