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

#include <ncFile.h>
#include <ncVar.h>
#include <ncDim.h>

#include <UGrid/Network1D.hpp>
#include <UGrid/Operations.hpp>

using ugrid::Network1D;

void Network1D::define(ugridapi::Network1d const& network1d)
{
    if (network1d.name == nullptr)
    {
        throw std::invalid_argument("Network1D::define mesh name field is empty");
    }
    if (network1d.node_x == nullptr || network1d.node_y == nullptr)
    {
        throw std::invalid_argument("Network1D::define network node coordinates missing");
    }
    if (network1d.edge_node == nullptr)
    {
        throw std::invalid_argument("Network1D::define network edges (branches) missing");
    }
    if (network1d.geometry_nodes_x == nullptr || network1d.geometry_nodes_y == nullptr)
    {
        throw std::invalid_argument("Network1D::define network geometry coordinates missing");
    }

    UGridEntity::define(network1d.name, network1d.start_index, "Topology data of 1D network", 1, network1d.is_spherical);
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);

    if (network1d.num_nodes > 0)
    {
        string_builder.clear(); string_builder << "_nNodes";
        m_dimensions.insert({ UGridDimensions::nodes, m_nc_file->addDim(string_builder.str(), network1d.num_nodes) });
        auto topology_attribute = m_topology_variable.putAtt("node_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);

        define_topological_variable_with_coordinates(UGridEntityLocations::nodes, UGridDimensions::nodes, "%s of network nodes");

        string_builder.clear(); string_builder << "_node_id";
        topology_attribute = m_topology_variable.putAtt("node_id", string_builder.str());
        add_topology_attribute(topology_attribute);
        auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, { m_dimensions[UGridDimensions::nodes], m_dimensions[UGridDimensions::ids] });
        auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "ID of network nodes");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);


        string_builder.clear(); string_builder << "_node_long_name";
        topology_attribute = m_topology_variable.putAtt("node_long_name", string_builder.str());
        add_topology_attribute(topology_attribute);
        topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, { m_dimensions[UGridDimensions::nodes], m_dimensions[UGridDimensions::long_names] });
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Long name of network nodes");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
    }

    if (network1d.num_edges > 0)
    {
        string_builder.clear(); string_builder << "_nEdges";
        m_dimensions.insert({ UGridDimensions::edges, m_nc_file->addDim(string_builder.str(), network1d.num_edges) });
        auto topology_attribute = m_topology_variable.putAtt("edge_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);

        string_builder.clear(); string_builder << "_edge_nodes";
        topology_attribute = m_topology_variable.putAtt("edge_node_connectivity", string_builder.str());
        add_topology_attribute(topology_attribute);
        auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::edges], m_dimensions[UGridDimensions::Two] });
        auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Start and end nodes of network edges");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        string_builder.clear(); string_builder << "_edge_length";
        topology_attribute = m_topology_variable.putAtt("edge_length", string_builder.str());
        add_topology_attribute(topology_attribute);
        topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, { m_dimensions[UGridDimensions::edges] });
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Real length of branch geometries");
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("units", "m");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        // Other branch variables
        string_builder.clear(); string_builder << "_branch_order";
        auto topology_related_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::edges] });
        auto topology_related_variable_attribute = topology_related_variable.putAtt("long_name", "Order of branches for interpolation");
        topology_related_variable_attribute = topology_related_variable.putAtt("mesh", m_entity_name);
        topology_related_variable_attribute = topology_related_variable.putAtt("location", "edge");
        m_related_variables.insert({ "edge_order", {topology_related_variable} });

        string_builder.clear(); string_builder << "_branch_type";
        topology_related_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::edges] });
        topology_related_variable_attribute = topology_related_variable.putAtt("long_name", "Type of branches");
        topology_related_variable_attribute = topology_related_variable.putAtt("mesh", m_entity_name);
        topology_related_variable_attribute = topology_related_variable.putAtt("location", "edge");
        m_related_variables.insert({ "branch_type", {topology_related_variable} });

        string_builder.clear(); string_builder << "_branch_id";
        topology_attribute = m_topology_variable.putAtt("branch_id", string_builder.str());
        add_topology_attribute(topology_attribute);
        topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, { m_dimensions[UGridDimensions::edges],m_dimensions[UGridDimensions::ids] });
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Long name of branch geometries");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        string_builder.clear(); string_builder << "_branch_long_name";
        topology_attribute = m_topology_variable.putAtt("branch_long_name", string_builder.str());
        add_topology_attribute(topology_attribute);
        topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, { m_dimensions[UGridDimensions::edges],m_dimensions[UGridDimensions::long_names] });
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Long name of branch geometries");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
    }

    if (network1d.num_geometry_nodes > 0)
    {
        string_builder.clear(); string_builder << "_nGeometryNodes";
        m_dimensions.insert({ UGridDimensions::nodes_geometry, m_nc_file->addDim(string_builder.str(), network1d.num_geometry_nodes) });

        string_builder.clear(); string_builder << "_geometry";
        auto topology_attribute = m_topology_variable.putAtt("edge_geometry", string_builder.str());
        add_topology_attribute(topology_attribute);
        auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::nodes_geometry] });
        auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("geometry_type", "line");
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "1D Geometry");
        string_builder.clear(); string_builder << "_geom_node_count";
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("node_count", string_builder.str());
        string_builder.clear(); string_builder << "_geom_x " << m_entity_name << " _geom_y";
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("node_coordinates", string_builder.str());
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        string_builder.clear(); string_builder << "_geom_node_count";
        auto topology_related_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::edges] });
        auto topology_related_variable_attribute = topology_related_variable.putAtt("node_count", string_builder.str());
        topology_related_variable_attribute = topology_related_variable.putAtt("long_name", "Number of geometry nodes per branch");
        m_related_variables.insert({ string_builder.str(), {topology_related_variable} });

        // TODO: this could be included into define_topological_variable_with_coordinates
        string_builder.clear(); string_builder << "_geom_x";
        auto node_geometry_x = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, { m_dimensions[UGridDimensions::nodes_geometry] });
        topology_related_variable_attribute = node_geometry_x.putAtt("long_name", "x-coordinate of branch geometry nodes");
        m_related_variables.insert({ string_builder.str(), {node_geometry_x} });

        string_builder.clear(); string_builder << "_geom_y";
        auto node_geometry_y = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, { m_dimensions[UGridDimensions::nodes_geometry] });
        topology_related_variable_attribute = node_geometry_y.putAtt("long_name", "y-coordinate of branch geometry nodes");
        m_related_variables.insert({ string_builder.str(), {node_geometry_y} });

        m_network_geometry_attribute_variables.insert({ "node_coordinates",{ node_geometry_x ,node_geometry_y } });
    }

    m_nc_file->enddef();
}

