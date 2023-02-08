#pragma once

#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace omfl {
    class Value;
    class Omfl;
    class Section;

    using SectionContainer = std::unordered_map<std::string, std::unique_ptr<Omfl>>;
    using Array = std::vector<Value>;

    class Omfl {
    public:
        Omfl() = default;
        Omfl(const Omfl& other) = default;
        Omfl(Omfl&& other) noexcept = default;
        Omfl& operator=(const Omfl& other) = default;
        Omfl& operator=(Omfl&& other) noexcept = default;

        virtual ~Omfl() = default;

        [[nodiscard]] virtual bool Valid() const = 0;
        [[nodiscard]] virtual bool Has(const std::string& name) const = 0;
        [[nodiscard]] virtual const Omfl& Get(const std::string& name) const = 0;

        template <class T>
        [[nodiscard]] bool Is() const;

        [[nodiscard]] bool IsSection() const;
        [[nodiscard]] bool IsValue() const;

        template <class T>
        [[nodiscard]] const T& As() const;

        [[nodiscard]] Section& AsSection() const;
        [[nodiscard]] Value& AsValue() const;

        template <class T>
        [[nodiscard]] const T& AsOrDefault(const T& value) const;

        virtual const Omfl& operator[](size_t index) const = 0;

    protected:
        [[nodiscard]] virtual const void* Raw() const = 0;
        [[nodiscard]] virtual const std::type_info& Type() const = 0;
    };

    template bool Omfl::Is<bool>() const;
    template bool Omfl::Is<int>() const;
    template bool Omfl::Is<float>() const;
    template bool Omfl::Is<std::string>() const;
    template bool Omfl::Is<Array>() const;
    template bool Omfl::Is<SectionContainer>() const;

    template const bool& Omfl::As<bool>() const;
    template const int& Omfl::As<int>() const;
    template const float& Omfl::As<float>() const;
    template const std::string& Omfl::As<std::string>() const;
    template const Array& Omfl::As<Array>() const;
    template const SectionContainer& Omfl::As<SectionContainer>() const;

    class Value final : public Omfl {
    public:
        explicit Value(std::any value);

        [[nodiscard]] bool Valid() const override;
        [[nodiscard]] bool Has(const std::string& name) const override;
        [[nodiscard]] const Omfl& Get(const std::string& name) const override;

        template <class T>
        void Set(const T& value);

        const Omfl& operator[](size_t index) const override;

    protected:
        [[nodiscard]] const void* Raw() const override;
        [[nodiscard]] const std::type_info& Type() const override;

    private:
        std::any data_;
    };

    class Section final : public Omfl {
    public:
        [[nodiscard]] bool Valid() const override;
        [[nodiscard]] bool Has(const std::string& name) const override;
        [[nodiscard]] const Omfl& Get(const std::string& name) const override;

        void SetValue(const std::string& name, Value value);
        void SetSection(const std::string& name, Section section);

        const Omfl& operator[](size_t index) const override;

    protected:
        [[nodiscard]] const void* Raw() const override;
        [[nodiscard]] const std::type_info& Type() const override;

    private:
        SectionContainer data_;
    };
}
