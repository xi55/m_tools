#pragma once
#include <string>
#include <vector>
#include <map>
namespace daxi
{
    namespace Json
    {
        class JsonElement
        {
        public:
            enum Type
            {
                json_null = 0,
                json_bool,
                json_int,
                json_double,
                json_string,
                json_array,
                json_object
            };

            JsonElement();
            JsonElement(bool value);
            JsonElement(int value);
            JsonElement(double value);
            JsonElement(const char * value);
            JsonElement(const std::string &value);
            JsonElement(Type type);
            JsonElement(const JsonElement &other);
            ~JsonElement();

            void parser(const std::string &str);

            Type type() const;
            bool isNull() const;
            bool isBool() const;
            bool isInt() const;
            bool isDouble() const;
            bool isString() const;
            bool isArray() const;
            bool isObject() const;

            // bool asBool() const;
            // bool asInt() const;
            // bool asDouble() const;
            // bool asString() const;

            // int size() const;
            // bool empty() const;

            bool has(int index);
            bool has(const char * key);
            bool has(const std::string & key);

            void remove(int index);
            void remove(const char * key);
            void remove(const std::string & key);

            operator bool();
            operator int();
            operator double();
            operator std::string();

            JsonElement & operator [] (int index);
            void append(const JsonElement & other);

            std::string str() const;

            JsonElement & operator [] (const char * key);
            JsonElement & operator [] (const std::string & key);
            void operator = (const JsonElement & other);
            bool operator == (const JsonElement & other);
            bool operator != (const JsonElement & other);
            void copy(const JsonElement & other);
            void clear();



        private:
            union Value
            {
                bool m_bool;
                int m_int;
                double m_double;
                std::string *m_string;
                std::vector<JsonElement> *m_array;
                std::map<std::string, JsonElement> *m_object;
            };

            Type m_type;
            Value m_value;
        };
    }


}