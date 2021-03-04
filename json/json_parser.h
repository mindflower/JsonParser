#pragma once
#include "json_lexer.h"
#include "json_object.h"
#include <stack>
#include <variant>
#include <map>
#include <vector>
#include <optional>

namespace json
{
    class JsonParser
    {
    public:
        JsonParser(const std::string& json);
        JsonObject Get() const;

    private:
        enum class State
        {
            READ_JSON,
            READ_OBJECT,
            READ_OBJECT_KEY,
            READ_OBJECT_VALUE,
            READ_ARRAY,
            READ_ARRAY_VALUE
        };

    private:
        void Parse();
        void StartObject();
        void EndObject();
        void StartArray();
        void EndArray();
        void StartObjectValue();
        void StartElement();
        void PushValue();
        void EndJson();

        bool IsJsonStart() const;
        bool IsObjectStart() const;
        bool IsArrayStart() const;
        std::pair<std::string, JsonObject> PopObjectValue();
        JsonObject PopArrayValue();
        JsonObject PopJsonObject();

    private:
        JsonLexer m_lexer;
        JsonObject m_container;
        std::string m_json;
        JsonLexer::TokenInfo m_tokenInfo;
        std::stack<std::variant<std::string, JsonObject, State>> m_stateStack;
        std::stack<State> m_context;
    };
}
