#include "json_lexer.h"
#include "json_exception.h"

using namespace std;

namespace json
{
    JsonLexer::JsonLexer(const wstring& json) :
        m_json(json),
        m_size(json.size())
    {
    }

    JsonLexer::TokenInfo JsonLexer::GetNextToken()
    {
        if (m_offset >= m_size)
        {
            return {JsonLexer::Token::END, m_offset, 0};
        }
        const auto c = m_json.at(m_offset);
        switch (c)
        {
        case L'{': return {JsonLexer::Token::BRACE_LEFT, m_offset++, 1};
        case L'}': return {JsonLexer::Token::BRACE_RIGTH, m_offset++, 1};
        case L'[': return {JsonLexer::Token::BRACKET_LEFT, m_offset++, 1};
        case L']': return {JsonLexer::Token::BRACKET_RIGTH, m_offset++, 1};
        case L':': return {JsonLexer::Token::COLON, m_offset++, 1};
        case L',': return {JsonLexer::Token::COMMA, m_offset++, 1};
        case L'"': return ProcessString();
        case L'\n':
            return ProcessNewLine();
        case L' ':
        case L'\t':
            return ProcessSpace();
        case L'0':
        case L'1':
        case L'2':
        case L'3':
        case L'4':
        case L'5':
        case L'6':
        case L'7':
        case L'8':
        case L'9':
            return ProcessNumber();
        default:
            return ProcessBooleanAndOther();
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
        while (c != L'"' && m_offset < m_json.size()) c = m_json.at(++m_offset);

        if (c != L'"')
        {
            throw JsonParseException(m_json.substr(m_offset - 1, 1), m_offset-1);
        }
        return {JsonLexer::Token::STRING, oldOffset+1, (++m_offset - oldOffset)-2};
    }

    JsonLexer::TokenInfo JsonLexer::ProcessNumber()
    {
        const auto oldOffset = m_offset;
        auto c = m_json.at(m_offset);
        while (isdigit(c) && m_offset < m_json.size()) c = m_json.at(++m_offset);

        if (m_offset >= m_json.size())
        {
            throw JsonParseException(m_json.substr(m_offset - 1, 1), m_offset - 1);
        }
        return {JsonLexer::Token::NUMBER, oldOffset, m_offset - oldOffset};
    }

    JsonLexer::TokenInfo JsonLexer::ProcessBooleanAndOther()
    {
        auto c = m_json.at(m_offset);
        if (m_offset + 3 < m_size && L't' == c)
        {
            if (L'r' == m_json.at(m_offset + 1) &&
                L'u' == m_json.at(m_offset + 2) &&
                L'e' == m_json.at(m_offset + 3))
            {
                m_offset += 4;
                return {JsonLexer::Token::TRUE, m_offset - 4, 4};
            }
        }
        else if (m_offset + 4 < m_size && 'f' == c)
        {
            if (L'a' == m_json.at(m_offset + 1) &&
                L'l' == m_json.at(m_offset + 2) &&
                L's' == m_json.at(m_offset + 3) &&
                L'e' == m_json.at(m_offset + 4))
            {
                m_offset += 5;
                return {JsonLexer::Token::FALSE, m_offset - 5, 5};
            }
        }
        else
        {
            throw JsonParseException(m_json.substr(m_offset, 1), m_offset);
        }
    }

    JsonLexer::TokenInfo JsonLexer::ProcessSpace()
    {
         const auto oldOffset = m_offset;
         while (++m_offset < m_size)
         {
             const auto c = m_json.at(m_offset);
             if (L' ' != c && L'\t' != c) break;
         }
         const auto size = m_offset - oldOffset;
         return {JsonLexer::Token::SPACE, oldOffset, size};
    }

    JsonLexer::TokenInfo JsonLexer::ProcessNewLine()
    {
        const auto oldOffset = m_offset++;
        while (m_offset < m_size)
        {
            const auto c = m_json.at(m_offset);
            if (L'\n' != c) break;
        }
        const auto size = m_offset - oldOffset;
        return {JsonLexer::Token::NEW_LINE, oldOffset, size};
    }
}
