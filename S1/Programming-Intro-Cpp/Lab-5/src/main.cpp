#include "main.h"

using namespace argument_parser;

struct Options {
    bool sum = false;
    bool mult = false;
};

int main(const int argc, char** argv) {
    Options opt;
    std::vector<int> values;

    Parser parser("Program");
    parser.add_int_option("N").multiple(1).positional().store(values);
    parser.add_flag("sum", "add args").store(opt.sum);
    parser.add_flag("mult", "multiply args").store(opt.mult);
    parser.add_help('h', "help", "Program accumulate arguments");

    try {
        parser.parse(argc, argv);
    }
    catch (std::runtime_error& err) {
        std::cout << "Wrong argument" << std::endl;
        std::cout << err.what() << std::endl;
        return 1;
    }

    if (opt.sum) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
    } else if(opt.mult) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<>()) << std::endl;
    } else {
        std::cout << "No one options had chosen" << std::endl;
        return 1;
    }

    return 0;

}
