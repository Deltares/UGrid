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

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include <UGrid/Constants.hpp>

// required header
#include <ncDim.h>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    /// @brief Generic function determining if two values are equal
    ///
    /// This is especially useful for floating point values.
    template <typename T>
    static bool is_equal(T value, T referenceValue)
    {
        return std::abs(value - referenceValue) < std::numeric_limits<T>::epsilon();
    }

    static UGridFileDimensions from_dimension_string_to_dimension_enum(std::string const& dimension_string)
    {
        if (dimension_string == "node" || dimension_string == "nodes")
        {
            return UGridFileDimensions::node;
        }
        if (dimension_string == "edge" || dimension_string == "edges")
        {
            return UGridFileDimensions::edge;
        }
        if (dimension_string == "face" || dimension_string == "faces")
        {
            return UGridFileDimensions::face;
        }
        if (dimension_string == "max_face_node" || dimension_string == "max_face_nodes")
        {
            return UGridFileDimensions::max_face_node;
        }
        throw std::invalid_argument("from_dimension_string_to_dimension_enum: Dimension not found.");
    }

    static UGridEntityLocations from_location_string_to_location(std::string const& location_string)
    {
        if (location_string == "node")
        {
            return node;
        }
        if (location_string == "edge")
        {
            return edge;
        }
        if (location_string == "face")
        {
            return face;
        }
    }

    static std::string from_location_integer_to_location_string(int location)
    {
        auto const ug_entity_location = static_cast<UGridEntityLocations>(location);

        if (ug_entity_location == node)
        {
            return "node";
        }
        if (ug_entity_location == edge)
        {
            return "edge";
        }
        if (ug_entity_location == face)
        {
            return "face";
        }
        if (ug_entity_location == layer)
        {
            return "layer";
        }
        if (ug_entity_location == layer_interface)
        {
            return "layer_interface";
        }
        if (ug_entity_location == vertical)
        {
            return "vertical";
        }
    }

    static bool fill_ugrid_entity_dimension(std::multimap<std::string, netCDF::NcDim> const& dimensions,
                                            std::string const& attribute_key_string,
                                            std::string const& attribute_value_string,
                                            std::map<UGridFileDimensions, netCDF::NcDim>& entity_dimensions)
    {
        bool isDimensionVariable = false;
        auto const it = dimensions.find(attribute_key_string);
        if (it == dimensions.end())
        {
            return isDimensionVariable;
        }

        if (attribute_value_string.find("_dimension") != std::string::npos)
        {
            isDimensionVariable = true;
            auto const substring_dimension_pos = attribute_value_string.find("_dimension");
            std::string const location_name = attribute_value_string.substr(0, substring_dimension_pos);
            auto const dimension_enum = from_dimension_string_to_dimension_enum(location_name);
            entity_dimensions.insert({dimension_enum, it->second});
        }
        return isDimensionVariable;
    }

    static std::tuple<std::map<std::string, std::vector<netCDF::NcVar>>,
                      std::map<std::string, std::vector<std::string>>,
                      std::map<UGridFileDimensions, netCDF::NcDim>>
    get_ugrid_entity(netCDF::NcVar const& variable,
                     std::multimap<std::string, netCDF::NcDim> const& file_dimensions,
                     std::multimap<std::string, netCDF::NcVar> const& file_variables)
    {
        std::map<std::string, std::vector<netCDF::NcVar>> entity_attribute_variables;
        std::map<std::string, std::vector<std::string>> entity_attribute_names;
        std::map<UGridFileDimensions, netCDF::NcDim> entity_dimensions;
        const auto variable_attributes = variable.getAtts();
        for (const auto& attribute : variable_attributes)
        {
            if (attribute.second.getType() != netCDF::NcType::nc_CHAR)
            {
                continue;
            }

            std::string attribute_key_string = attribute.first;
            std::string attribute_value_string;
            attribute.second.getValues(attribute_value_string);

            // check if it is a dimension variable
            auto const is_dimension_variable = fill_ugrid_entity_dimension(file_dimensions,
                                                                           attribute_value_string,
                                                                           attribute_key_string,
                                                                           entity_dimensions);
            if (is_dimension_variable)
            {
                continue;
            }

            std::vector<std::string> attribute_value_string_tokens;
            split(attribute_value_string_tokens, attribute_value_string, boost::is_any_of(" "));
            std::vector<std::string> valid_variable_names;
            std::vector<netCDF::NcVar> valid_attribute_variables;
            for (auto const& token : attribute_value_string_tokens)
            {
                // a name of a valid attribute has been found
                const auto it = file_variables.find(token);
                if (it != file_variables.end())
                {
                    valid_attribute_variables.emplace_back(it->second);
                }
            }
            // valid file_variables have been found
            if (!valid_attribute_variables.empty())
            {
                entity_attribute_variables.insert({attribute_key_string, valid_attribute_variables});
            }
            entity_attribute_names.insert({attribute_key_string, attribute_value_string_tokens});
        }
        return {entity_attribute_variables, entity_attribute_names, entity_dimensions};
    }

    static void string_to_char_array(std::string const& value, size_t len, char* char_array)
    {
        if (char_array == nullptr || value.empty())
        {
            return;
        }
        for (auto i = 0; i < value.size(); ++i)
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

    static void vector_of_strings_to_char_array(std::vector<std::string> const& values, size_t len, char* char_array)
    {
        if (char_array == nullptr || values.empty())
        {
            return;
        }

        for (auto i = 0; i < values.size(); ++i)
        {
            size_t char_array_position = i * len;
            for (auto j = 0; j < values[i].size(); ++j)
            {
                char_array[char_array_position + j] = values[i][j];
            }
            for (auto j = values[i].size(); j < len - 1; ++j)
            {
                char_array[char_array_position + j] = ' ';
            }
        }

        // null terminate
        char_array[len * values.size() - 1] = '\0';
    }

    static void right_trim_string(std::string& str)
    {
        auto const isalnum_lambda = [](auto const& ch) { return std::isalnum(ch); };
        str.erase(std::find_if(str.rbegin(), str.rend(), isalnum_lambda).base(), str.end());
    }

    static std::string char_array_to_string(char const* char_array, size_t len)
    {
        std::string result(" ", len);
        if (char_array == nullptr)
        {
            return result;
        }
        for (auto i = 0; i < len; ++i)
        {
            result[i] = char_array[i];
        }
        right_trim_string(result);
        return result;
    }

} // namespace ugrid
