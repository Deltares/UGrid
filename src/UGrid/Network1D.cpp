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

void Network1D::Define(ugridapi::Network1d const& network1d)
{
    if (network1d.name == nullptr)
    {
        throw std::invalid_argument("Network1D::Define mesh name field is empty");
    }
    m_spherical_coordinates = network1d.is_spherical == 0 ? false : true;
    m_start_index = network1d.start_index;

    // add and define the topology variable
    m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_CHAR);
    m_topology_attributes.insert({ "cf_role", m_topology_variable.putAtt("cf_role", "mesh_topology") });
    m_topology_attributes.insert({ "long_name",m_topology_variable.putAtt("long_name", "Topology data of 2D mesh") });
    m_topology_attributes.insert({ "topology_dimension", m_topology_variable.putAtt("topology_dimension", netCDF::NcType::nc_INT, 2) });

    // add dimension Two 
    m_dimensions.insert({ UGridDimensions::Two, m_nc_file->addDim("Two", 2) });

    // string builder with the entity name as first part of the string
    UGridVarAttributeStringBuilder string_builder(m_entity_name);

    m_nc_file->enddef();
}

void Network1D::Put(ugridapi::Network1d const& network1d)
{
    if (network1d.name == nullptr)
    {
        throw std::invalid_argument("Network1D::Put invalid mesh name");
    }

}

void Network1D::Inquire(ugridapi::Network1d& network1d) const
{
    if (m_dimensions.find(UGridDimensions::nodes) != m_dimensions.end())
    {
        network1d.num_nodes = m_dimensions.at(UGridDimensions::nodes).getSize();
    }
    if (m_dimensions.find(UGridDimensions::edges) != m_dimensions.end())
    {
        network1d.num_edges = m_dimensions.at(UGridDimensions::edges).getSize();
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



void Network1D::Get(ugridapi::Network1d& network1d) const
{
    if (network1d.name != nullptr)
    {
        for (auto i = 0; i < m_entity_name.size(); ++i)
        {
            network1d.name[i] = m_entity_name[i];
        }
        network1d.name[m_entity_name.size()] = '\0';
    }

    if (network1d.node_x != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(0).getVar(network1d.node_x);
    }

    if (network1d.node_y != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(1).getVar(network1d.node_y);
    }

    if (network1d.edge_nodes != nullptr)
    {
        m_topology_attribute_variables.at("edge_node_connectivity").at(0).getVar(network1d.edge_nodes);
    }

    if (network1d.node_ids != nullptr)
    {
        m_topology_attribute_variables.at("node_ids").at(0).getVar(network1d.node_ids);
    }

    if (network1d.node_long_names != nullptr)
    {
        m_topology_attribute_variables.at("node_long_names").at(0).getVar(network1d.node_long_names);
    }

    if (network1d.branch_ids != nullptr)
    {
        m_topology_attribute_variables.at("branch_ids").at(0).getVar(network1d.branch_ids);
    }

    if (network1d.branch_long_names != nullptr)
    {
        m_topology_attribute_variables.at("branch_long_names").at(0).getVar(network1d.branch_long_names);
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

std::vector<Network1D> Network1D::Create(std::shared_ptr<netCDF::NcFile> const& nc_file)
{
    // Get all vars in this file
    const auto file_variables = nc_file->getVars();
    const auto file_dimensions = nc_file->getDims();

    std::vector<Network1D> result;
    for (auto const& variable : file_variables)
    {
        auto variable_attributes = variable.second.getAtts();

        if (!is_network1d_topology_variable(variable_attributes))
        {
            continue;
        }

        int dimensionality;
        variable_attributes["topology_dimension"].getValues(&dimensionality);

        if (dimensionality == m_dimensionality)
        {
            // entity_attribute_keys, entity_attribute_values, entity_dimensions
            auto const [entity_attribute_variables, entity_attribute_names, entity_dimensions] = GetUGridEntity(variable.second, file_dimensions, file_variables);
            // find the network geometry
            auto const entity_attribute_strings_iterator = entity_attribute_names.find("edge_geometry");
            if (entity_attribute_strings_iterator == entity_attribute_names.end())
            {
                throw std::invalid_argument("Network1D::Create " + entity_attribute_strings_iterator->first + " attribute in" + variable.first + " can not be found");
            }
            auto const edge_geometry_variable_name = entity_attribute_strings_iterator->second.front();
            auto const edge_geometry_variable_iterator = file_variables.find(edge_geometry_variable_name);
            if (edge_geometry_variable_iterator == file_variables.end())
            {
                throw std::invalid_argument("Network1D::Create " + edge_geometry_variable_name + " variable can not be found");
            }

            auto const network_geometry_variable = edge_geometry_variable_iterator->second;
            auto const [edge_geometry_attribute_variables, edge_geometry_attribute_names, edge_geometry_entity_dimensions] =
                GetUGridEntity(network_geometry_variable, file_dimensions, file_variables);

            result.emplace_back(nc_file,
                variable.second,
                entity_attribute_variables,
                entity_attribute_names,
                entity_dimensions,
                network_geometry_variable,
                edge_geometry_attribute_variables,
                edge_geometry_attribute_names,
                edge_geometry_entity_dimensions);

        }
    }

    return result;
};
