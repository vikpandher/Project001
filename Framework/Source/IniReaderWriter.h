// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-18

#pragma once

#include <map>
#include <string>
#include <unordered_map>



// |           Input Line           | Parsed Key |     Parsed Value     |               Notes               |
// |--------------------------------|------------|----------------------|-----------------------------------|
// | [Section]                      |            |                      | creates `"Section"` section       |
// | [ Section 1 ]                  |            |                      | creates `"Section 1"` section     |
// | [Section] # comment            |            |                      | leading / trailing spaces ignored |
// | [Section] ; comment            |            |                      | same as above                     |
// | key=value                      | key        | value                | normal                            |
// | k e y = v a l u e              | k e y      | v a l u e            | leading / trailing spaces ignored |
// | key = "value"                  | key        | value                | quotes removed                    |
// | key = "abc#def"                | key        | abc#def              | `#` kept                          |
// | key = value # comment          | key        | value                | comment stripped                  |
// | key = "value # not comment"    | key        | value # not comment  | `#` kept                          |
// | key = "quoted \"value\""       | key        | quoted "value"       | escaped quotes supported          |
// | key = "\"quoted\""             | key        | "quoted"             | keeps quotes inside               |
// | key = \\value\\                | key        | \value \             | escape for \                      |
// | key = "\\value\\"              | key        | \value \             | same in quotes                    |
// | key = "abc\zdef"               | key        | abcdef               | unkonwn escape '\z' discarded     |
// | key = "dangling\""             | key        | dangling"            | quotes removed                    |
// | key = "dangling\"              | key        | dangling             | similar, but unkonwn escape '\'   |
// | key = "dangling                | key        | "dangling            | value not fully in quotes         |
// | key = "dangling # comment      | key        | "dangling # comment  | same as above                     |
// 
// * Sections with no values will be ignored

namespace Project001
{
    // The map's keys are section names and the values are a second level of maps.
    // The second level maps' keys are variable names and the values are the
    // associated variable values.
    template <typename MapType>
    void ReadIniStream(MapType& sections, std::istream& inputStream);

    template void ReadIniStream<std::map<std::string, std::map<std::string, std::string>>>
        (std::map<std::string, std::map<std::string, std::string>>&, std::istream&);
    template void ReadIniStream<std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>
        (std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&, std::istream&);

    template <typename MapType>
    void WriteIniStream(std::ostream& outputStream, const MapType& sections);

    template void WriteIniStream<std::map<std::string, std::map<std::string, std::string>>>
        (std::ostream&, const std::map<std::string, std::map<std::string, std::string>>&);
    template void WriteIniStream<std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>
        (std::ostream&, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&);

    // Trims whitespace infront and at the end of a string.
    std::string TrimWhiteSpace(const std::string& str);

    // Parses escape sequences in a string.
    // Recognized sequences are \n, \r, \t, \", \\
    // Unknown or dangling escapes are ignored (backslash discarded)
    std::string ParseEscapes(const std::string& str);

    // Converts certain chacters into escape sequence pairs.
    std::string UnParseEscapes(const std::string& str);
}