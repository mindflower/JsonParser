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
        using Map = std::map<std::string, JsonObject>;
        using Array = std::vector<JsonObject>;

    public:
        static JsonObject FromString(const std::string& json);

    public:
        JsonObject() = default;
        explicit JsonObject(const std::string& value);
        explicit JsonObject(double value);
        explicit JsonObject(bool value);
        explicit JsonObject(Map&& jsonMap);
        explicit JsonObject(Array&& jsonArray);

        std::string GetAsString() const;
        double GetAsDouble() const;
        int64_t GetAsInt64() const;
        bool GetAsBool() const;

        JsonObject& operator[](const std::string& key);
        JsonObject& operator[](int index);

        JsonObject& operator=(const std::string& value);
        JsonObject& operator=(double value);

        template <class T, class Y = std::enable_if_t<std::is_same<T, bool>{}>>
        JsonObject& operator=(T value)
        {
            m_container = value;
            return *this;
        }

        bool IsObject() const;
        bool IsArray() const;
        bool IsString() const;
        bool IsNumber() const;
        bool IsBool() const;
        bool IsEmpty() const;

    private:
        std::variant<std::monostate, Map, Array, std::string, double, bool> m_container;
    };
}
