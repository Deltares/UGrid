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

#include "Constants.hpp"

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

    static std::tuple<std::map<std::string, std::vector<netCDF::NcVar>>, std::map < std::string, std::vector<std::string>>> GetAttributesNames(
        std::map<std::string, netCDF::NcVarAtt> const& topology_variable,
        std::multimap<std::string, netCDF::NcVar> const& variables)
    {
        std::map<std::string, std::vector<netCDF::NcVar>> attribute_variables;
        std::map<std::string, std::vector<std::string>> attribute_variable_names;
        for (const auto& attribute : topology_variable)
        {
            if (attribute.second.getType() != netCDF::NcType::nc_CHAR)
            {
                continue;
            }
            std::string name;
            attribute.second.getValues(name);
            std::vector<std::string> tokens;
            split(tokens, name, boost::is_any_of(" "));
            std::vector<std::string> valid_variable_names;
            std::vector<netCDF::NcVar> valid_attribute_variables;
            for (auto const& token : tokens)
            {
                // a name of a valid attribute has been found
                const auto variable_iterator = variables.find(token);
                if (variable_iterator != variables.end())
                {
                    valid_attribute_variables.emplace_back(variable_iterator->second);
                }
            }
            // valid variables have been found
            if (!valid_attribute_variables.empty())
            {
                attribute_variables.insert({ attribute.first, valid_attribute_variables });
            }

            attribute_variable_names.insert({ attribute.first, tokens });

        }
        return { attribute_variables , attribute_variable_names };
    }

    static void AddStartIndex(int const& start_index, netCDF::NcVar& variable)
    {
        if (start_index != 0)
        {
            variable.putAtt("start_index", netCDF::NcType::nc_INT, start_index);
        }
    }

    static void AddFillValue(double const& fill_value, netCDF::NcVar& variable)
    {
        if (fill_value != double_invalid_value)
        {
            variable.putAtt("_FillValue", netCDF::NcType::nc_DOUBLE, fill_value);
        }
    }

    static void AddFillValue(int const& fill_value, netCDF::NcVar& variable)
    {
        if (fill_value != int_invalid_value)
        {
            variable.putAtt("_FillValue", netCDF::NcType::nc_INT, fill_value);
        }
    }


} // namespace ugrid
