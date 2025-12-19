// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#include "IniReaderWriter.h"

#include "Utilities/StringUtility.h"

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
            line = String::TrimWhiteSpace(line);

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
                    sectionName = String::TrimWhiteSpace(sectionName);
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

            std::string key = String::TrimWhiteSpace(line.substr(0, equalPos));
            std::string value = String::TrimWhiteSpace(line.substr(equalPos + 1));

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

            value = String::TrimWhiteSpace(value);

            // Strip quotes from value (if it's in quotes)
            if (!value.empty() && value.front() == '"' && value.back() == '"')
            {
                value = value.substr(1, value.size() - 2);

                value = String::ParseEscapes(value);

                value = String::TrimWhiteSpace(value);
            }
            else
            {
                value = String::ParseEscapes(value);
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
                std::string escapedValue = String::UnParseEscapes(value);

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
}