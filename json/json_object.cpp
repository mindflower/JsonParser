#include "json_object.h"
#include "json_parser.h"
#include "json_exception.h"

using namespace std;

namespace json
{
    JsonObject JsonObject::FromString(const std::wstring& json)
    {
        return JsonParser{json}.Get();
    }

    JsonObject::JsonObject(const wstring& value) :
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

    wstring& JsonObject::GetAsString()
    {
        if (IsAny())
        {
            m_container = wstring{};
        }
        if (IsString())
        {
            return get<wstring>(m_container);
        }
        else
        {
            throw JsonAccessException(JsonAccessException::Type::STRING);
        }
    }
    JsonObject& JsonObject::operator[](const wstring& key)
    {
        if (IsAny())
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
    JsonObject& JsonObject::operator[](int index)
    {
        if (IsAny())
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

    bool JsonObject::IsAny() const
    {
        return holds_alternative<monostate>(m_container);
    }
}
