#pragma once
#include <string>

namespace json
{
    class JsonLexer
    {
    public:
        enum class Token
        {
            END,
            BRACE_LEFT,
            BRACE_RIGTH,
            BRACKET_LEFT,
            BRACKET_RIGTH,
            COLON,
            COMMA,
            STRING,
            NUMBER,
            TRUE,
            FALSE,
            SPACE,
            NEW_LINE
        };

        struct TokenInfo
        {
            Token type = Token::END;
            size_t offset = 0;
            size_t size = 0;
        };

    public:
        JsonLexer(const std::wstring& json);
        TokenInfo GetNextToken();

    private:
        TokenInfo ProcessString();
        TokenInfo ProcessNumber();
        TokenInfo ProcessBooleanAndOther();
        TokenInfo ProcessSpace();
        TokenInfo ProcessNewLine();

    private:
        const std::wstring& m_json;
        size_t m_offset = 0;
        size_t m_size = 0;
    };
}
