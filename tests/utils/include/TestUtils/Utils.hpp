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
    auto const isalnum_lambda = [](auto const& ch) { return std::isalnum(ch); };
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
    for (auto i = 0; i < num_tokens; ++i)
    {
        auto attribute_name = string_value.substr(i * length_token, length_token);
        result.emplace_back(attribute_name);
    }
    return result;
}
