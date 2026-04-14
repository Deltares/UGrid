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
    if (network1d.edge_nodes == nullptr)
    {
        throw std::invalid_argument("Network1D::define network edge (branches) missing");
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
        m_dimensions.insert({UGridFileDimensions::node, m_nc_file->addDim(string_builder.str(), network1d.num_nodes)});

        // Define node_dimension topology attribute
        define_topological_attribute("node_dimension", string_builder.str());

        // Define node coordinates variable
        define_topology_coordinates(UGridFileDimensions::node, "{} of network node");

        // Define node_id topological attribute and variable
        define_topological_attribute("node_id");
        define_topological_variable("node_id",
                                    "node_id",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::node, UGridFileDimensions::id},
                                    {{"long_name", "ID of network node"}});

        // Define node_long_name topological attribute and variable
        define_topological_attribute("node_long_name");
        define_topological_variable("node_long_name",
                                    "node_long_name",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::node, UGridFileDimensions::long_name},
                                    {{"long_name", "ID of network node"}});
    }

    // branch variables
    if (network1d.num_edges > 0)
    {
        // Define dimensions
        string_builder.clear();
        string_builder << "_nEdges";
        m_dimensions.insert({UGridFileDimensions::edge, m_nc_file->addDim(string_builder.str(), network1d.num_edges)});

        // Define edge_dimension topology attribute
        define_topological_attribute("edge_dimension", string_builder.str());

        // Define edge_nodes topology attribute and variable
        string_builder.clear();
        string_builder << "_edge_nodes";
        define_topological_attribute("edge_node_connectivity", string_builder.str());
        define_topological_variable("edge_node_connectivity",
                                    "edge_nodes",
                                    netCDF::NcType::nc_INT,
                                    {UGridFileDimensions::edge, UGridFileDimensions::Two},
                                    {{"cf_role", "edge_node_connectivity"},
                                     {"long_name", "Start and end node of network edge"}});

        // Define edge_lengths topology attribute and variable
        define_topological_attribute("edge_length");
        define_topological_variable("edge_length",
                                    "edge_length",
                                    netCDF::NcType::nc_DOUBLE,
                                    {UGridFileDimensions::edge},
                                    {{"long_name", "Real length of branch geometries"}});

        // Other topology-related variables
        define_topology_related_variables("edge_order",
                                          netCDF::NcType::nc_INT,
                                          {UGridFileDimensions::edge},
                                          {{"long_name", "Order of branches for interpolation"},
                                           {"mesh", m_entity_name},
                                           {"location", "edge"}});

        define_topology_related_variables("edge_type",
                                          netCDF::NcType::nc_INT,
                                          {UGridFileDimensions::edge},
                                          {{"long_name", "Type of branches"},
                                           {"mesh", m_entity_name},
                                           {"location", "edge"}});

        // Define edge_id topology attribute and variable
        define_topological_attribute("edge_id");
        define_topological_variable("edge_id",
                                    "edge_id",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::edge, UGridFileDimensions::id},
                                    {{"long_name", "ID of branch geometries"}});

        // Define edge_long_name topology attribute and variable
        define_topological_attribute("edge_long_name");
        define_topological_variable("edge_long_name",
                                    "edge_long_name",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::edge, UGridFileDimensions::long_name},
                                    {{"long_name", "Long name of branch geometries"}});
    }

    // Geometry node
    if (network1d.num_geometry_nodes > 0)
    {
        // Define dimension
        string_builder.clear();
        string_builder << "_nGeometryNodes";
        auto const node_geometry_dimension = string_builder.str();
        m_dimensions.insert({UGridFileDimensions::node_geometry, m_nc_file->addDim(node_geometry_dimension, network1d.num_geometry_nodes)});

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

        define_topological_variable("edge_geometry",
                                    "geometry",
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
                                          {UGridFileDimensions::edge},
                                          {{"node_count", string_builder.str()},
                                           {"long_name", "Number of geometry node per branch"}});

        define_topology_related_variables("geom_x",
                                          netCDF::NcType::nc_DOUBLE,
                                          {UGridFileDimensions::node_geometry},
                                          {{"long_name", "x-coordinate of branch geometry node"}});

        define_topology_related_variables("geom_y",
                                          netCDF::NcType::nc_DOUBLE,
                                          {UGridFileDimensions::node_geometry},
                                          {{"long_name", "y-coordinate of branch geometry node"}});

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

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); network1d.node_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.node_x);
    }
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); network1d.node_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).putVar(network1d.node_y);
    }
    if (auto const it = find_attribute_variable_name_with_aliases("node_id"); network1d.node_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.node_id);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("node_long_name"); network1d.node_long_name != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.node_long_name);
    }

    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); network1d.edge_nodes != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.edge_nodes);
    }

    if (auto const it = m_topology_attribute_variables.find("edge_length"); network1d.edge_length != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.edge_length);
    }

    if (auto const it = m_related_variables.find("edge_order"); network1d.edge_order != nullptr && it != m_related_variables.end())
    {
        it->second.putVar(network1d.edge_order);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("edge_id"); network1d.edge_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.edge_id);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("edge_long_name"); network1d.edge_long_name != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.edge_long_name);
    }

    if (auto const it = m_network_geometry_attribute_variables.find("node_coordinates"); network1d.geometry_nodes_x != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.geometry_nodes_x);
    }

    if (auto const it = m_network_geometry_attribute_variables.find("node_coordinates"); network1d.geometry_nodes_y != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(1).putVar(network1d.geometry_nodes_y);
    }

    if (auto const it = m_network_geometry_attribute_variables.find("part_node_count"); network1d.num_edge_geometry_nodes != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(0).putVar(network1d.num_edge_geometry_nodes);
    }
}

void Network1D::inquire(ugridapi::Network1D& network1d) const
{
    if (m_dimensions.find(UGridFileDimensions::node) != m_dimensions.end())
    {
        network1d.num_nodes = static_cast<int>(m_dimensions.at(UGridFileDimensions::node).getSize());
    }
    if (m_dimensions.find(UGridFileDimensions::edge) != m_dimensions.end())
    {
        network1d.num_edges = static_cast<int>(m_dimensions.at(UGridFileDimensions::edge).getSize());
    }
    // get network dimensions
    if (auto const it = m_network_geometry_attribute_variables.find("node_coordinates"); it != m_network_geometry_attribute_variables.end())
    {
        network1d.num_geometry_nodes = static_cast<int>(it->second.at(0).getDim(0).getSize());
    }
}

void Network1D::get(ugridapi::Network1D& network1d) const
{

    string_to_char_array(m_entity_name, name_long_length, network1d.name);

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); network1d.node_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.node_x);
    }

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); network1d.node_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).getVar(network1d.node_y);
    }

    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); network1d.edge_nodes != nullptr && it != m_topology_attribute_variables.end())
    {
        const auto var = it->second.at(0);
        var.getVar(network1d.edge_nodes);
        apply_start_index_offset(var, network1d.start_index, network1d.num_edges * 2, network1d.edge_nodes);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("node_id"); network1d.node_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.node_id);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("node_long_name"); network1d.node_long_name != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.node_long_name);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("edge_id"); network1d.edge_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.edge_id);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("edge_long_name"); network1d.edge_long_name != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.edge_long_name);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("edge_length"); network1d.edge_length != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.edge_length);
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
    if (auto const it = m_network_geometry_attribute_variables.find("part_node_count"); network1d.num_edge_geometry_nodes != nullptr && it != m_network_geometry_attribute_variables.end())
    {
        it->second.at(0).getVar(network1d.num_edge_geometry_nodes);
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
