#include <iostream>
#include <fstream>
#include <iterator>

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
    std::cout << source << std::endl;
    
    return 0;
}
