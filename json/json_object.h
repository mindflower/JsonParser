#pragma once
#include <string>
#include <map>
#include <vector>
#include <variant>

namespace json
{
    class JsonObject
    {
    public:
        using Map = std::map<std::wstring, JsonObject>;
        using Array = std::vector<JsonObject>;

    public:
        static JsonObject FromString(const std::wstring& json);

    public:
        JsonObject() = default;
        explicit JsonObject(const std::wstring& value);
        explicit JsonObject(const Map&& jsonMap);
        explicit JsonObject(const Array&& jsonArray);

        std::wstring& GetAsString();
        JsonObject& operator[](const std::wstring& key);
        JsonObject& operator[](int index);

        bool IsObject() const;
        bool IsArray() const;
        bool IsString() const;
        bool IsAny() const;

    private:
        std::variant<std::monostate, Map, Array, std::wstring> m_container;
    };
}
