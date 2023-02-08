#include <omfl_parser.h>

#include <filesystem>

int main() {
    const auto section = omfl::Parse(
        std::filesystem::path{
            R"(C:\Users\Deyte\Source\Repos\labwork-6-unknowableshade\example\config.omfl)"
        }
    );
    Stringify(
        section, std::filesystem::path{
            R"(C:\Users\Deyte\Source\Repos\labwork-6-unknowableshade\example\config-re.omfl)"
        }
    );
    Jsonify(
        section, std::filesystem::path{
            R"(C:\Users\Deyte\Source\Repos\labwork-6-unknowableshade\example\config-re.json)"
        }, true
    );
    return 0;
}
