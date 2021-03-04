#include "json_object.h"
#include "json_parser.h"
#include "json_exception.h"

using namespace std;

namespace json
{
    JsonObject JsonObject::FromString(const string& json)
    {
        return JsonParser{json}.Get();
    }

    JsonObject::JsonObject(const string& value) :
        m_container(value)
    {
    }

    JsonObject::JsonObject(const double value) :
        m_container(value)
    {
    }

    JsonObject::JsonObject(const bool value) :
        m_container(value)
    {
    }

    JsonObject::JsonObject(Map&& jsonMap) :
        m_container(jsonMap)
    {
    }

    JsonObject::JsonObject(Array&& jsonArray) :
        m_container(jsonArray)
    {
    }

    string JsonObject::GetAsString() const
    {
        if (IsString())
        {
            return get<string>(m_container);
        }
        if (IsNumber())
        {
            return to_string(get<double>(m_container));
        }
        if (IsBool())
        {
            return get<bool>(m_container) ? "true" : "false";
        }
        if (IsEmpty())
        {
            return "null";
        }
        throw JsonAccessException(JsonAccessException::Type::STRING);
    }

    double JsonObject::GetAsDouble() const
    {
        if (IsNumber())
        {
            return get<double>(m_container);
        }
        if (IsEmpty())
        {
            return 0;
        }
        throw JsonAccessException(JsonAccessException::Type::NUMBER);
    }

    int64_t JsonObject::GetAsInt64() const
    {
        return static_cast<int64_t>(GetAsDouble());
    }

    bool JsonObject::GetAsBool() const
    {
        if (IsBool())
        {
            return get<bool>(m_container);
        }
        else
        {
            throw JsonAccessException(JsonAccessException::Type::BOOL);
        }
    }

    JsonObject& JsonObject::operator[](const string& key)
    {
        if (IsEmpty())
        {
            m_container = Map{};
        }
        if (IsObject())
        {
            return get<Map>(m_container)[key];
        }
        throw JsonAccessException(JsonAccessException::Type::OBJECT);
    }

    JsonObject& JsonObject::operator[](const int index)
    {
        if (IsEmpty())
        {
            m_container = Array{};
        }
        if (IsArray())
        {
            return get<Array>(m_container)[index];
        }
        throw JsonAccessException(JsonAccessException::Type::ARRAY);
    }

    JsonObject& JsonObject::operator=(const string& value)
    {
        m_container = value;
        return *this;
    }

    JsonObject& JsonObject::operator=(const double value)
    {
        m_container = value;
        return *this;
    }

    bool JsonObject::IsObject() const
    {
        return holds_alternative<Map>(m_container);
    }

    bool JsonObject::IsArray() const
    {
        return holds_alternative<Array>(m_container);
    }

    bool JsonObject::IsString() const
    {
        return holds_alternative<string>(m_container);
    }

    bool JsonObject::IsNumber() const
    {
        return holds_alternative<double>(m_container);
    }

    bool JsonObject::IsBool() const
    {
        return holds_alternative<bool>(m_container);
    }

    bool JsonObject::IsEmpty() const
    {
        return holds_alternative<monostate>(m_container);
    }
}
