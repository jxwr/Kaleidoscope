#include <iostream>
#include <fstream>
#include <iterator>

#include "parser.hh"
#include "printer.hh"

bool load_file(const char* fname, std::string& source) {
    std::ifstream in(fname, std::ios_base::in);
    if (!in) {
        std::cerr << "Error: Could not open input file: " << fname << std::endl;
        return false;
    }

    in.unsetf(std::ios::skipws);
    std::copy(std::istream_iterator<char>(in),
              std::istream_iterator<char>(),
              std::back_inserter(source));
    return true;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        std::cerr << "Error: No input file provided" << std::endl;
        return 1;
    }

    const char* fname = argv[1];
    std::string source;
    if (!load_file(fname, source)) return 1;

    std::cout << "======>" << fname << "<======" << std::endl;

    typedef std::string::const_iterator iterator_type;
    iterator_type iter = source.begin();
    iterator_type end = source.end();

    ast::statement_list ast;
    parser::statement<iterator_type> grammar;
    parser::skipper<iterator_type> skipper;

    bool success = phrase_parse(iter, end, grammar, skipper, ast);
    if (!success) {
        std::string rest(iter, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \"" << rest << "\"\n";
        std::cout << "-------------------------\n";
    } else {
        visitor::printer print;
        print(ast);
        std::cout << std::endl;
    }
    
    return 0;
}
