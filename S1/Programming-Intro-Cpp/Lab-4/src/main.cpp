#include "main.h"

int main(const int argc, char** argv) {

    switch (const Config config(argc, argv); config.mode) {
    case Config::kCreate:
        interactions::create(config);
        break;
    case Config::kExtract:
        interactions::extract(config);
        break;
    case Config::kAppend:
        interactions::append(config);
        break;
    case Config::kDelete:
        interactions::remove(config);
        break;
    case Config::kConcat:
        interactions::concat(config);
        break;
    case Config::kList:
        interactions::list(config);
        break;
    }

    return 0;
}
