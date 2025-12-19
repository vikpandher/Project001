// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#pragma once

#include <string>



namespace Project001
{
namespace String
{
    // Trims whitespace infront and at the end of a string.
    std::string TrimWhiteSpace(const std::string& str);

    // Parses escape sequences in a string.
    // Recognized sequences are \n, \r, \t, \", \\
    // Unknown or dangling escapes are ignored (backslash discarded)
    std::string ParseEscapes(const std::string& str);

    // Converts certain chacters into escape sequence pairs.
    std::string UnParseEscapes(const std::string& str);
}
}