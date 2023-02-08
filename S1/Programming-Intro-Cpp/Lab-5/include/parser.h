#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <optional>

#include <builders.h>
#include <exceptions.h>

namespace argument_parser {
    class Parser {
        std::string name_;
        mutable std::string path_;

        std::list<std::unique_ptr<Option>> options_;

        std::optional<SingleArgument<bool>> help_;
    public:
        explicit Parser(std::string name);

        void parse(int argc, char** argv) const;
        void parse(const std::vector<std::string>& args) const;

        // Help resolver.

        void add_help(const std::string& name);
        void add_help(const std::string& name, const std::string& info);
        void add_help(char shortcut, const std::string& name);
        void add_help(
            char shortcut,
            const std::string& name,
            const std::string& info
        );

        std::string get_help() const;

        // Flag resolvers.

        SingleArgumentBuilder<bool> add_flag(const std::string& name);
        SingleArgumentBuilder<bool> add_flag(const std::string& name, const std::string& info);
        SingleArgumentBuilder<bool> add_flag(char shortcut, const std::string& name);
        SingleArgumentBuilder<bool> add_flag(
            char shortcut,
            const std::string& name,
            const std::string& info
        );

        bool get_flag(const std::string& name) const;

        // Int resolvers.

        SingleArgumentBuilder<int> add_int_option(const std::string& name);
        SingleArgumentBuilder<int> add_int_option(const std::string& name, const std::string& info);
        SingleArgumentBuilder<int> add_int_option(char shortcut, const std::string& name);
        SingleArgumentBuilder<int> add_int_option(
            char shortcut,
            const std::string& name,
            const std::string& info
        );

        int get_int_option(const std::string& name) const;
        int get_int_option(const std::string& name, size_t idx) const;

        // String resolvers.

        SingleArgumentBuilder<std::string> add_string_option(const std::string& name);
        SingleArgumentBuilder<std::string> add_string_option(
            const std::string& name,
            const std::string& info
        );
        SingleArgumentBuilder<std::string> add_string_option(
            char shortcut,
            const std::string& name
        );
        SingleArgumentBuilder<std::string> add_string_option(
            char shortcut,
            const std::string& name,
            const std::string& info
        );

        const std::string& get_string_option(const std::string& name) const;
        const std::string& get_string_option(const std::string& name, size_t idx) const;

    private:
        [[nodiscard]] std::list<std::unique_ptr<Option>>::const_iterator find_option(const std::string& key) const;
        static [[nodiscard]] std::list<std::string>::const_iterator find_token(
            const std::list<std::string>& tokens, const Option& option);

        static std::list<std::string> tokenize(const std::vector<std::string>& args);
        static bool begins_with(const std::string& base, char pref);
        static bool begins_with(const std::string& base, const std::string& pref);
    };
}
