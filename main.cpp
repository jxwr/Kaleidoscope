#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace client
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    template <typename Iterator>
    bool parse_number_list(Iterator first, Iterator last) {
        using qi::double_;
        using qi::phrase_parse;
        using ascii::space;

        std::vector<double> num_list;
        auto print_action = [] (double const& val) { std::cout << "action: " << val << std::endl; };
        auto parser = double_[print_action] % ',';

        bool r = phrase_parse(first, last, parser, space, num_list);
        if (first != last)
            return false;

        for (auto num : num_list) {
            std::cout << num << ',';
        }
        return r;
    }
}

int main()
{
    std::cout << "Type [q or Q] to quit\n\n";

    std::string str;
    while (getline(std::cin, str)) {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        if (client::parse_number_list(str.begin(), str.end())) {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << str << " Parses OK: " << std::endl;
        } else {
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}