void Network1D::put(ugridapi::Network1d const& network1d)
{
    if (network1d.name == nullptr)
    {
        throw std::invalid_argument("Network1D::put invalid mesh name");
    }

    // Nodes
    if (network1d.node_x != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(0).putVar(network1d.node_x);
    }
    if (network1d.node_y != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(0).putVar(network1d.node_y);
    }
    if (network1d.node_name_id != nullptr)
    {
        auto const map_iterator = find_variable_with_aliases("node_id");
        map_iterator->second.at(0).putVar(network1d.node_name_id);
    }
    if (network1d.node_name_long != nullptr)
    {
        auto const map_iterator = find_variable_with_aliases("node_long_name");
        map_iterator->second.at(0).putVar(network1d.node_name_long);
    }

    if (network1d.edge_node != nullptr)
    {
        m_topology_attribute_variables.at("edge_node_connectivity").at(0).putVar(network1d.edge_node);
    }

    if (network1d.edge_length != nullptr)
    {
        m_topology_attribute_variables.at("edge_length").at(0).putVar(network1d.edge_length);
    }

    if (network1d.edge_order != nullptr)
    {
        m_related_variables.at("edge_order").putVar(network1d.edge_order);
    }
    if (network1d.edge_name_id != nullptr)
    {
        auto const map_iterator = find_variable_with_aliases("branch_id");
        map_iterator->second.at(0).putVar(network1d.edge_name_id);
    }

    if (network1d.edge_name_long != nullptr)
    {
        auto const map_iterator = find_variable_with_aliases("branch_long_name");
        map_iterator->second.at(0).putVar(network1d.edge_name_long);
    }

    if (network1d.geometry_nodes_x != nullptr)
    {
        m_network_geometry_attribute_variables.at("node_coordinates").at(0).putVar(network1d.geometry_nodes_x);
    }

    if (network1d.geometry_nodes_y != nullptr)
    {
        m_network_geometry_attribute_variables.at("node_coordinates").at(1).putVar(network1d.geometry_nodes_y);
    }
}

void Network1D::inquire(ugridapi::Network1d& network1d) const
{
    if (m_dimensions.find(UGridDimensions::nodes) != m_dimensions.end())
    {
        network1d.num_nodes = m_dimensions.at(UGridDimensions::nodes).getSize();
    }
    if (m_dimensions.find(UGridDimensions::edges) != m_dimensions.end())
    {
        network1d.num_edges = m_dimensions.at(UGridDimensions::edges).getSize();
    }
    // get network dimensions
    auto const it = m_network_geometry_attribute_variables.find("node_coordinates");
    if (it != m_network_geometry_attribute_variables.end())
    {
        network1d.num_geometry_nodes = it->second.at(0).getDim(0).getSize();;
    }
}

void Network1D::get(ugridapi::Network1d& network1d) const
{

    fill_char_array_with_string_values(network1d.name, m_entity_name);

    if (network1d.node_x != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(0).getVar(network1d.node_x);
    }

    if (network1d.node_y != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(1).getVar(network1d.node_y);
    }

    if (network1d.edge_node != nullptr)
    {
        m_topology_attribute_variables.at("edge_node_connectivity").at(0).getVar(network1d.edge_node);
    }

    if (network1d.node_name_id != nullptr)
    {
        auto const map_iterator = find_variable_with_aliases("node_id");
        map_iterator->second.at(0).getVar(network1d.node_name_id);
    }

    if (network1d.node_name_long != nullptr)
    {
        auto const map_iterator = find_variable_with_aliases("node_long_name");
        map_iterator->second.at(0).getVar(network1d.node_name_long);
    }

    if (network1d.edge_name_id != nullptr)
    {
        auto const map_iterator = find_variable_with_aliases("branch_id");
        map_iterator->second.at(0).getVar(network1d.edge_name_id);
    }

    if (network1d.edge_name_long != nullptr)
    {
        auto const map_iterator = find_variable_with_aliases("branch_long_name");
        map_iterator->second.at(0).getVar(network1d.edge_name_long);
    }

    if (network1d.edge_length != nullptr)
    {
        m_topology_attribute_variables.at("edge_length").at(0).getVar(network1d.edge_length);
    }

    // Network geometry
    if (network1d.geometry_nodes_x != nullptr)
    {
        m_network_geometry_attribute_variables.at("node_coordinates").at(0).getVar(network1d.geometry_nodes_x);
    }

    if (network1d.geometry_nodes_y != nullptr)
    {
        m_network_geometry_attribute_variables.at("node_coordinates").at(1).getVar(network1d.geometry_nodes_y);
    }
}