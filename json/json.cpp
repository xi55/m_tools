#include "json.h"
#include "parser.h"
#include <stdexcept>
#include <sstream>
using namespace daxi::Json;

JsonElement::JsonElement() : m_type(json_null){}
JsonElement::~JsonElement(){}
JsonElement::JsonElement(bool value) : m_type(json_bool){m_value.m_bool = value;}
JsonElement::JsonElement(int value) : m_type(json_int){m_value.m_int = value;}
JsonElement::JsonElement(double value) : m_type(json_double){m_value.m_double = value;}
JsonElement::JsonElement(const char * value) : m_type(json_string){m_value.m_string = new std::string(value);}
JsonElement::JsonElement(const std::string &value) : m_type(json_string){m_value.m_string = new std::string(value);}
JsonElement::JsonElement(Type type) : m_type(type)
{
    switch (m_type)
    {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:
        m_value.m_string = new std::string("");
        break;
    case json_array:
        m_value.m_array = new std::vector<JsonElement>();
        break;
    case json_object:
        m_value.m_object = new std::map<std::string, JsonElement>();
        break;
    default:
        break;
    }
}
JsonElement::JsonElement(const JsonElement &other)
{
    copy(other);
}

void JsonElement::parser(const std::string &str)
{
    Parser parser;
    parser.load(str);
    *this = parser.parser();
}

JsonElement::operator bool()
{
    if(m_type != json_bool)
    {
        throw new std::logic_error("type error, requie bool");
    }
    return m_value.m_bool;
}
JsonElement::operator double()
{
    if(m_type != json_double)
    {
        throw new std::logic_error("type error, requie double");
    }
    return m_value.m_double;
}
JsonElement::operator int()
{
    if(m_type != json_int)
    {
        throw new std::logic_error("type error, requie int");
    }
    return m_value.m_int;
}
JsonElement::operator std::string()
{
    if(m_type != json_string)
    {
        throw new std::logic_error("type error, requie string");
    }
    return *(m_value.m_string);
}

JsonElement & JsonElement::operator [] (int index)
{
    if(m_type != json_array)
    {
        m_type = json_array;
        m_value.m_array = new std::vector<JsonElement>();
    }
    if(index < 0) throw new std::logic_error("index can not less than 0");
    int size = (m_value.m_array)->size();
    if(index >= size)
    {
        for(int i = size; i <= index; i++)
        {
            (m_value.m_array)->push_back(JsonElement());
        }
    }
    return (m_value.m_array)->at(index);
}
void JsonElement::append(const JsonElement & other)
{
    // clear();
    if(m_type != json_array)
    {
        m_type = json_array;
        m_value.m_array = new std::vector<JsonElement>();
    }
    (m_value.m_array)->push_back(other);
}

void JsonElement::copy(const JsonElement & other)
{
    m_type = other.m_type;
    switch (m_type)
    {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = other.m_value.m_bool;
        break;
    case json_int:
        m_value.m_int = other.m_value.m_int;
        break;
    case json_double:
        m_value.m_double = other.m_value.m_double;
        break;
    case json_string:
        m_value.m_string = other.m_value.m_string;
        break;
    case json_array:
        m_value.m_array = other.m_value.m_array;
        break;
    case json_object:
        m_value.m_object = other.m_value.m_object;
        break;
    default:
        break;
    }
}
void JsonElement::clear()
{
    switch (m_type)
    {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:
        delete m_value.m_string;
        break;
    case json_array:
    {
        for(auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
            it->clear();
        delete m_value.m_array;
        break;
    }
    case json_object:
    {
        for(auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
            (it->second).clear();
        delete m_value.m_object;
        break;
    }
    default:
        break;
    }
    m_type = json_null;
}

std::string JsonElement::str() const
{
    std::stringstream ss;
    switch (m_type)
    {
    case json_null:
        ss << "null";
        break;
    case json_bool:
        if(m_value.m_bool)
            ss << "true";
        else
            ss << "false";
        break;
    case json_int:
        ss << m_value.m_int;
        break;
    case json_double:
        ss << m_value.m_double;
        break;
    case json_string:
        ss << '\"' << *(m_value.m_string) << '\"';
        break;
    case json_array:
        {
            ss << '[';
            for(auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
            {
                if(it != (m_value.m_array)->begin())
                {
                    ss << ',';
                }
                ss << it->str();
            }
            ss << ']';
            break;
        }
    case json_object:
        {
            ss << '{';
            for(auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
            {
                if(it != (m_value.m_object)->begin())
                {
                    ss << ',';
                }
                ss << '\"' << it->first << '\"' << ':' << it->second.str();
            }
            ss << '}';
            break;
        }
        break;
    default:
        break;
    }

    return ss.str();
}

JsonElement & JsonElement::operator [] (const char * key)
{
    std::string name(key);
    return (*(this))[name];
}
JsonElement & JsonElement::operator [] (const std::string & key)
{
    if(m_type != json_object)
    {
        clear();
        m_type = json_object;
        m_value.m_object = new std::map<std::string, JsonElement>();
    }
    return (*(m_value.m_object))[key];
}
void JsonElement::operator = (const JsonElement & other)
{
    clear();
    copy(other);
}
bool JsonElement::operator == (const JsonElement & other)
{
    if(m_type != other.m_type) return false;
    switch (m_type)
    {
    case json_null:
        return true;
    case json_bool:
        return m_value.m_bool == other.m_value.m_bool;
    case json_int:
        return m_value.m_int == other.m_value.m_int;
    case json_double:
        return m_value.m_double == other.m_value.m_double;
    case json_string:
        return *(m_value.m_string) == *(other.m_value.m_string);
    case json_array:
        return m_value.m_array == other.m_value.m_array;
    case json_object:
        return m_value.m_object == other.m_value.m_object;
    default:
        break;
    }
    return false;
}
bool JsonElement::operator != (const JsonElement & other)
{
    return !(*(this) == other);
}

JsonElement::Type JsonElement::type() const
{
    return m_type;
}

bool JsonElement::isNull() const
{
    return m_type == json_null;
}
bool JsonElement::isBool() const
{
    return m_type == json_bool;
}
bool JsonElement::isInt() const
{
    return m_type == json_int;
}
bool JsonElement::isDouble() const
{
    return m_type == json_double;
}
bool JsonElement::isString() const
{
    return m_type == json_string;
}
bool JsonElement::isArray() const
{
    return m_type == json_array;
}
bool JsonElement::isObject() const
{
    return m_type == json_object;
}


bool JsonElement::has(int index)
{
    if (m_type != json_array)
    {
        return false;
    }
    int size = (m_value.m_array)->size();
    return (index >= 0) && (index < size);
}

bool JsonElement::has(const char * key)
{
    std::string name(key);
    return has(name);
}

bool JsonElement::has(const std::string & key)
{
    if (m_type != json_object)
    {
        return false;
    }
    return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}

void JsonElement::remove(int index)
{
    if (m_type != json_array)
    {
        return;
    }
    int size = (m_value.m_array)->size();
    if (index < 0 || index >= size)
    {
        return;
    }
    auto it = (m_value.m_array)->begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }
    it->clear();
    (m_value.m_array)->erase(it);
}

void JsonElement::remove(const char * key)
{
    std::string name = key;
    remove(name);
}

void JsonElement::remove(const std::string & key)
{
    if (m_type != json_object)
    {
        return;
    }
    auto it = (m_value.m_object)->find(key);
    if (it != (m_value.m_object)->end())
    {
        it->second.clear();
        (m_value.m_object)->erase(key);
    }
}

