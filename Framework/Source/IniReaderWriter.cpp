// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-18

#include "IniReaderWriter.h"

#include <istream>
#include <ostream>



namespace Project001
{
    template <typename MapType>
    void ReadIniStream(MapType& sections, std::istream& inputStream)
    {
        std::string line;
        std::string currentSection;

        while (std::getline(inputStream, line))
        {
            line = TrimWhiteSpace(line);

            // Skip empty or comment-only lines
            if (line.empty() || line[0] == '#' || line[0] == ';')
            {
                continue;
            }

            // Handle section headers
            if (!line.empty() && line.front() == '[')
            {
                size_t closingBracketPos = line.find(']');
                if (closingBracketPos != std::string::npos)
                {
                    std::string sectionName = line.substr(1, closingBracketPos - 1);
                    sectionName = TrimWhiteSpace(sectionName);
                    currentSection = sectionName;
                    continue;
                }
            }

            // Start Parsing key-value pairs
            size_t equalPos = line.find('=');
            if (equalPos == std::string::npos)
            {
                continue; // skip invalid lines
            }

            std::string key = TrimWhiteSpace(line.substr(0, equalPos));
            std::string value = TrimWhiteSpace(line.substr(equalPos + 1));

            // Remove comments from value (the ones not inside quotes)
            bool currentlyInQuotes = false;
            for (size_t i = 0; i < value.size(); ++i)
            {
                if (value[i] == '"' && (i == 0 || value[i - 1] != '\\'))
                {
                    currentlyInQuotes = !currentlyInQuotes;
                }

                if (!currentlyInQuotes && (value[i] == '#' || value[i] == ';'))
                {
                    value = value.substr(0, i);
                    break;
                }
            }

            value = TrimWhiteSpace(value);

            // Strip quotes from value (if it's in quotes)
            if (!value.empty() && value.front() == '"' && value.back() == '"')
            {
                value = value.substr(1, value.size() - 2);

                value = ParseEscapes(value);

                value = TrimWhiteSpace(value);
            }
            else
            {
                value = ParseEscapes(value);
            }

            sections[currentSection][key] = value;
        }
    }

    template void ReadIniStream<std::map<std::string, std::map<std::string, std::string>>>
        (std::map<std::string, std::map<std::string, std::string>>&, std::istream&);
    template void ReadIniStream<std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>
        (std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&, std::istream&);

    template <typename MapType>
    void WriteIniStream(std::ostream& outputStream,
        const MapType& sections)
    {
        // Iterate over sections
        MapType::const_iterator sectionIt;
        for (sectionIt = sections.begin(); sectionIt != sections.end(); ++sectionIt)
        {
            const std::string& sectionName = sectionIt->first;
            const auto& keyValues = sectionIt->second;

            // Write section header
            outputStream << "[" << sectionName << "]\n";

            // Iterate over key-value pairs
            for (auto kvIt = keyValues.begin(); kvIt != keyValues.end(); ++kvIt)
            {
                const std::string& key = kvIt->first;
                const std::string& value = kvIt->second;

                // Escape special characters
                std::string escapedValue = UnParseEscapes(value);

                // Wrap in quotes if it contains '#' or ';'
                bool needsQuotes = escapedValue.find_first_of("#;") != std::string::npos;
                if (needsQuotes)
                {
                    outputStream << key << "=\"" << escapedValue << "\"\n";
                }
                else
                {
                    outputStream << key << "=" << escapedValue << "\n";
                }
            }

            outputStream << "\n"; // Separate sections by a blank line
        }
    }

    template void WriteIniStream<std::map<std::string, std::map<std::string, std::string>>>
        (std::ostream&, const std::map<std::string, std::map<std::string, std::string>>&);
    template void WriteIniStream<std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>
        (std::ostream&, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&);

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