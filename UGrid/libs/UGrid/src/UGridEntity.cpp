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

#include <format>

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
      m_topology_attributes_variables_values(attribute_variable_names),
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

std::vector<std::pair<std::string, std::string>> UGridEntity::create_coordinate_variable_attributes(
    std::string const& units,
    std::string const& standard_name,
    std::string const& long_name)
{
    std::vector<std::pair<std::string, std::string>> attributes = {{"units", units},
                                                                   {"standard_name", standard_name},
                                                                   {"long_name", long_name}};
    if (!m_grid_mapping.empty())
    {
        attributes.emplace_back("grid_mapping", m_grid_mapping);
    }
    return attributes;
}

std::vector<std::pair<std::string, std::string>> UGridEntity::create_data_variable_attributes(
    std::string const& standard_name,
    std::string const& long_name,
    std::string const& units,
    UGridEntityLocations location)
{
    std::string const location_str = from_location_to_location_string(location);

    UGridVarAttributeStringBuilder string_builder(m_entity_name);
    string_builder << "_" << location_str << "_x " << m_entity_name << "_" << location_str << "_y";
    std::string const coordinates = string_builder.str();

    std::vector<std::pair<std::string, std::string>> attributes = {{"mesh", m_entity_name},
                                                                   {"standard_name", standard_name},
                                                                   {"long_name", long_name},
                                                                   {"units", units},
                                                                   {"coordinates", coordinates},
                                                                   {"location", location_str}};
    if (!m_grid_mapping.empty())
    {
        attributes.emplace_back("grid_mapping", m_grid_mapping);
    }
    return attributes;
}

std::vector<std::string> UGridEntity::get_data_variables_names(std::string const& location_string)
{
    auto const variables = m_nc_file->getVars();

    std::string const mesh_attribute_name = "mesh";
    std::string const location_attribute_name = "location";
    std::vector<std::string> variable_names;
    for (auto const& v : variables)
    {
        auto const variable_attributes = v.second.getAtts();
        auto const mesh_it = variable_attributes.find(mesh_attribute_name);
        auto const location_it = variable_attributes.find(location_attribute_name);

        if (mesh_it == variable_attributes.end() || location_it == variable_attributes.end())
        {
            continue;
        }

        std::string variable_location_type;
        location_it->second.getValues(variable_location_type);
        std::string variable_mesh_name;
        mesh_it->second.getValues(variable_mesh_name);

        if (variable_mesh_name == m_entity_name && variable_location_type == location_string)
        {
            variable_names.emplace_back(v.first);
        }
    }
    return variable_names;
}

std::tuple<std::string,
           std::string,
           std::string,
           std::string,
           std::string,
           std::string,
           std::string,
           std::string>
UGridEntity::get_location_variable_names(UGridEntityLocations location, std::string const& long_name_pattern, std::string const& name_pattern)
{
    std::string standard_name_x;
    std::string standard_name_y;
    std::string long_name_x;
    std::string long_name_y;
    std::string units_x;
    std::string units_y;

    std::string const location_str = from_location_to_location_string(location);
    std::string const location_coordinate_x = std::vformat(name_pattern, std::make_format_args(location_str, "_x"));
    std::string const location_coordinate_y = std::vformat(name_pattern, std::make_format_args(location_str, "_y"));

    if (!m_spherical_coordinates)
    {
        long_name_x = std::vformat(long_name_pattern, std::make_format_args("x-coordinate"));
        long_name_y = std::vformat(long_name_pattern, std::make_format_args("y-coordinate"));

        standard_name_x = "projection_x_coordinate";
        standard_name_y = "projection_y_coordinate";

        units_x = "m";
        units_y = "m";
    }
    if (m_spherical_coordinates)
    {
        long_name_x = std::vformat(long_name_pattern, std::make_format_args("longitude coordinate"));
        long_name_y = std::vformat(long_name_pattern, std::make_format_args("latitude coordinate"));

        standard_name_x = "longitude";
        standard_name_y = "latitude";

        units_x = "degrees_east";
        units_y = "degrees_north";
    }

    return {location_coordinate_x, location_coordinate_y, standard_name_x, standard_name_y, long_name_x, long_name_y, units_x, units_y};
}

void UGridEntity::define_topology_coordinates(UGridEntityLocations location, std::string const& long_name_pattern, std::string const& name_pattern)
{
    auto [location_coordinate_x,
          location_coordinate_y,
          standard_name_x,
          standard_name_y,
          long_name_x,
          long_name_y,
          units_x,
          units_y] = get_location_variable_names(location, long_name_pattern, name_pattern);

    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);
    string_builder << "_" << location_coordinate_x << " " << m_entity_name << "_" << location_coordinate_y;

    std::string const attribute_name = from_location_to_location_string(location) + "_coordinates";
    define_topological_attribute(attribute_name, string_builder.str());

    auto const dimension = from_location_to_dimension(location);
    auto const attributes_x = create_coordinate_variable_attributes(units_x, standard_name_x, long_name_x);
    auto const attributes_y = create_coordinate_variable_attributes(units_y, standard_name_y, long_name_y);

    define_topological_variable(
        attribute_name,
        location_coordinate_x,
        netCDF::NcType::nc_DOUBLE,
        {dimension},
        attributes_x);

    define_topological_variable(
        attribute_name,
        location_coordinate_y,
        netCDF::NcType::nc_DOUBLE,
        {dimension},
        attributes_y);
}

