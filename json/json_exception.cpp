#include "json_exception.h"
#include <map>
#include <sstream>

using namespace std;

namespace json
{
    JsonParseException::JsonParseException(const wstring& symbol, size_t position)
    {
        wstringstream ss;
        ss << "Unexpected symbol found near \'" << symbol << "\' at position " << position << '.';
        m_description = ss.str();
    }

    const char* JsonParseException::what() const
    {
        return "JsonParseException occured";
    }

    wstring JsonParseException::Description() const
    {
        return m_description;
    }

    JsonAccessException::JsonAccessException(JsonAccessException::Type type)
    {
        static const map<Type, wstring> converter = {
            {Type::OBJECT, L"Object"},
            {Type::ARRAY, L"Array"},
            {Type::STRING, L"String"}
        };

        const auto typeStr = [&type]() -> wstring
        {
            const auto it = converter.find(type);
            if (it != converter.end())
            {
                return it->second;
            }
            else
            {
                return L"Unknown";
            }
        }();

        wstringstream ss;
        ss << "Bad access to storage value. Expected type \'" << typeStr << "\'.";
        m_description = ss.str();
    }

    const char* JsonAccessException::what() const
    {
        return "JsonAccessException occured";
    }

    wstring JsonAccessException::Description() const
    {
        return m_description;
    }
}
