#include <config.h>
#include <iostream>
#include <string>

Config::Config(const int argc, char** argv) {
    ArgumentParser parser(kOptions);

    parser.parse(argc, argv);

    int opt;
    while ((opt = parser.next()) != EOF) {
        switch (opt) {
        case 'c':
            mode = kCreate;
            break;
        case 'x':
            mode = kExtract;
            break;
        case 'a':
            mode = kAppend;
            break;
        case 'd':
            mode = kDelete;
            break;
        case 'A':
            mode = kConcat;
            break;
        case 'l':
            mode = kList;
            break;
        case 'f':
            path = fs::absolute(parser.value().first);
            break;
        case 'h':
            help(std::cout);
            exit(0);
        case 'e':
            word_size = std::stoi(parser.value().first);
            word_size += word_size % 8;
            break;
        case '!':
            args.push_back(parser.value().first);
            break;
        case '?':
            undef(std::cout, parser.value().first);
            exit(0);
        default:
            throw std::runtime_error("Config::Config error: Could not resolve arguments.");
        }
    }
}

void Config::help(std::ostream& out) {
    out << "HamArc v1.0\n"
        << "Modes:\n"
        << "\t-c (--create) 1+       : create new archive (default).\n"
        << "\t-x (--extract) 0+      : extract all or by a wildcard.\n"
        << "\t-a (--append) 1+       : append new file(s) to archive.\n"
        << "\t-d (--delete) 1+       : delete one or more files.\n"
        << "\t-A (--concatenate) 1+  : concatenate one or more archives.\n"
        << "\t-l (--list) 0/1        : list all files or wildcard filter.\n"
        << "Options:\n"
        << "\t-f (--file) [PATH]     : Archive to operate on.\n"
        << "\t-e (--word-size) [INT] : Encoder word size option."
        << std::endl;
}

void Config::undef(std::ostream& out, const std::string& opt) {
    out << "error: unsupported option: '" << opt << '\'' << std::endl;
}
