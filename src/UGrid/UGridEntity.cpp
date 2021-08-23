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

#include <boost/format.hpp>

#include <string>
#include <vector>
#include <map>

#include <ncFile.h>
#include <ncVar.h>

#include <UGrid/Operations.hpp>
#include <UGrid/Constants.hpp>
#include <UGrid/UGridEntity.hpp>


using namespace ugrid;

UGridEntity::UGridEntity(const std::shared_ptr<netCDF::NcFile>& m_nc_file)
    : m_nc_file(m_nc_file)
{
}

UGridEntity::UGridEntity(
    std::shared_ptr<netCDF::NcFile> const& nc_file,
    std::string const& entity_name,
    std::map<std::string, std::vector<netCDF::NcVar>>& const attribute_variables,
    std::map<std::string, std::vector<std::string>> const& attribute_variable_names)
    :
    m_nc_file(nc_file),
    m_entity_name(entity_name),
    m_topology_attribute_variables(attribute_variables),
    m_topology_attributes_names(attribute_variable_names)
{
}
void UGridEntity::define_variable_with_coordinate(
    std::string const& variable_name_in_topology,
    UGridDimensions const& ugrid_entity_dimension,
    UGridCoordinates const& coordinate,
    std::string const& long_name_pattern)
{
    std::string standard_name;
    std::string units;
    std::string long_name;
    if (m_spherical_coordinates && coordinate == UGridCoordinates::lat)
    {
        boost::format formatter = boost::format(long_name_pattern) % "latitude coordinate";
        long_name = formatter.str();
        units = "degrees_north";
        standard_name = "latitude";
    }

    if (m_spherical_coordinates && coordinate == UGridCoordinates::lon)
    {
        boost::format formatter = boost::format(long_name_pattern) % "longitude coordinate";
        long_name = formatter.str();
        units = "degrees_east";
        standard_name = "longitude";
    }

    if (!m_spherical_coordinates && coordinate == UGridCoordinates::x)
    {
        boost::format formatter = boost::format(long_name_pattern) % "x-coordinate";
        long_name = formatter.str();
        units = "m";
        standard_name = "projection_x_coordinate";
    }

    if (!m_spherical_coordinates && coordinate == UGridCoordinates::y)
    {
        boost::format formatter = boost::format(long_name_pattern) % "y-coordinate";
        long_name = formatter.str();
        units = "m";
        standard_name = "projection_y_coordinate";
    }

    m_topology_attribute_variables[variable_name_in_topology].emplace_back(m_nc_file->addVar(variable_name_in_topology, netCDF::NcType::nc_DOUBLE, m_dimensions[ugrid_entity_dimension]));
    m_topology_attribute_variables[variable_name_in_topology].back().putAtt("mesh", m_entity_name);

    m_topology_attribute_variables[variable_name_in_topology].back().putAtt("units", units);
    m_topology_attribute_variables[variable_name_in_topology].back().putAtt("standard_name", standard_name);
    m_topology_attribute_variables[variable_name_in_topology].back().putAtt("long_name", long_name);

    AddFillValue(m_double_fill_value, m_topology_attribute_variables[variable_name_in_topology].back());
}

void UGridEntity::define_topological_variable_with_coordinates(
    UGridEntityLocations const& location,
    UGridDimensions const& dimension,
    bool add_coordinate_variables,
    std::string const& long_name_pattern,
    std::string const& name_pattern)
{
    UGridVarAttributeStringBuilder string_builder(m_entity_name);

    if (!add_coordinate_variables)
    {
        return;
    }
    boost::format formatter = boost::format(name_pattern);
    std::string location_string;
    if (location == UGridEntityLocations::nodes)
    {
        formatter = formatter % "node";
        location_string = "node";

    }
    if (location == UGridEntityLocations::edges)
    {
        formatter = formatter % "edge";
        location_string = "edge";
    }
    if (location == UGridEntityLocations::faces)
    {
        formatter = formatter % "face";
        location_string = "face";
    }

    std::string const coordinate_name(location_string + "_coordinates");
    std::string first_coordinate_variable;
    std::string second_coordinate_variable;
    if (!m_spherical_coordinates)
    {
        string_builder.clear(); string_builder << "_" << formatter % "x";
        first_coordinate_variable = string_builder.str();

        string_builder.clear(); string_builder << "_" << formatter % "y";
        second_coordinate_variable = string_builder.str();

        define_variable_with_coordinate(first_coordinate_variable, dimension, UGridCoordinates::x, long_name_pattern);
        define_variable_with_coordinate(second_coordinate_variable, dimension, UGridCoordinates::y, long_name_pattern);
    }

    if (m_spherical_coordinates)
    {
        string_builder.clear(); string_builder << "_" << formatter % "lon";
        first_coordinate_variable = string_builder.str();

        string_builder.clear(); string_builder << "_" << formatter % "lat";
        second_coordinate_variable = string_builder.str();

        define_variable_with_coordinate(first_coordinate_variable, dimension, UGridCoordinates::lon, long_name_pattern);
        define_variable_with_coordinate(second_coordinate_variable, dimension, UGridCoordinates::lat, long_name_pattern);
    }

    m_topology_attributes.insert({ location_string + "_coordinates",m_topology_variable.putAtt(location_string + "_coordinates ", first_coordinate_variable + " " + second_coordinate_variable) });

}

