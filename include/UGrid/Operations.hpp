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
#include "netcdf.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
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

    static std::map<std::string, netCDF::NcVar> FillMappedVariables(
        std::map<std::string, netCDF::NcVarAtt> const& attributes,
        std::multimap<std::string, netCDF::NcVar> const& file_vars)
    {
        std::map<std::string, netCDF::NcVar> result;
        for (const auto& attribute : attributes)
        {
            if (attribute.second.getType() != netCDF::NcType::nc_CHAR)
            {
                continue;
            }
            std::string name;
            attribute.second.getValues(name);
            std::vector<std::string> variable_names;
            split(variable_names, name, boost::is_any_of(" "));
            for (auto const& variable_name : variable_names)
            {
                const auto variable_iterator = file_vars.find(variable_name);
                if (variable_iterator == file_vars.end())
                {
                    continue;
                }
                auto const variable = file_vars.find(variable_name)->second;
                result.insert({ variable_name, variable });

            }
        }
        return result;
    }

} // namespace ugrid
