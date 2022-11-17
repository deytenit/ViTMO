#include "argument_parser.h"

argument_parser::argument_parser(std::list<option> options): options_(std::move(options)) {}

void argument_parser::parse(int argc, char** argv) {
    exec_ = argv[0];

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.size() > 2 && arg.substr(0, 2) == "--") {
            arg = arg.substr(2);
            auto it = find_option(arg);

            if (it->type == FLAG) {
                args_.emplace_back(arg);
            }
            else if (i + 1 == argc) {
                throw std::runtime_error("argument_parser::parse error: No option for argument provided.");
            }
            else {
                args_.emplace_back(arg);

                opts_.emplace_back(argv[++i]);
            }
        }
        else if (arg[0] == '-') {
            for (size_t j = 1; j < arg.size(); ++j) {
                auto it = find_option(arg[j]);

                if (it->type == FLAG) {
                    args_.emplace_back(std::string(1, arg[j]));
                }
                else if (j + 1 < arg.size() || i + 1 == argc) {
                    throw std::runtime_error("argument_parser::parse error: No option for argument provided.");
                }
                else {
                    args_.emplace_back(std::string(1, arg[j]));

                    opts_.emplace_back(argv[++i]);
                }
            }
        }
    }
}

int argument_parser::next() {
    if (args_pos_ == args_.size()) {
        return -1;
    }

    const auto it = args_[args_pos_].size() > 1 ? find_option(args_[args_pos_]) : find_option(args_[args_pos_][0]);

    ++args_pos_;

    if (it == options_.end()) {
        return '?';
    }

    if (it->type == STORE) {
        ++opts_pos_;
    }

    return it->value;
}

std::string argument_parser::value() const {
    return opts_pos_ > 0 ? opts_[opts_pos_ - 1] : std::string();
}