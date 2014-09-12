#pragma once

#include <utility>
#include <string>
#include <vector>

#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/boost_tuple.hpp> 

namespace ast {

    struct nil {};

    typedef std::string identifier;

    typedef boost::variant<
          nil
        , identifier
        >
    expression;

}

