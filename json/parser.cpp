#include "parser.h"
#include <iostream>
using namespace daxi::Json;

Parser::Parser() :m_idx(0)
{

}
Parser::~Parser()
{

}

void Parser::load(const std::string &str)
{
    m_str = str;
    m_idx = 0;
}

void Parser::skip_white_space()
{
    while(m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
    {
        m_idx++;
    }
}

char Parser::get_next_token()
{
    skip_white_space();
    if(m_idx == m_str.size())
        throw std::logic_error("unexpected end of input");
    return m_str[m_idx++];
}

JsonElement Parser::parser_null()
{
    if(m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return JsonElement();
    }
    throw std::logic_error("parser null error");
}

JsonElement Parser::parser_bool()
{
    if(m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return JsonElement(true);
    }
    else if(m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return JsonElement(false);
    }
    throw std::logic_error("parser bool error");
}

JsonElement Parser::parser_number()
{
    size_t pos = m_idx;
    if(m_str[m_idx] == '-')
        m_idx++;

    if(m_str[m_idx] == '0')
    {
        m_idx++;
    }
    else if(in_range(m_str[m_idx], '1', '9'))
    {
        m_idx++;
        while(in_range(m_str[m_idx], '0', '9'))
            m_idx++;
    }
    else
    {
        throw std::logic_error("invalid character in number");
    }

    if(m_str[m_idx] != '.')
        return JsonElement(std::atoi(m_str.c_str() + pos));

    m_idx++;
    if(!in_range(m_str[m_idx], '0', '9'))
        throw std::logic_error("at least one digit required in fractional part");
    
    while(in_range(m_str[m_idx], '0', '9')) m_idx++;

    return JsonElement(std::atoi(m_str.c_str() + pos));
}

JsonElement Parser::parser_string()
{
    int pos=m_idx;
    while(true)
    {
        if(m_idx == m_str.size())
            throw std::logic_error("unexpected end of input in string");

        char ch = m_str[m_idx++];
        if(ch == '"') break;
        if(ch == '\\')
        {
            ch = m_str[m_idx++];
            switch (ch)
            {
            case 'b':
            case 't':
            case 'n':
            case 'f':
            case 'r':
            case '"':
            case '\\':
                break;
            case 'u':
                m_idx += 4;
                break;
            default:
                break;
            }
        }
    }

    return m_str.substr(pos, m_idx - pos - 1);
}

JsonElement Parser::parser_array()
{
    JsonElement arr(JsonElement::json_array);
    char ch = get_next_token();
    if(ch == ']') return arr;

    m_idx--;
    while(true)
    {
        arr.append(parser());
        ch = get_next_token();
        if(ch == ']') break;
        if(ch != ',') std::logic_error("expected ',' in array");
    }
    return arr;
}

JsonElement Parser::parser_object()
{
    JsonElement obj(JsonElement::json_object);
    char ch = get_next_token();
    
    if(ch == '}') return obj;
    m_idx--;
    while(true)
    {
        ch = get_next_token();
        if (ch != '"')
        {
            throw std::logic_error("expected '\"' in object");
        }
        std::string key = parser_string();
        // std::cout << key << std::endl;
        ch = get_next_token();
        if (ch != ':')
        {
            throw std::logic_error("expected ':' in object");
        }
        obj[key] = parser();
        ch = get_next_token();
        if (ch == '}')
        {
            break;
        }
        if (ch != ',')
        {
            throw std::logic_error("expected ',' in object");
        }
    }
    return obj;
}


JsonElement Parser::parser()
{
    char ch = get_next_token();
    switch (ch)
    {
    case 'n':
        m_idx--;
        return parser_null();
    case 't':
    case 'f':
        m_idx--;
        return parser_bool();
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
        m_idx--;
        return parser_number();
    case '"':
        return parser_string();
    case '[':
        return parser_array();
    case '{':
        return parser_object();
    
    default:
        break;
    }
    throw std::logic_error("unexpected character in parse json");
}

