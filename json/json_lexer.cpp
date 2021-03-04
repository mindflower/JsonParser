#include "json_lexer.h"
#include "json_exception.h"
#include <regex>

using namespace std;

namespace json
{
    JsonLexer::JsonLexer(const string& json) :
        m_json(json),
        m_size(json.size())
    {
    }

    JsonLexer::TokenInfo JsonLexer::GetNextToken()
    {
        if (m_offset >= m_size)
        {
            return {Token::END, m_offset, 0};
        }
        const auto c = m_json.at(m_offset);
        switch (c)
        {
        case '{': return {Token::BRACE_LEFT, m_offset++, 1};
        case '}': return {Token::BRACE_RIGTH, m_offset++, 1};
        case '[': return {Token::BRACKET_LEFT, m_offset++, 1};
        case ']': return {Token::BRACKET_RIGTH, m_offset++, 1};
        case ':': return {Token::COLON, m_offset++, 1};
        case ',': return {Token::COMMA, m_offset++, 1};
        case '"': return ProcessString();
        case '\n':
            return ProcessNewLine();
        case ' ':
        case '\t':
            return ProcessSpace();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return ProcessNumber();
        default:
            return ProcessOther();
        }
    }

    JsonLexer::TokenInfo JsonLexer::ProcessString()
    {
        const auto oldOffset = m_offset++;
        if (m_offset >= m_json.size())
        {
            throw JsonParseException(m_json.substr(oldOffset, 1), oldOffset);
        }

        auto c = m_json.at(m_offset);
        while (c != '"' && m_offset < m_json.size()) c = m_json.at(++m_offset);

        if (c != '"')
        {
            throw JsonParseException(m_json.substr(m_offset - 1, 1), m_offset - 1);
        }
        return {Token::STRING, oldOffset + 1, (++m_offset - oldOffset) - 2};
    }

    JsonLexer::TokenInfo JsonLexer::ProcessNumber()
    {
        size_t characterReadCount = 0;
        stod(m_json.substr(m_offset), &characterReadCount);
        if (0 != characterReadCount)
        {
            const auto oldOffset = m_offset;
            m_offset += characterReadCount;
            return {Token::NUMBER, oldOffset, characterReadCount};
        }
        throw JsonParseException(m_json.substr(m_offset - 1, 1), m_offset - 1);
    }

    JsonLexer::TokenInfo JsonLexer::ProcessOther()
    {
        static const regex TRUE_REGEX("true");
        static const regex FALSE_REGEX("false");
        static const regex NULL_REGEX("null");
        static const size_t TRUE_SIZE = 4;
        static const size_t FALSE_SIZE = 5;
        static const size_t NULL_SIZE = 4;

        const auto oldOffset = m_offset;
        if (regex_match(m_json.substr(m_offset, TRUE_SIZE), TRUE_REGEX))
        {
            m_offset += TRUE_SIZE;
            return {Token::TRUE, oldOffset, TRUE_SIZE};
        }
        if (regex_match(m_json.substr(m_offset, FALSE_SIZE), FALSE_REGEX))
        {
            m_offset += FALSE_SIZE;
            return {Token::FALSE, oldOffset, FALSE_SIZE};
        }
        if (regex_match(m_json.substr(m_offset, NULL_SIZE), NULL_REGEX))
        {
            m_offset += NULL_SIZE;
            return {Token::NIL, oldOffset, NULL_SIZE};
        }
        throw JsonParseException(m_json.substr(m_offset - 1, 1), m_offset - 1);
    }

    JsonLexer::TokenInfo JsonLexer::ProcessSpace()
    {
        const auto oldOffset = m_offset;
        while (++m_offset < m_size)
        {
            const auto c = m_json.at(m_offset);
            if (' ' != c && '\t' != c) break;
        }
        const auto size = m_offset - oldOffset;
        return {Token::SPACE, oldOffset, size};
    }

    JsonLexer::TokenInfo JsonLexer::ProcessNewLine()
    {
        const auto oldOffset = m_offset++;
        while (m_offset < m_size)
        {
            const auto c = m_json.at(m_offset);
            if ('\n' != c) break;
        }
        const auto size = m_offset - oldOffset;
        return {Token::NEW_LINE, oldOffset, size};
    }
}
