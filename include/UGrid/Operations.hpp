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
#include <sstream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

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
    static bool IsEqual(T value, T referenceValue)
    {
        return std::abs(value - referenceValue) < std::numeric_limits<T>::epsilon();
    }


    class UGridVarAttributeStringBuilder
    {
        std::stringstream m_os;
        std::string m_name;
    public:
        UGridVarAttributeStringBuilder(std::string const& name) : m_name(name)
        {
            m_os << m_name;
        }

        void clear()
        {
            m_os.str(std::string());
            m_os << m_name;
        }

        [[nodiscard]] std::string str() const
        {
            return m_os.str();
        }

        template<typename T>
        UGridVarAttributeStringBuilder& operator<<(T val)
        {
            m_os << val;
            return *this;
        }
    };

    static bool is_mesh_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes)
    {
        if (attributes.find("cf_role") == attributes.end())
        {
            return false;
        }

        if (attributes.find("topology_dimension") == attributes.end())
        {
            return false;
        }
        return true;
    }

    static bool is_network1d_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes)
    {
        if (attributes.find("cf_role") == attributes.end())
        {
            return false;
        }

        if (attributes.find("edge_geometry") == attributes.end())
        {
            return false;
        }
        return true;
    }


    static bool FillUGridEntityDimensions(std::multimap<std::string, netCDF::NcDim> const& dimensions,
        std::string const& attribute_key_string,
        std::string const& attribute_value_string,
        std::map<UGridDimensions, netCDF::NcDim>& entity_dimensions)
    {
        auto const substring_dimension_pos = attribute_value_string.find("_dimension");
        bool isDimensionVariable = false;
        if (attribute_value_string.find("_dimension") != std::string::npos)
        {
            isDimensionVariable = true;
            std::string location_name = attribute_value_string.substr(0, substring_dimension_pos);
            auto const it = dimensions.find(attribute_key_string);
            if (location_name == "node")
            {
                entity_dimensions.insert({ UGridDimensions::nodes, it->second });
            }
            if (location_name == "edge")
            {
                entity_dimensions.insert({ UGridDimensions::edges, it->second });
            }
            if (location_name == "face")
            {
                entity_dimensions.insert({ UGridDimensions::faces, it->second });
            }
            if (location_name == "max_face_nodes")
            {
                entity_dimensions.insert({ UGridDimensions::max_face_nodes, it->second });
            }
        }
        return isDimensionVariable;
    }

    static std::tuple<std::map<std::string, std::vector<netCDF::NcVar>>,
        std::map < std::string, std::vector<std::string>>,
        std::map<UGridDimensions, netCDF::NcDim>> GetUGridEntity
        (
            std::multimap<std::string, netCDF::NcDim> const& dimensions,
            std::map<std::string, netCDF::NcVarAtt> const& topology_variable,
            std::multimap<std::string, netCDF::NcVar> const& variables
        )
    {
        std::map<std::string, std::vector<netCDF::NcVar>> entity_attribute_keys;
        std::map<std::string, std::vector<std::string>> entity_attribute_values;
        std::map<UGridDimensions, netCDF::NcDim> entity_dimensions;
        for (const auto& attribute : topology_variable)
        {
            if (attribute.second.getType() != netCDF::NcType::nc_CHAR)
            {
                continue;
            }

            std::string attribute_key_string = attribute.first;
            std::string attribute_value_string;
            attribute.second.getValues(attribute_value_string);

            // check if it is a dimension variable
            auto const isDimensionVariable = FillUGridEntityDimensions(dimensions, attribute_value_string, attribute_key_string, entity_dimensions);
            if (isDimensionVariable)
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
                const auto it = variables.find(token);
                if (it != variables.end())
                {
                    valid_attribute_variables.emplace_back(it->second);
                }
            }
            // valid variables have been found
            if (!valid_attribute_variables.empty())
            {
                entity_attribute_keys.insert({ attribute_key_string, valid_attribute_variables });
            }

            entity_attribute_values.insert({ attribute_key_string, attribute_value_string_tokens });

        }
        return { entity_attribute_keys, entity_attribute_values, entity_dimensions };
    }

    static void AddStartIndex(int const& start_index, netCDF::NcVar& variable)
    {
        if (start_index != 0)
        {
            variable.putAtt("start_index", netCDF::NcType::nc_INT, start_index);
        }
    }


} // namespace ugrid
