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

#include <map>
#include <string>
#include <vector>

#include <ncFile.h>
#include <ncVar.h>

#include <UGrid/Constants.hpp>
#include <UGrid/Operations.hpp>
#include <UGrid/UGridEntity.hpp>
#include <UGrid/UGridVarAttributeStringBuilder.hpp>

using namespace ugrid;

UGridEntity::UGridEntity(const std::shared_ptr<netCDF::NcFile>& nc_file)
    : m_nc_file(nc_file)
{
}

UGridEntity::UGridEntity(
    std::shared_ptr<netCDF::NcFile> nc_file,
    netCDF::NcVar const& topology_variable,
    std::map<std::string, std::vector<netCDF::NcVar>> const& attribute_variables,
    std::map<std::string, std::vector<std::string>> const& attribute_variable_names,
    std::map<UGridFileDimensions, netCDF::NcDim> const& dimensions)
    : m_nc_file(nc_file),
      m_topology_variable(topology_variable),
      m_topology_attribute_variables(attribute_variables),
      m_topology_attributes_names(attribute_variable_names),
      m_dimensions(dimensions)
{
    m_entity_name = m_topology_variable.getName();
}

bool UGridEntity::is_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes)
{

    if (attributes.find("cf_role") == attributes.end())
    {
        return false;
    }

    if (attributes.find("topology_dimension") == attributes.end())
    {
        return false;
    }

    // Only network1d has edge_geometry
    if (attributes.find("edge_geometry") != attributes.end())
    {
        return false;
    }
    return true;
}

bool UGridEntity::has_matching_dimensionality(std::map<std::string, netCDF::NcVarAtt> const& attributes, int entity_dimensionality)
{
    auto const it = attributes.find("topology_dimension");
    if (it == attributes.end())
    {
        return false;
    }

    int dimensionality;
    attributes.at("topology_dimension").getValues(&dimensionality);
    if (dimensionality == entity_dimensionality)
    {
        return true;
    }

    return false;
}

std::string UGridEntity::get_location_attribute_value(std::string const& location)
{
    UGridVarAttributeStringBuilder string_builder(m_entity_name);
    if (!m_spherical_coordinates)
    {
        string_builder.clear();
        string_builder << "_" << location << "_x " << m_entity_name << "_" << location << "_y";
    }

    if (m_spherical_coordinates)
    {
        string_builder.clear();
        string_builder << "_" << location << "_lon " << m_entity_name << "_" << location << "_lat";
    }

    return string_builder.str();
}

std::tuple<std::string,
           std::string,
           std::string,
           std::string,
           std::string,
           std::string,
           std::string,
           std::string>
UGridEntity::get_location_variable_names(std::string const& location, std::string const& long_name_pattern, std::string const& name_pattern)
{
    std::string location_coordinate_x;
    std::string location_coordinate_y;
    std::string standard_name_x;
    std::string standard_name_y;
    std::string long_name_x;
    std::string long_name_y;
    std::string units_x;
    std::string units_y;

    std::string topology_attribute_name = location + "_coordinates";
    UGridVarAttributeStringBuilder string_builder(m_entity_name);

    if (!m_spherical_coordinates)
    {
        string_builder.clear();
        string_builder << "_" << boost::format(name_pattern) % location % "_x";
        location_coordinate_x = string_builder.str();

        string_builder.clear();
        string_builder << "_" << boost::format(name_pattern) % location % "_y";
        location_coordinate_y = string_builder.str();

        boost::format formatter = boost::format(long_name_pattern) % "x-coordinate";
        long_name_x = formatter.str();

        formatter = boost::format(long_name_pattern) % "y-coordinate";
        long_name_y = formatter.str();

        standard_name_x = "projection_x_coordinate";
        standard_name_y = "projection_y_coordinate";

        units_x = "m";
        units_y = "m";
    }
    if (m_spherical_coordinates)
    {
        string_builder.clear();
        string_builder << "_" << boost::format(name_pattern) % location % "_lon";
        location_coordinate_x = string_builder.str();

        string_builder.clear();
        string_builder << "_" << boost::format(name_pattern) % location % "_lat";
        location_coordinate_y = string_builder.str();

        boost::format formatter = boost::format(long_name_pattern) % "latitude coordinate";
        long_name_x = formatter.str();

        formatter = boost::format(long_name_pattern) % "longitude coordinate";
        long_name_y = formatter.str();

        standard_name_x = "latitude";
        standard_name_y = "longitude";

        units_x = "degrees_north";
        units_y = "degrees_east";
    }

    return {location_coordinate_x, location_coordinate_y, standard_name_x, standard_name_y, long_name_x, long_name_y, units_x, units_y};
}

void UGridEntity::define_topology_coordinates(UGridFileDimensions dimension, std::string const& long_name_pattern, std::string const& name_pattern)
{
    std::string dimension_string;
    if (dimension == UGridFileDimensions::nodes)
    {
        dimension_string = "node";
    }
    if (dimension == UGridFileDimensions::edges)
    {
        dimension_string = "edge";
    }
    if (dimension == UGridFileDimensions::faces)
    {
        dimension_string = "face";
    }

    auto [location_coordinate_x,
          location_coordinate_y,
          standard_name_x,
          standard_name_y,
          long_name_x,
          long_name_y,
          units_x,
          units_y] = get_location_variable_names(dimension_string, long_name_pattern, name_pattern);

    auto const attribute_value = location_coordinate_x + " " + location_coordinate_y;

    define_topological_attribute(dimension_string + "_coordinates", attribute_value);

    define_topological_variable(location_coordinate_x,
                                netCDF::NcType::nc_DOUBLE,
                                {dimension},
                                {{"units", units_x},
                                 {"standard_name", standard_name_x},
                                 {"long_name", long_name_x}});

    define_topological_variable(location_coordinate_y,
                                netCDF::NcType::nc_DOUBLE,
                                {dimension},
                                {{"units", units_y},
                                 {"standard_name", standard_name_y},
                                 {"long_name", long_name_y}});
}

