#include "json_exception.h"
#include <sstream>

using namespace std;

namespace json
{
    JsonParseException::JsonParseException(const string& symbol, const size_t position)
    {
        stringstream ss;
        ss << "Unexpected symbol found near \'" << symbol << "\' at position " << position << '.';
        m_description = ss.str();
    }

    const char* JsonParseException::what() const
    {
        return m_description.c_str();
    }

    JsonAccessException::JsonAccessException(const Type type)
    {
        const auto typeStr = [&type]() -> string
        {
            switch(type)
            {
            case Type::OBJECT: return "Object"s;
            case Type::ARRAY: return "Array"s;
            case Type::STRING: return "String"s;
            default:
                return "Unknown"s;
            }
        }();

        stringstream ss;
        ss << "Bad access to storage value. Expected type \'" << typeStr << "\'.";
        m_description = ss.str();
    }

    const char* JsonAccessException::what() const
    {
        return m_description.c_str();
    }
}
