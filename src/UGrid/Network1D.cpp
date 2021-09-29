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

#include <ncDim.h>
#include <ncFile.h>
#include <ncVar.h>

#include <UGrid/Network1D.hpp>
#include <UGrid/Operations.hpp>
#include <UGrid/UGridVarAttributeStringBuilder.hpp>

using ugrid::Network1D;

Network1D::Network1D(std::shared_ptr<netCDF::NcFile> nc_file) : UGridEntity(nc_file) {}

Network1D::Network1D(std::shared_ptr<netCDF::NcFile> nc_file,
                     netCDF::NcVar const& topology_variable,
                     std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
                     std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
                     std::map<UGridFileDimensions, netCDF::NcDim> const& entity_dimensions) : UGridEntity(nc_file, topology_variable, entity_attributes, entity_attribute_names, entity_dimensions)

{
    // file variables and file dimensions
    auto const file_variables = nc_file->getVars();
    auto const file_dimensions = nc_file->getDims();

    // find the network geometry
    auto const entity_attribute_strings_iterator = entity_attribute_names.find("edge_geometry");
    if (entity_attribute_strings_iterator == entity_attribute_names.end())
    {
        throw std::invalid_argument("Network1D::create " + entity_attribute_strings_iterator->first + " attribute in" + m_topology_variable.getName() + " can not be found");
    }
    auto const edge_geometry_variable_name = entity_attribute_strings_iterator->second.front();
    auto const edge_geometry_variable_iterator = file_variables.find(edge_geometry_variable_name);
    if (edge_geometry_variable_iterator == file_variables.end())
    {
        throw std::invalid_argument("Network1D::create " + edge_geometry_variable_name + " variable can not be found");
    }

    auto const network_geometry_variable = edge_geometry_variable_iterator->second;
    auto const [edge_geometry_attribute_variables, edge_geometry_attribute_names, edge_geometry_entity_dimensions] =
        get_ugrid_entity(network_geometry_variable, file_dimensions, file_variables);

    m_network_geometry_variable = network_geometry_variable;
    m_network_geometry_attribute_variables = edge_geometry_attribute_variables;
    m_network_geometry_attributes_names = edge_geometry_attribute_names;
    m_network_geometry_dimensions = edge_geometry_entity_dimensions;
}

