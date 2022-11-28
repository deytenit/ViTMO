#pragma once

#include <filesystem>
#include <vector>
#include <string>

#include "argument_parser.h"

namespace fs = std::filesystem;

class Config {
public:
    const std::list<ArgumentParser::Option> kOptions = {
        {"create", 'c', ArgumentParser::kFlag, 'c'},
        {"extract", 'x', ArgumentParser::kFlag, 'x'},
        {"append", 'a', ArgumentParser::kFlag, 'a'},
        {"delete", 'd', ArgumentParser::kFlag, 'd'},
        {"concatenate", 'A', ArgumentParser::kFlag, 'A'},
        {"list", 'l', ArgumentParser::kFlag, 'l'},
        {"file", 'f', ArgumentParser::kStore, 'f'},
        {"word-size", 'e', ArgumentParser::kStore, 'e'},
        {"help", 'h', ArgumentParser::kFlag, 'h'}
    };

    enum WorkingMode {
        kCreate,
        kExtract,
        kAppend,
        kDelete,
        kConcat,
        kList
    };

    WorkingMode mode = kCreate;
    uint32_t word_size = 8;
    fs::path path = fs::current_path();
    std::vector<std::string> args;

    Config(int argc, char** argv);

private:
    static void help(std::ostream& out);

    static void undef(std::ostream& out, const std::string& opt);
};
