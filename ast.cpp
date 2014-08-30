#include "ast.hh"

namespace ast {

expression& expression::operator+=(expression const& rhs) {
    expr = binary_op('+', expr, rhs);
    return *this;
}

expression& expression::operator-=(expression const& rhs) {
    expr = binary_op('-', expr, rhs);
    return *this;
}

expression& expression::operator*=(expression const& rhs) {
    expr = binary_op('*', expr, rhs);
    return *this;
}

expression& expression::operator/=(expression const& rhs) {
    expr = binary_op('/', expr, rhs);
    return *this;
}

}