void Network1D::define(ugridapi::Network1D const& network1d)
{
    if (network1d.name == nullptr)
    {
        throw std::invalid_argument("Network1D::define mesh name field is empty");
    }
    if (network1d.node_x == nullptr || network1d.node_y == nullptr)
    {
        throw std::invalid_argument("Network1D::define network node coordinates missing");
    }
    if (network1d.branch_node == nullptr)
    {
        throw std::invalid_argument("Network1D::define network edges (branches) missing");
    }
    if (network1d.geometry_nodes_x == nullptr || network1d.geometry_nodes_y == nullptr)
    {
        throw std::invalid_argument("Network1D::define network geometry coordinates missing");
    }

    UGridEntity::define(network1d.name, network1d.start_index, "Topology data of 1D network", 1, network1d.is_spherical);
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);

    // node variables
    if (network1d.num_nodes > 0)
    {
        // Define dimensions
        string_builder.clear();
        string_builder << "_nNodes";
        m_dimensions.insert({UGridFileDimensions::nodes, m_nc_file->addDim(string_builder.str(), network1d.num_nodes)});

        // Define node_dimension topology attribute
        define_topological_attribute("node_dimension", string_builder.str());

        // Define node coordinates variable
        define_topology_coordinates(UGridFileDimensions::nodes, "%s of network nodes");

        // Define node_id topological attribute and variable
        define_topological_attribute("node_id");
        define_topological_variable("node_id",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::nodes, UGridFileDimensions::ids},
                                    {{"long_name", "ID of network nodes"}});

        // Define node_long_name topological attribute and variable
        define_topological_attribute("node_long_name");
        define_topological_variable("node_long_name",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::nodes, UGridFileDimensions::long_names},
                                    {{"long_name", "ID of network nodes"}});
    }

    // branch variables
    if (network1d.num_branches > 0)
    {
        // Define dimensions
        string_builder.clear();
        string_builder << "_nEdges";
        m_dimensions.insert({UGridFileDimensions::edges, m_nc_file->addDim(string_builder.str(), network1d.num_branches)});

        // Define edge_dimension topology attribute
        define_topological_attribute("edge_dimension", string_builder.str());

        // Define edge_nodes topology attribute and variable
        string_builder.clear();
        string_builder << "_edge_nodes";
        define_topological_attribute("edge_node_connectivity", string_builder.str());
        define_topological_variable("edge_nodes",
                                    netCDF::NcType::nc_INT,
                                    {UGridFileDimensions::edges, UGridFileDimensions::Two},
                                    {{"cf_role", "edge_node_connectivity"},
                                     {"long_name", "Start and end nodes of network edges"}});

        // Define edge_lengths topology attribute and variable
        define_topological_attribute("branch_length");
        define_topological_variable("branch_length",
                                    netCDF::NcType::nc_DOUBLE,
                                    {UGridFileDimensions::edges},
                                    {{"long_name", "Real length of branch geometries"}});

        // Other topology-related variables
        define_topology_related_variables("branch_order",
                                          netCDF::NcType::nc_INT,
                                          {UGridFileDimensions::edges},
                                          {{"long_name", "Order of branches for interpolation"},
                                           {"mesh", m_entity_name},
                                           {"location", "edge"}});

        define_topology_related_variables("branch_type",
                                          netCDF::NcType::nc_INT,
                                          {UGridFileDimensions::edges},
                                          {{"long_name", "Type of branches"},
                                           {"mesh", m_entity_name},
                                           {"location", "edge"}});

        // Define branch_id topology attribute and variable
        define_topological_attribute("branch_id");
        define_topological_variable("branch_id",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::edges, UGridFileDimensions::ids},
                                    {{"long_name", "ID of branch geometries"}});

        // Define branch_long_name topology attribute and variable
        define_topological_attribute("branch_long_name");
        define_topological_variable("branch_long_name",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::edges, UGridFileDimensions::long_names},
                                    {{"long_name", "Long name of branch geometries"}});
    }

    // Geometry nodes
    if (network1d.num_geometry_nodes > 0)
    {
        // Define dimension
        string_builder.clear();
        string_builder << "_nGeometryNodes";
        auto const node_geometry_dimension = string_builder.str();
        m_dimensions.insert({UGridFileDimensions::nodes_geometry, m_nc_file->addDim(node_geometry_dimension, network1d.num_geometry_nodes)});

        // Define edge_geometry attribute
        string_builder.clear();
        string_builder << "_geometry";
        define_topological_attribute("edge_geometry", string_builder.str());

        // Define edge_geometry variable, with edge_geometry attributes
        string_builder.clear();
        string_builder << "_geom_part_node_count";
        auto const geom_node_node_count = string_builder.str();

        string_builder.clear();
        string_builder << "_geom_x " << m_entity_name << "_geom_y";
        auto const geometry_coordinate_var_name = string_builder.str();

        define_topological_variable("geometry",
                                    netCDF::NcType::nc_INT,
                                    {},
                                    {
                                        {"geometry_type", "multiline"},
                                        {"long_name", "1D Geometry"},
                                        {"node_count", node_geometry_dimension},
                                        {"part_node_count", geom_node_node_count},
                                        {"node_coordinates", geometry_coordinate_var_name},
                                    });

        // Define edge_geometry attribute variables
        define_topology_related_variables("geom_part_node_count",
                                          netCDF::NcType::nc_INT,
                                          {UGridFileDimensions::edges},
                                          {{"node_count", string_builder.str()},
                                           {"long_name", "Number of geometry nodes per branch"}});

        define_topology_related_variables("geom_x",
                                          netCDF::NcType::nc_DOUBLE,
                                          {UGridFileDimensions::nodes_geometry},
                                          {{"long_name", "x-coordinate of branch geometry nodes"}});

        define_topology_related_variables("geom_y",
                                          netCDF::NcType::nc_DOUBLE,
                                          {UGridFileDimensions::nodes_geometry},
                                          {{"long_name", "y-coordinate of branch geometry nodes"}});

        m_network_geometry_attribute_variables.insert({"part_node_count", {m_related_variables.at("geom_part_node_count")}});
        m_network_geometry_attribute_variables.insert({"node_coordinates", {m_related_variables.at("geom_x"), m_related_variables.at("geom_y")}});
    }

    m_nc_file->enddef();
}