void UGridEntity::define_topological_variable_attributes(netCDF::NcVar& variable,
    std::string const& variable_name,
    std::string const& cf_role_attribute,
    std::string const& long_name)
{
    variable.putAtt("cf_role", cf_role_attribute);
    variable.putAtt("long_name", long_name);

    AddFillValue(m_int_fill_value, variable);
    AddStartIndex(m_start_index, variable);

    m_topology_attribute_variables.insert({ variable_name, {variable} });
}

netCDF::NcVar UGridEntity::define_variable_on_location(std::string const& variable_name,
    UGridDimensions const& ugrid_entity_dimension,
    std::string const& standard_name,
    std::string const& long_name,
    std::string const& units,
    double const& fill_value)
{
    return define_variable_on_location(variable_name, ugrid_entity_dimension, netCDF::NcType::nc_DOUBLE, standard_name, long_name, units, int_invalid_value, fill_value);
}

netCDF::NcVar UGridEntity::define_variable_on_location(std::string const& variable_name,
    UGridDimensions const& ugrid_entity_dimension,
    std::string const& standard_name,
    std::string const& long_name,
    std::string const& units,
    int const& fill_value)
{
    return define_variable_on_location(variable_name, ugrid_entity_dimension, netCDF::NcType::nc_INT, standard_name, long_name, units, fill_value, fill_value);
}

netCDF::NcVar UGridEntity::define_variable_on_location(std::string const& variable_name,
    UGridDimensions const& ugrid_entity_dimension,
    netCDF::NcType const& nc_type,
    std::string const& standard_name,
    std::string const& long_name,
    std::string const& units,
    int const& int_fill_value,
    double const& double_fill_value)
{
    UGridVarAttributeStringBuilder string_builder(m_entity_name);

    string_builder << variable_name;
    netCDF::NcVar variable = m_nc_file->addVar(string_builder.str(), nc_type, m_dimensions.at(ugrid_entity_dimension));

    std::string location;
    if (ugrid_entity_dimension == UGridDimensions::nodes)
    {
        location = "node";
    }
    if (ugrid_entity_dimension == UGridDimensions::edges)
    {
        location = "edges";
    }
    if (ugrid_entity_dimension == UGridDimensions::faces)
    {
        location = "faces";
    }
    if (ugrid_entity_dimension == UGridDimensions::contacts)
    {
        location = "contact";
        string_builder.clear(); string_builder << "_" << location;
        variable.putAtt("location", string_builder.str());
        variable.putAtt("coordinates", location);
        define_additional_attributes(variable, standard_name, long_name, units, int_fill_value, double_fill_value);
        return variable;
    }

    if (!m_spherical_coordinates)
    {
        string_builder.clear(); string_builder << "_" << location << "_x " << m_entity_name << "_" << location << "_y";
        variable.putAtt("location", string_builder.str());
        variable.putAtt("coordinates", location);
    }

    if (m_spherical_coordinates)
    {
        string_builder.clear(); string_builder << "_" << location << "_lon " << m_entity_name << "_" << location << "_lat";
        variable.putAtt("location", string_builder.str());
        variable.putAtt("coordinates", location);
    }

    define_additional_attributes(variable, standard_name, long_name, units, int_fill_value, double_fill_value);

    return variable;
}

void UGridEntity::define_additional_attributes(netCDF::NcVar& variable,
    std::string const& standard_name,
    std::string const& long_name,
    std::string const& units,
    int const& int_fill_value,
    double const& double_fill_value)
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

    if (!IsEqual(int_fill_value, int_invalid_value))
    {
        variable.putAtt("_FillValue", netCDF::NcType::nc_INT, int_fill_value);
    }

    if (!IsEqual(double_fill_value, double_invalid_value))
    {
        variable.putAtt("_FillValue", netCDF::NcType::nc_DOUBLE, double_fill_value);
    }
}
