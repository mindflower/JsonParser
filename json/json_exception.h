#pragma once
#include <exception>
#include <string>

namespace json
{
    class JsonParseException : public std::exception
    {
    public:
        JsonParseException(const std::wstring& token, size_t position);
        const char* what() const override;
        std::wstring Description() const;

    private:
        std::wstring m_description;
    };

    class JsonAccessException : public std::exception
    {
    public:
        enum class Type
        {
            OBJECT,
            ARRAY,
            STRING,
            NUMBER,
            BOOL
        };

    public:
        JsonAccessException(Type type);
        const char* what() const override;
        std::wstring Description() const;

    private:
        std::wstring m_description;
    };
}