void Network1D::put(ugridapi::Network1D const& network1d)
{
    if (network1d.name == nullptr)
    {
        throw std::invalid_argument("Network1D::put invalid mesh name");
    }

    if (auto const it = m_topology_attribute_variables.find("node_y"); network1d.node_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.node_x);
    }
    if (auto const it = m_topology_attribute_variables.find("node_x"); network1d.node_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.node_y);
    }
    if (auto const it = find_variable_name_with_aliases("node_id"); network1d.node_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.node_name_id);
    }

    if (auto const it = find_variable_name_with_aliases("node_long_name"); network1d.node_name_long != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.node_name_long);
    }

    if (auto const it = m_topology_attribute_variables.find("edge_nodes"); network1d.branch_node != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.branch_node);
    }

    if (auto const it = m_topology_attribute_variables.find("branch_length"); network1d.branch_length != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.branch_length);
    }

    if (auto const it = m_related_variables.find("branch_order"); network1d.branch_order != nullptr && it != m_related_variables.end())
    {
        it->second.putVar(network1d.branch_order);
    }

    if (auto const it = find_variable_name_with_aliases("branch_id"); network1d.branch_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.branch_name_id);
    }

    if (auto const it = find_variable_name_with_aliases("branch_long_name"); network1d.branch_name_long != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.branch_name_long);
    }

    if (auto const it = m_network_geometry_attribute_variables.find("node_coordinates"); network1d.geometry_nodes_x != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.geometry_nodes_x);
    }

    if (auto const it = m_network_geometry_attribute_variables.find("node_coordinates"); network1d.geometry_nodes_y != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(1).putVar(network1d.geometry_nodes_y);
    }

    if (auto const it = m_network_geometry_attribute_variables.find("part_node_count"); network1d.geometry_nodes_count != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.geometry_nodes_count);
    }
}

void Network1D::inquire(ugridapi::Network1D& network1d) const
{
    if (m_dimensions.find(UGridFileDimensions::nodes) != m_dimensions.end())
    {
        network1d.num_nodes = m_dimensions.at(UGridFileDimensions::nodes).getSize();
    }
    if (m_dimensions.find(UGridFileDimensions::edges) != m_dimensions.end())
    {
        network1d.num_branches = m_dimensions.at(UGridFileDimensions::edges).getSize();
    }
    // get network dimensions
    if (auto const it = m_network_geometry_attribute_variables.find("node_coordinates"); it != m_network_geometry_attribute_variables.end())
    {
        network1d.num_geometry_nodes = it->second.at(0).getDim(0).getSize();
    }
}

void Network1D::get(ugridapi::Network1D& network1d) const
{

    string_to_char_array(network1d.name, m_entity_name, name_length);

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); network1d.node_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.node_x);
    }

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); network1d.node_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).getVar(network1d.node_y);
    }

    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); network1d.branch_node != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.branch_node);
    }

    if (auto const it = find_variable_name_with_aliases("node_id"); network1d.node_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.node_name_id);
    }

    if (auto const it = find_variable_name_with_aliases("node_long_name"); network1d.node_name_long != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.node_name_long);
    }

    if (auto const it = find_variable_name_with_aliases("branch_id"); network1d.branch_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.branch_name_id);
    }

    if (auto const it = find_variable_name_with_aliases("branch_long_name"); network1d.branch_name_long != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.branch_name_long);
    }

    if (auto const it = find_variable_name_with_aliases("branch_length"); network1d.branch_length != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.branch_length);
    }

    // Network geometry
    if (auto const it = m_network_geometry_attribute_variables.find("node_coordinates"); network1d.geometry_nodes_x != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.geometry_nodes_x);
    }

    if (auto const it = m_network_geometry_attribute_variables.find("node_coordinates"); network1d.geometry_nodes_y != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(1).getVar(network1d.geometry_nodes_y);
    }
    if (auto const it = m_network_geometry_attribute_variables.find("part_node_count"); network1d.geometry_nodes_count != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.geometry_nodes_count);
    }
}

bool Network1D::is_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes)
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