std::map<std::string, std::vector<netCDF::NcVar>>::const_iterator UGridEntity::find_variable_name_with_aliases(std::string const& variable_name) const
{
    // define topology variable aliases
    static std::map<std::string, std::vector<std::string>> aliases{
        {"node_id", {"node_id", "node_ids"}},
        {"node_long_name", {"node_long_name", "node_long_names"}},
        {"branch_id", {"branch_id", "branch_ids"}},
        {"branch_long_name", {"branch_long_name", "branch_long_names"}},
        {"contact_id", {"contact_id", "contact_ids"}},
        {"contact_long_name", {"contact_long_name", "contact_long_names"}},
        {"branch_length", {"branch_length", "branch_lengths"}}};

    auto iterator = m_topology_attribute_variables.end();
    for (auto const& alias : aliases.at(variable_name))
    {
        iterator = m_topology_attribute_variables.find(alias);
        if (iterator != m_topology_attribute_variables.end())
        {
            break;
        }
    }
    if (iterator == m_topology_attribute_variables.end())
    {
        throw std::invalid_argument("find_variable_name_with_aliases: No Matching found");
    }

    return iterator;
}

void UGridEntity::define_topological_attribute(std::string const& attribute_name, std::string const& attribute_value)
{
    netCDF::NcVarAtt topology_attribute;
    if (attribute_value.empty())
    {
        auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);
        string_builder << "_" << attribute_name;
        topology_attribute = m_topology_variable.putAtt(attribute_name, string_builder.str());
    }
    else
    {
        topology_attribute = m_topology_variable.putAtt(attribute_name, attribute_value);
    }

    m_topology_attributes.insert({attribute_name, topology_attribute});
}

void UGridEntity::define_topological_variable(std::string const& variable_suffix,
                                              netCDF::NcType nc_type,
                                              std::vector<UGridFileDimensions> const& ugridfile_dimensions,
                                              std::vector<std::pair<std::string, std::string>> const& attributes)
{
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);
    string_builder << "_" << variable_suffix;

    // create dimensions vector
    std::vector<netCDF::NcDim> dimensions;
    for (auto const d : ugridfile_dimensions)
    {
        dimensions.emplace_back(m_dimensions[d]);
    }

    // create topology variable
    auto const topology_attribute_variable = m_nc_file->addVar(string_builder.str(), nc_type, dimensions);

    for (auto const& attribute : attributes)
    {
        topology_attribute_variable.putAtt(attribute.first, attribute.second);
    }

    // add start index if necessary
    if (m_start_index != 0)
    {
        topology_attribute_variable.putAtt("start_index", netCDF::NcType::nc_INT, m_start_index);
    }

    // add fill value
    nc_type == netCDF::NcType::nc_DOUBLE ? topology_attribute_variable.setFill(true, m_double_fill_value) : topology_attribute_variable.setFill(true, m_int_fill_value);

    // find if an attribute variable_suffix is already stored, otherwise fill it
    auto topology_attribute_iterator = m_topology_attribute_variables.find(variable_suffix);
    if (topology_attribute_iterator != m_topology_attribute_variables.end())
    {
        topology_attribute_iterator->second.emplace_back(topology_attribute_variable);
    }
    else
    {
        m_topology_attribute_variables.insert({variable_suffix, {topology_attribute_variable}});
    }
}

void UGridEntity::define_topology_related_variables(std::string const& variable,
                                                    netCDF::NcType nc_type,
                                                    std::vector<UGridFileDimensions> const& ugridfile_dimensions,
                                                    std::vector<std::pair<std::string, std::string>> const& attributes)
{
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);
    string_builder << variable;

    // create dimensions vector
    std::vector<netCDF::NcDim> dimensions;
    for (auto const d : ugridfile_dimensions)
    {
        dimensions.emplace_back(m_dimensions[d]);
    }

    const auto topology_related_variable = m_nc_file->addVar(string_builder.str(), nc_type, dimensions);

    for (auto const& attribute : attributes)
    {
        topology_related_variable.putAtt(attribute.first, attribute.second);
    }

    m_related_variables.insert({variable, {topology_related_variable}});
}

void UGridEntity::define(char* entity_name, int start_index, std::string const& long_name, int topology_dimension, int is_spherical)
{
    m_start_index = start_index;
    m_entity_name = char_array_to_string(entity_name, name_length);
    m_spherical_coordinates = is_spherical == 0 ? false : true;

    // Topology name
    m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_CHAR);

    // Topology attributes
    define_topological_attribute("cf_role", "mesh_topology");
    define_topological_attribute("long_name", long_name);
    auto topology_attribute = m_topology_variable.putAtt("topology_dimension", netCDF::NcType::nc_INT, topology_dimension);
    m_topology_attributes.insert({topology_attribute.getName(), topology_attribute});

    // Define additional dimensions, maybe required later
    m_dimensions.insert({UGridFileDimensions::ids, m_nc_file->addDim(name_length_dimension, name_length)});
    m_dimensions.insert({UGridFileDimensions::long_names, m_nc_file->addDim(name_long_length_dimension, name_long_length)});
    m_dimensions.insert({UGridFileDimensions::Two, m_nc_file->addDim(two_string, 2)});
}