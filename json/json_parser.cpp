#include "json_parser.h"
#include "json_exception.h"
#include <sstream>
#include <list>

using namespace std;

namespace json
{
    JsonParser::JsonParser(const wstring& json) :
        m_json(json),
        m_lexer(json)
    {
        Parse();
    }

    JsonObject JsonParser::Get() const
    {
        return m_container;;
    }

    void JsonParser::Parse()
    {
        m_stateStack.emplace(State::READ_JSON);
        for (m_tokenInfo = m_lexer.GetNextToken(); m_tokenInfo.type != JsonLexer::Token::END; m_tokenInfo = m_lexer.GetNextToken())
        {
            switch (m_tokenInfo.type)
            {
            case JsonLexer::Token::BRACE_LEFT: StartObject(); break;
            case JsonLexer::Token::BRACE_RIGTH: EndObject(); break;
            case JsonLexer::Token::BRACKET_LEFT: StartArray(); break;
            case JsonLexer::Token::BRACKET_RIGTH: EndArray(); break;
            case JsonLexer::Token::COLON: StartObjectValue(); break;
            case JsonLexer::Token::COMMA: StartElement(); break;
            case JsonLexer::Token::STRING:
            case JsonLexer::Token::NUMBER:
            case JsonLexer::Token::TRUE:
            case JsonLexer::Token::FALSE:
                PushValue();
                break;
            }
        }
        EndJson();
    }

    void JsonParser::StartObject()
    {
        m_context.push(State::READ_OBJECT);
        m_stateStack.emplace(State::READ_OBJECT);
    }

    void JsonParser::EndObject()
    {
        if (State::READ_OBJECT != m_context.top())
        {
            throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
        }
        m_context.pop();

        JsonObject::Map values;
        while (!IsObjectStart())
        {
            const auto value = PopObjectValue();
            values.emplace(value);
        }
        m_stateStack.pop();
        m_stateStack.emplace(JsonObject{move(values)});
    }

    void JsonParser::StartArray()
    {
        m_context.push(State::READ_ARRAY);
        m_stateStack.emplace(State::READ_ARRAY);
    }

    void JsonParser::EndArray()
    {
        if (State::READ_ARRAY != m_context.top())
        {
            throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
        }
        m_context.pop();

        JsonObject::Array values;
        while (!IsArrayStart())
        {
            const auto value = PopArrayValue();
            values.emplace_back(value);
        }
        m_stateStack.pop();
        reverse(values.begin(), values.end());
        m_stateStack.emplace(JsonObject{move(values)});
    }

    void JsonParser::StartObjectValue()
    {
        if (State::READ_OBJECT == m_context.top())
        {
            m_stateStack.emplace(State::READ_OBJECT_VALUE);
        }
        else
        {
            throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
        }
    }

    void JsonParser::StartElement()
    {
        const auto context = m_context.top();
        if (State::READ_OBJECT == context)
        {
            m_stateStack.emplace(State::READ_OBJECT_KEY);
        }
        else if (State::READ_ARRAY == context)
        {
            m_stateStack.emplace(State::READ_ARRAY_VALUE);
        }
        else
        {
            throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
        }
    }

    void JsonParser::PushValue()
    {
        const auto validObjectKey = [this]()
        {
            if (auto pState = get_if<State>(&m_stateStack.top()))
            {
                const auto state = *pState;
                if (State::READ_OBJECT == m_context.top() &&
                    State::READ_OBJECT_KEY == state &&
                    JsonLexer::Token::STRING != m_tokenInfo.type)
                {
                    return false;
                }
            }
            return true;
        }();

        if (validObjectKey)
        {
            m_stateStack.emplace(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size));
        }
        else
        {
            throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
        }
    }

    void JsonParser::EndJson()
    {
        if (!IsJsonStart())
        {
            m_container = PopJsonObject();
        }
        if (!IsJsonStart())
        {
            throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
        }
    }

    bool JsonParser::IsJsonStart() const
    {
        if (auto pState = get_if<State>(&m_stateStack.top()))
        {
            return State::READ_JSON == *pState;
        }
        else
        {
            return false;
        }
    }

    bool JsonParser::IsObjectStart() const
    {
        if (auto pState = get_if<State>(&m_stateStack.top()))
        {
            return State::READ_OBJECT == *pState;
        }
        else
        {
            return false;
        }
    }

    bool JsonParser::IsArrayStart() const
    {
        if (auto pState = get_if<State>(&m_stateStack.top()))
        {
            return State::READ_ARRAY == *pState;
        }
        else
        {
            return false;
        }
    }

    pair<wstring, JsonObject> JsonParser::PopObjectValue()
    {
        const auto value = PopJsonObject();
        if (auto pState = get_if<State>(&m_stateStack.top()))
        {
            if (State::READ_OBJECT_VALUE != *pState)
            {
                throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
            }
        }
        m_stateStack.pop();

        const auto key = [this]() -> wstring
        {
            if (auto pKey = get_if<wstring>(&m_stateStack.top()))
            {
                const auto key = *pKey;
                m_stateStack.pop();
                return key;
            }
            else
            {
                throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
            }
        }();

        if (auto pState = get_if<State>(&m_stateStack.top()))
        {
            const auto state = *pState;
            if (State::READ_OBJECT_KEY == state)
            {
                m_stateStack.pop();
            }
            else if (State::READ_OBJECT != state)
            {
                throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
            }
        }
        return {key, value};
    }

    JsonObject JsonParser::PopArrayValue()
    { 
        const auto value = PopJsonObject();
        if (auto pState = get_if<State>(&m_stateStack.top()))
        {
            const auto state = *pState;
            if (State::READ_ARRAY_VALUE == state)
            {
                m_stateStack.pop();
            }
            else if (State::READ_ARRAY != state)
            {
                throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
            }
        }
        return value;
    }

    JsonObject JsonParser::PopJsonObject()
    {
        const auto value = visit([this](auto& arg) -> JsonObject
        {
            using T = decay_t<decltype(arg)>;
            if constexpr (is_same_v<T, wstring>)
            {
                return JsonObject{arg};
            }
            else if constexpr (is_same_v<T, JsonObject>)
            {
                return arg;
            }
            else
            {
                throw JsonParseException(m_json.substr(m_tokenInfo.offset, m_tokenInfo.size), m_tokenInfo.offset);
            }
        }, m_stateStack.top());
        m_stateStack.pop();
        return value;
    }
}
