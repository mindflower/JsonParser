#include "json_object.h"
#include "json_parser.h"
#include "json_exception.h"

using namespace std;

namespace json
{
    JsonObject JsonObject::FromString(const wstring& json)
    {
        return JsonParser{json}.Get();
    }

    JsonObject::JsonObject(const wstring& value) :
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

    JsonObject::JsonObject(const Map&& jsonMap) :
        m_container(jsonMap)
    {
    }

    JsonObject::JsonObject(const Array&& jsonArray) :
        m_container(jsonArray)
    {
    }

    wstring JsonObject::GetAsString() const
    {
        if (IsString())
        {
            return get<wstring>(m_container);
        }
        else if (IsNumber())
        {
            return to_wstring(get<double>(m_container));
        }
        else if (IsBool())
        {
            return get<bool>(m_container) ? L"true" : L"false";
        }
        else if (IsEmpty())
        {
            return L"null";
        }
        else
        {
            throw JsonAccessException(JsonAccessException::Type::STRING);
        }
    }

    double JsonObject::GetAsDobule() const
    {
        if (IsNumber())
        {
            return get<double>(m_container);
        }
        else
        {
            throw JsonAccessException(JsonAccessException::Type::NUMBER);
        }
    }

    int64_t JsonObject::GetAsInt64() const
    {
        return static_cast<int64_t>(GetAsDobule());
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

    JsonObject& JsonObject::operator[](const wstring& key)
    {
        if (IsEmpty())
        {
            m_container = Map{};
        }
        if (IsObject())
        {
            return get<Map>(m_container)[key];
        }
        else
        {
            throw JsonAccessException(JsonAccessException::Type::OBJECT);
        }
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
        else
        {
            throw JsonAccessException(JsonAccessException::Type::ARRAY);
        }
    }

    JsonObject& JsonObject::operator=(const wstring& value)
    {
        m_container = wstring{value};
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
        return holds_alternative<wstring>(m_container);
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
