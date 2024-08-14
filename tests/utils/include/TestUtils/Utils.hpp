//---- GPL ---------------------------------------------------------------------
//
// Copyright (C)  Stichting Deltares, 2011-2021.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 3.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// contact: delft3d.support@deltares.nl
// Stichting Deltares
// P.O. Box 177
// 2600 MH Delft, The Netherlands
//
// All indications and logos of, and references to, "Delft3D" and "Deltares"
// are registered trademarks of Stichting Deltares, and remain the property of
// Stichting Deltares. All rights reserved.
//
//------------------------------------------------------------------------------

#pragma once

/// @brief Performs right trim of single string
/// @param str [in] The input string
static void right_trim_string(std::string& str)
{
    auto const isalnum_lambda = [](auto const& ch)
    {
        return std::isalnum(ch);
    };
    str.erase(std::find_if(str.rbegin(), str.rend(), isalnum_lambda).base(), str.end());
}

/// @brief Right trims all entries of a vector of strings
/// @param str_vector [in] The input vector
static void right_trim_string_vector(std::vector<std::string>& str_vector)
{
    for (auto& str : str_vector)
    {
        right_trim_string(str);
    }
}

/// @brief Splits a string in equal sized tokens
/// @param string_value [in] The input string
/// @param num_tokens [in] The number of tokens
/// @param length_token [in] The length of the token
/// @return The resulting vector containing each token
static std::vector<std::string> split_string(std::string string_value, size_t num_tokens, size_t length_token)
{
    std::vector<std::string> result;
    for (size_t i = 0; i < num_tokens; ++i)
    {
        auto attribute_name = string_value.substr(i * length_token, length_token);
        result.emplace_back(attribute_name);
    }
    return result;
}

/// @brief Puts the content of a string into a char array and adds null termination
/// @param value [in] The input string
/// @param len [in] The array length
/// @param char_array [out] The output char array
static void string_to_char_array(std::string const& value, size_t len, char* char_array)
{
    if (value.empty())
    {
        throw std::runtime_error("string_to_char_array: value string is empty.");
    }
    if (char_array == nullptr)
    {
        throw std::runtime_error("string_to_char_array: char_array is nullptr.");
    }
    for (size_t i = 0; i < value.size(); ++i)
    {
        char_array[i] = value[i];
    }
    for (auto i = value.size(); i < len - 1; ++i)
    {
        char_array[i] = ' ';
    }
    // null terminate
    char_array[len - 1] = '\0';
}

/// @brief Puts the content of a vector of strings into a char array and adds null termination
/// @param value [in] The input string
/// @param len [in] The length of each entry in char_array
/// @param char_array [out] The output char array
static void vector_of_strings_to_char_array(std::vector<std::string> const& values, size_t len, char* char_array)
{
    if (char_array == nullptr || values.empty())
    {
        return;
    }

    size_t char_array_position = 0;
    for (size_t i = 0; i < values.size(); ++i)
    {
        for (size_t j = 0; j < values[i].size(); ++j)
        {
            char_array[char_array_position] = values[i][j];
            char_array_position++;
        }
        for (size_t j = values[i].size(); j < len; ++j)
        {
            char_array[char_array_position] = ' ';
            char_array_position++;
        }
    }

    // null terminate
    char_array[char_array_position - 1] = '\0';
}