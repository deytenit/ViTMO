#pragma once
#include <string>
#include <memory>
#include <optional>
#include <vector>

namespace argument_parser {
    constexpr char kUndefinedShortcut = '?';

    class Option {
    private:
        char shortcut_ = kUndefinedShortcut;
        std::string name_;
        std::string info_;

    public:
        bool positional_ = false;

        explicit Option(std::string name);
        Option(std::string name, std::string info);
        Option(char shortcut, std::string name);
        Option(char shortcut, std::string name, std::string info);

        Option(const Option& option);

        Option(Option&& option) noexcept;

        virtual ~Option() = default;

        Option& operator=(const Option& option);

        Option& operator=(Option&& option) noexcept;

        [[nodiscard]] char shortcut() const;
        [[nodiscard]] const std::string& name() const;
        [[nodiscard]] const std::string& info() const;

        virtual void from_string(const std::string& str);
        [[nodiscard]] virtual bool has_default_value() const;
    };

    template <class T>
    class SingleArgument final : public Option {
    public:
        std::unique_ptr<T> value_ = std::make_unique<T>();
        T* store_ = value_.get();
        std::optional<T> default_value_;

        explicit SingleArgument(std::string name);
        SingleArgument(std::string name, std::string info);
        SingleArgument(char shortcut, std::string name);
        SingleArgument(char shortcut, std::string name, std::string info);

        void from_string(const std::string& str) override;
        [[nodiscard]] bool has_default_value() const override;
    };

    template <class T>
    class MultipleArgument final : public Option {
    private:
        size_t min_size_ = 0;
    public:
        std::unique_ptr<std::vector<T>> values_ = std::make_unique<std::vector<T>>();
        std::vector<T>* store_ = values_.get();

        MultipleArgument(std::string name, size_t min_size);
        MultipleArgument(std::string name, std::string info, size_t min_size);
        MultipleArgument(char shortcut, std::string name, size_t min_size);
        MultipleArgument(char shortcut, std::string name, std::string info, size_t min_size);

        [[nodiscard]] size_t min_size() const;

        void from_string(const std::string& str) override;
        [[nodiscard]] bool has_default_value() const override;
    };
}
