#include <getopt.h>

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>

const size_t PROPS_SIZE = 4;

inline void print_help(const std::string& path) {
    std::cout << "WordCount 1.2\n"
              << "Usage: " << path << " [OPTIONS]... [FILE]...\n"
              << "Print countable PROPERTIES for each FILE.\n"
              << "Print overall line if more than one FILE is specified.\n\n"
              << "If no FILE was specified or '-' was passed, will read standart input.\n\n"
              << "PROPERTIES always go in the following order:\n"
              << "\tnewline, word, byte, character\n\n"
              << "OPTIONS select PROPERTIES that you want to print:\n"
              << "\t-h (--help):          You're already here!\n"
              << "\t-l (--lines):         Print the amount of newlines.\n"
              << "\t-w (--words):         Print the amount of words.\n"
              << "\t-c (--bytes):         Print the file value in bytes.\n"
              << "\t-m (--chars):         Print the amount of symbols.\n"
              << "\t-o (--output) [FILE]: Print in FILE instead of a console."
              << std::endl;
}

/*
    argv[0] - path to executable
*/
void args_parser(
    int argc,
    char** argv,
    std::set<int>& opts,
    std::vector<std::string>& paths,
    std::string output) {
    while (true) {
        int option_index = 0;
        struct option long_options[] = {
            {"lines", no_argument, 0, 'l'},
            {"words", no_argument, 0, 'w'},
            {"bytes", no_argument, 0, 'c'},
            {"chars", no_argument, 0, 'm'},
            {"help", no_argument, 0, 'h'},
            {"output", required_argument, 0, 'o'},
            {0, 0, 0, 0}};

        char c = getopt_long(argc, argv, "lwmcho:", long_options, &option_index);

        if (c == EOF) {
            break;
        }

        switch (c) {
            case 'l':
                opts.insert(0);
                break;
            case 'w':
                opts.insert(1);
                break;
            case 'c':
                opts.insert(2);
                break;
            case 'm':
                opts.insert(3);
                break;
            case 'h':
                print_help(argv[0]);
                exit(0);
                break;
            case 'o':
                output = optarg;
                break;
            case '?':
                std::cout << "Try: '" << argv[0] << " -h' or '" << argv[0] << " --help' to learn about this utility." << std::endl;
                exit(0);
            default:
                break;
        }
    }

    while (optind < argc) {
        paths.push_back(argv[optind++]);
    }

    /*
        If no options persists will opt in:
            -l, -w, -c
    */
    if (opts.empty()) {
        for (int i = 0; i < 3; i++) {
            opts.insert(i);
        }
    }

    /*
        If no paths then read from standart input.
    */
    if (paths.empty()) {
        paths.push_back("-");
    }
}

std::array<uintmax_t, PROPS_SIZE> count_properties(std::istream& stream) {
    /*
        result[0]: lines,
        result[1]: words,
        result[2]: bytes,
        result[3]: characters
    */
    std::array<uintmax_t, PROPS_SIZE> result = {0, 0, 0, 0};

    char buffer;
    bool whitespace = true;

    while (stream.get(buffer)) {
        if (buffer == '\n') {
            result[0]++;
        }

        if (!isspace(buffer) && whitespace) {
            result[1]++;
        }

        whitespace = isspace(buffer);

        /*
            Non leading utf-8 characters will not be included in a character count,
            as their 2 leading bits is equal "10"
        */
        result[3] += ((buffer & 0xc0) != 0x80);

        result[2]++;
    }

    return result;
}

uintmax_t base10_size(uintmax_t x) {
    uintmax_t result = 0;

    do {
        x /= 10;
        result++;
    } while (x != 0);

    return result;
}

int main(int argc, char** argv) {
    std::vector<std::string> paths;
    std::set<int> opts;
    std::string output;

    args_parser(argc, argv, opts, paths, output);

    if (!output.empty()) {
        /*
            Change standart output stream with file if there any...
        */
        FILE* fstdout = freopen(output.c_str(), "w", stdout);

        if (!fstdout) {
            std::cerr << "Error: Cannot print to " << output << ". Check permissions or try another one..." << std::endl;
            return 0;
        }
    }

    std::array<uintmax_t, PROPS_SIZE> all = {0, 0, 0, 0};

    std::vector<std::array<uintmax_t, PROPS_SIZE>> counts;

    for (const std::string& path : paths) {
        if (path == "-") {
            counts.push_back(count_properties(std::cin));
        } else {
            std::ifstream file(path, std::ifstream::in);

            if (!file.is_open()) {
                std::cerr << "Error: accessing file at " << path << '\n';
                continue;
            }

            counts.push_back(count_properties(file));
        }

        for (size_t i = 0; i < counts.back().size(); i++) {
            all[i] += counts.back()[i];
        }
    }

    /*
        Pass ' ' character since user may end input without escape character.
    */

    for (int i = 0; i < counts.size(); i++) {
        for (const int& opt : opts) {
            /*
                Subtract largest count size with current to determine allign margin.
            */
            std::cout << ' ' << std::string(base10_size(all[opt]) - base10_size(counts[i][opt]), ' ')
                      << counts[i][opt];
        }
        std::cout << ' ' << paths[i] << std::endl;
    }

    if (paths.size() > 1) {
        for (const int& opt : opts) {
            std::cout << ' ' << all[opt];
        }
        std::cout << " overall" << std::endl;
    }

    return 0;
}
