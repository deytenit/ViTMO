#pragma once
#include <options.h>

namespace argument_parser {
    class OptionBuilder {
    protected:
        std::unique_ptr<Option>& argument_;
        explicit OptionBuilder(std::unique_ptr<Option>& ptr);
    };

    template <class T>
    struct MultipleArgumentBuilder final : OptionBuilder {
        explicit MultipleArgumentBuilder(std::unique_ptr<Option>& ptr);

        MultipleArgumentBuilder<T>& store(std::vector<T>& store);
        MultipleArgumentBuilder<T>& positional();
    };

    template <class T>
    struct SingleArgumentBuilder final : OptionBuilder {
        explicit SingleArgumentBuilder(std::unique_ptr<Option>& ptr);

        SingleArgumentBuilder<T>& set_default(const T& value);
        SingleArgumentBuilder<T>& store(T& store);
        SingleArgumentBuilder<T>& positional();
        MultipleArgumentBuilder<T> multiple();
        MultipleArgumentBuilder<T> multiple(size_t min_size);
    };

    template <>
    struct SingleArgumentBuilder<bool> : OptionBuilder {
        explicit SingleArgumentBuilder(std::unique_ptr<Option>& ptr);

        SingleArgumentBuilder<bool>& set_default(bool value);
        SingleArgumentBuilder<bool>& store(bool& store);
    };
}
