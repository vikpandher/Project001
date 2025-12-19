// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#include "StringUtility.h"



namespace Project001
{
namespace String
{
    std::string TrimWhiteSpace(const std::string& str)
    {
        size_t first = str.find_first_not_of(" \n\r\t"); // space, new-line, carriage-return, tab
        size_t last = str.find_last_not_of(" \n\r\t");
        if (first == std::string::npos || last == std::string::npos)
        {
            return "";
        }
        else
        {
            return str.substr(first, last - first + 1);
        }
    }

    std::string ParseEscapes(const std::string& str)
    {
        std::string result;
        result.reserve(str.size());

        for (size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == '\\')
            {
                if (i + 1 >= str.size())
                {
                    // Lone backslash at end: discard
                    break;
                }

                char next = str[i + 1];
                switch (next)
                {
                case 'n':  result += '\n'; break;
                case 'r':  result += '\r'; break;
                case 't':  result += '\t'; break;
                case '"':  result += '"';  break;
                case '\\': result += '\\'; break;
                default:
                    // Unknown escape: discard backslash, keep nothing
                    break;
                }
                ++i; // skip the next char
            }
            else
            {
                result += str[i];
            }
        }
        return result;
    }

    std::string UnParseEscapes(const std::string& str)
    {
        std::string result;
        result.reserve(str.size()); // Reserve original size as an estimate

        for (char c : str)
        {
            switch (c)
            {
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            default:   result += c; break;
            }
        }

        return result;
    }
}
}