void UGridEntity::define_topology_related_coordinates(
    UGridEntityLocations location,
    std::string const& long_name_pattern,
    std::string const& name_pattern,
    std::string const& topology_attribute_name,
    std::string const& attribute_name)
{
    static std::map<UGridFileDimensions, UGridFileDimensions> const extra_dimension{
        {UGridFileDimensions::edge, UGridFileDimensions::Two},
        {UGridFileDimensions::face, UGridFileDimensions::max_face_node}};

    UGridFileDimensions const dimension = from_location_to_dimension(location);
    std::vector<UGridFileDimensions> dimensions = {dimension};

    if (auto const it = extra_dimension.find(dimension); it != extra_dimension.end())
    {
        dimensions.emplace_back(it->second);
    }

    auto [location_coordinate_x,
          location_coordinate_y,
          standard_name_x,
          standard_name_y,
          long_name_x,
          long_name_y,
          units_x,
          units_y] = get_location_variable_names(location, long_name_pattern, name_pattern);

    if (!topology_attribute_name.empty() && !attribute_name.empty())
    {
        auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);
        string_builder << "_" << location_coordinate_x;
        m_topology_attribute_variables.at(topology_attribute_name).at(0).putAtt(attribute_name, string_builder.str());
    }

    if (!topology_attribute_name.empty() && !attribute_name.empty())
    {
        auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);
        string_builder << "_" << location_coordinate_y;
        m_topology_attribute_variables.at(topology_attribute_name).at(1).putAtt(attribute_name, string_builder.str());
    }

    auto const attributes_x = create_coordinate_variable_attributes(units_x, standard_name_x, long_name_x);
    auto const attributes_y = create_coordinate_variable_attributes(units_y, standard_name_y, long_name_y);

    define_topology_related_variables(
        location_coordinate_x,
        netCDF::NcType::nc_DOUBLE,
        dimensions,
        attributes_x,
        true);

    define_topology_related_variables(
        location_coordinate_y,
        netCDF::NcType::nc_DOUBLE,
        dimensions,
        attributes_y,
        true);
}

std::map<std::string, std::vector<netCDF::NcVar>>::const_iterator UGridEntity::find_attribute_variable_name_with_aliases(std::string const& variable_name) const
{
    auto iterator = m_topology_attribute_variables.end();
    for (auto const& alias : attribute_aliases.at(variable_name))
    {
        iterator = m_topology_attribute_variables.find(alias);
        if (iterator != m_topology_attribute_variables.end())
        {
            break;
        }
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

void UGridEntity::define_topological_variable(std::string const& topology_attribute_name,
                                              std::string const& variable_suffix,
                                              netCDF::NcType nc_type,
                                              std::vector<UGridFileDimensions> const& ugridfile_dimensions,
                                              std::vector<std::pair<std::string, std::string>> const& attributes,
                                              bool add_fill_value)
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

    // create the attributes
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
    if (add_fill_value)
    {
        nc_type == netCDF::NcType::nc_DOUBLE
            ? topology_attribute_variable.setFill(true, m_double_fill_value)
            : topology_attribute_variable.setFill(true, m_int_fill_value);
    }

    // find if an attribute variable_suffix is already stored, otherwise fill it
    auto topology_attribute_iterator = m_topology_attribute_variables.find(topology_attribute_name);
    if (topology_attribute_iterator != m_topology_attribute_variables.end())
    {
        topology_attribute_iterator->second.emplace_back(topology_attribute_variable);
    }
    else
    {
        m_topology_attribute_variables.insert({topology_attribute_name, {topology_attribute_variable}});
    }
}

void UGridEntity::define_topology_related_variables(std::string const& variable,
                                                    netCDF::NcType nc_type,
                                                    std::vector<UGridFileDimensions> const& ugridfile_dimensions,
                                                    std::vector<std::pair<std::string, std::string>> const& attributes,
                                                    bool add_fill_value)
{
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);
    string_builder << "_" << variable;

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

    if (add_fill_value)
    {
        nc_type == netCDF::NcType::nc_DOUBLE
            ? topology_related_variable.setFill(true, m_double_fill_value)
            : topology_related_variable.setFill(true, m_int_fill_value);
    }

    m_related_variables.insert({variable, {topology_related_variable}});
}

void UGridEntity::define(char const* const entity_name,
                         int start_index,
                         std::string const& long_name,
                         int topology_dimension,
                         int is_spherical,
                         char const* const grid_mapping)
{
    m_start_index = start_index;
    m_entity_name = char_array_to_string(entity_name, name_length);
    m_grid_mapping = char_array_to_string(grid_mapping, name_long_length);
    m_spherical_coordinates = is_spherical == 0 ? false : true;

    // Topology name
    m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_CHAR);

    // Topology attributes
    define_topological_attribute("cf_role", "mesh_topology");
    define_topological_attribute("long_name", long_name);
    auto topology_attribute = m_topology_variable.putAtt("topology_dimension", netCDF::NcType::nc_INT, topology_dimension);
    m_topology_attributes.insert({topology_attribute.getName(), topology_attribute});

    // Define additional dimensions, maybe required later
    m_dimensions.insert({UGridFileDimensions::id, m_nc_file->addDim(name_length_dimension, name_length)});
    m_dimensions.insert({UGridFileDimensions::long_name, m_nc_file->addDim(name_long_length_dimension, name_long_length)});
    m_dimensions.insert({UGridFileDimensions::Two, m_nc_file->addDim(two_string, 2)});
}
