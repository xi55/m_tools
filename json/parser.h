#pragma once

#include <string>
#include "json.h"

namespace daxi
{
    namespace Json
    {
        class Parser
        {
        public:
            Parser();
            ~Parser();

            void load(const std::string &str);
            JsonElement parser();

        private:
            std::string m_str;
            size_t m_idx;

            void skip_white_space();
            char get_next_token();

            JsonElement parser_null();
            JsonElement parser_bool();
            JsonElement parser_number();
            JsonElement parser_string();
            JsonElement parser_array();
            JsonElement parser_object();

            bool in_range(int x, int low, int upper)
            {
                return (x >= low && x <= upper);
            }
        };
    }
}