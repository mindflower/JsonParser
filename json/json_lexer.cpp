#include "json_lexer.h"
#include "json_exception.h"
#include <regex>

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
        case L'-':
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
        while (c != L'"' && m_offset < m_json.size()) c = m_json.at(++m_offset);

        if (c != L'"')
        {
            throw JsonParseException(m_json.substr(m_offset - 1, 1), m_offset-1);
        }
        return {JsonLexer::Token::STRING, oldOffset+1, (++m_offset - oldOffset)-2};
    }

    JsonLexer::TokenInfo JsonLexer::ProcessNumber()
    {
        size_t characterReadCount = 0;
        stod(m_json.substr(m_offset), &characterReadCount);
        if (0 != characterReadCount)
        {
            const auto oldOffset = m_offset;
            m_offset += characterReadCount;
            return {JsonLexer::Token::NUMBER, oldOffset, characterReadCount};
        }
        else
        {
            throw JsonParseException(m_json.substr(m_offset - 1, 1), m_offset - 1);
        }
    }

    JsonLexer::TokenInfo JsonLexer::ProcessOther()
    {
        static const wregex TRUE_REGEX(L"true");
        static const wregex FALSE_REGEX(L"false");
        static const wregex NULL_REGEX(L"null");
        static const size_t TRUE_SIZE = 4;
        static const size_t FALSE_SIZE = 5;
        static const size_t NULL_SIZE = 4;

        const auto oldOffset = m_offset;
        if (regex_match(m_json.substr(m_offset, TRUE_SIZE), TRUE_REGEX))
        {
            m_offset += TRUE_SIZE;
            return {JsonLexer::Token::TRUE, oldOffset, TRUE_SIZE};
        }
        else if (regex_match(m_json.substr(m_offset, FALSE_SIZE), FALSE_REGEX))
        {
            m_offset += FALSE_SIZE;
            return {JsonLexer::Token::FALSE, oldOffset, FALSE_SIZE};
        }
        else if (regex_match(m_json.substr(m_offset, NULL_SIZE), NULL_REGEX))
        {
            m_offset += NULL_SIZE;
            return {JsonLexer::Token::NIL, oldOffset, NULL_SIZE};
        }
        else
        {
            throw JsonParseException(m_json.substr(m_offset - 1, 1), m_offset - 1);
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
