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
#include <vector>
#include <map>

#include <ncFile.h>
#include <UGrid/Operations.hpp>
#include <UGrid/UGridEntity.hpp>

#include "Constants.hpp"

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    /// @brief A class containing the ids of UGrid netcdf file
    struct UGridVariable
    {


        static netCDF::NcVar define_node_variables(std::string const& variable_name,
            UGridEntity const& ugrid_entity,
            UGridEntityLocations const& ugrid_entity_location,
            std::string const& standard_name,
            std::string const& long_name,
            std::string const& units,
            double const& double_fill_value = double_invalid_value)
        {
            define_node_variables(variable_name, ugrid_entity, ugrid_entity_location, netCDF::NcType::nc_DOUBLE, standard_name, long_name, units, int_invalid_value, double_fill_value);
        }

        static netCDF::NcVar define_node_variables(std::string const& variable_name,
            UGridEntity const& ugrid_entity,
            UGridEntityLocations const& ugrid_entity_location,
            std::string const& standard_name,
            std::string const& long_name,
            std::string const& units,
            int const& int_fill_value = int_invalid_value)
        {
            define_node_variables(variable_name, ugrid_entity, ugrid_entity_location, netCDF::NcType::nc_INT, standard_name, long_name, units, int_fill_value, double_invalid_value);
        }

        static netCDF::NcVar define_node_variables(std::string const& variable_name,
            UGridEntity const& ugrid_entity,
            UGridEntityLocations const& ugrid_entity_location,
            netCDF::NcType const& nc_type,
            std::string const& standard_name,
            std::string const& long_name,
            std::string const& units,
            int const& int_fill_value = int_invalid_value,
            double const& double_fill_value = double_invalid_value)
        {
            UGridVarAttributeStringBuilder string_builder(ugrid_entity.m_entity_name);

            string_builder << variable_name;
            netCDF::NcVar variable = ugrid_entity.m_nc_file->addVar(string_builder.str(), nc_type, ugrid_entity.m_dimensions.at(ugrid_entity_location));

            std::string location;
            if (ugrid_entity_location == UGridEntityLocations::nodes)
            {
                location = "node";
            }
            if (ugrid_entity_location == UGridEntityLocations::edges)
            {
                location = "edges";

            }
            if (ugrid_entity_location == UGridEntityLocations::faces)
            {
                location = "faces";
            }
            if (ugrid_entity_location == UGridEntityLocations::contact)
            {
                location = "contact";
                string_builder.clear(); string_builder << "_" << location;
                variable.putAtt("location", string_builder.str());
                variable.putAtt("coordinates", location);
                define_additional_attributes(variable, standard_name, long_name, units, int_fill_value, double_fill_value);
                return variable;
            }

            if (!ugrid_entity.m_spherical_coordinates)
            {
                string_builder.clear(); string_builder << "_" << location << "_x " << ugrid_entity.m_entity_name << "_" << location << "_y";
                variable.putAtt("location", string_builder.str());
                variable.putAtt("coordinates", location);
            }

            if (ugrid_entity.m_spherical_coordinates)
            {
                string_builder.clear(); string_builder << "_" << location << "_lon " << ugrid_entity.m_entity_name << "_" << location << "_lat";
                variable.putAtt("location", string_builder.str());
                variable.putAtt("coordinates", location);
            }

            define_additional_attributes(variable, standard_name, long_name, units, int_fill_value, double_fill_value);

            return variable;
        }

        static void define_additional_attributes(netCDF::NcVar& variable,
            std::string const& standard_name,
            std::string const& long_name,
            std::string const& units,
            int const& int_fill_value = int_invalid_value,
            double const& double_fill_value = double_invalid_value)
        {

            if (!standard_name.empty())
            {
                variable.putAtt("standard_name", standard_name);
            }

            if (!long_name.empty())
            {
                variable.putAtt("long_name", long_name);
            }

            if (!units.empty())
            {
                variable.putAtt("units", units);
            }

            if (IsEqual(int_fill_value, int_invalid_value))
            {
                variable.putAtt("_FillValue", netCDF::NcType::nc_INT, int_fill_value);
            }

            if (IsEqual(double_fill_value, double_invalid_value))
            {
                variable.putAtt("_FillValue", netCDF::NcType::nc_DOUBLE, double_fill_value);
            }

        }
    };
} // namespace ugrid
