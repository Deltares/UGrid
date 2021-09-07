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

#include <UGrid/Mesh1D.hpp>
#include <UGrid/Operations.hpp>
#include <UGridApi/Mesh1D.hpp>

using ugrid::Mesh1D;


Mesh1D::Mesh1D(std::shared_ptr<netCDF::NcFile> nc_file) : UGridEntity(nc_file) {};

Mesh1D::Mesh1D(
    std::shared_ptr<netCDF::NcFile> nc_file,
    netCDF::NcVar const& topology_variable,
    std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
    std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
    std::map<UGridDimensions, netCDF::NcDim> const& entity_dimensions
)
    : UGridEntity(nc_file, topology_variable, entity_attributes, entity_attribute_names, entity_dimensions)
{
};

// ug_create_1d_mesh_v2
// ug_def_mesh_ids
void Mesh1D::define(ugridapi::Mesh1D const& mesh1d)
{
    if (mesh1d.name == nullptr)
    {
        throw std::invalid_argument("Mesh1D::define mesh1d name field is empty");
    }
    if (mesh1d.network_name == nullptr)
    {
        throw std::invalid_argument("Mesh1D::define mesh1d network name is empty");
    }
    if (mesh1d.branch_id == nullptr)
    {
        throw std::invalid_argument("Mesh1D::define mesh1d branch id vector is empty");
    }
    if (mesh1d.branch_offset == nullptr)
    {
        throw std::invalid_argument("Mesh1D::define mesh1d branch offset vector is empty");
    }

    UGridEntity::define(mesh1d.name, mesh1d.start_index, "Topology data of 1D mesh", 1, mesh1d.is_spherical);
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);

    std::string network_name(mesh1d.network_name);
    auto topology_attribute = m_topology_variable.putAtt("coordinate_space", network_name);

    if (mesh1d.num_nodes > 0)
    {
        string_builder.clear(); string_builder << "_nNodes";
        m_dimensions.insert({ UGridDimensions::nodes, m_nc_file->addDim(string_builder.str(), mesh1d.num_nodes) });
        topology_attribute = m_topology_variable.putAtt("node_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);

        string_builder.clear(); string_builder << "_node_branch " << m_entity_name << "_node_offset";
        topology_attribute = m_topology_variable.putAtt("node_coordinates", string_builder.str());
        add_topology_attribute(topology_attribute);

        string_builder.clear(); string_builder << "_node_branch";
        auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, m_dimensions[UGridDimensions::nodes]);
        auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Index of branch on which mesh nodes are located");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        string_builder.clear(); string_builder << "_node_offset";
        topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, m_dimensions[UGridDimensions::nodes]);
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Offset along branch of mesh nodes");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        // add node x an y if present, TODO: consider lat/lon 
        bool const add_coordinate_variables = mesh1d.node_x != nullptr && mesh1d.node_y != nullptr;
        if (add_coordinate_variables)
        {
            define_topological_variable_with_coordinates(UGridEntityLocations::nodes, UGridDimensions::nodes, "%s of mesh nodes");
        }

        string_builder.clear(); string_builder << "_node_id";
        topology_attribute = m_topology_variable.putAtt("node_name_id", string_builder.str());
        add_topology_attribute(topology_attribute);
        topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, { m_dimensions[UGridDimensions::nodes], m_dimensions[UGridDimensions::ids] });
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "ID of mesh nodes");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);


        string_builder.clear(); string_builder << "_node_long_name";
        topology_attribute = m_topology_variable.putAtt("node_name_long", string_builder.str());
        add_topology_attribute(topology_attribute);
        topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, { m_dimensions[UGridDimensions::nodes], m_dimensions[UGridDimensions::long_names] });
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "'Long name of mesh nodes");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
    }

    if (mesh1d.num_edges > 0)
    {
        string_builder.clear(); string_builder << "_nEdges";
        m_dimensions.insert({ UGridDimensions::edges, m_nc_file->addDim(string_builder.str(), mesh1d.num_edges) });
        topology_attribute = m_topology_variable.putAtt("edge_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);
        m_topology_attributes.insert({ topology_attribute.getName(), topology_attribute });


        string_builder.clear(); string_builder << "_edge_nodes";
        topology_attribute = m_topology_variable.putAtt("edge_node_connectivity", string_builder.str());
        add_topology_attribute(topology_attribute);
        auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::edges], m_dimensions[UGridDimensions::Two] });
        auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Maps every edge to the two nodes that it connects");
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        if (mesh1d.edge_edge_offset != nullptr && mesh1d.edge_edge_id)
        {
            string_builder.clear(); string_builder << "_edge_branch " << m_entity_name << "_edge_offset";
            topology_attribute = m_topology_variable.putAtt("edge_coordinates", string_builder.str());
            add_topology_attribute(topology_attribute);

            string_builder.clear(); string_builder << "_edge_branch";
            topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, m_dimensions[UGridDimensions::edges]);
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Index of branch on which mesh edges are located");
            add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

            string_builder.clear(); string_builder << "_edge_offset";
            topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, m_dimensions[UGridDimensions::edges]);
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Offset along branch of mesh edges");
            add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
        }

        bool const add_coordinate_variables = mesh1d.edge_x != nullptr && mesh1d.edge_y != nullptr;
        if (add_coordinate_variables)
        {
            define_topological_variable_with_coordinates(UGridEntityLocations::edges, UGridDimensions::edges, "%s of mesh edges");
        }
    }

    m_nc_file->enddef();
}

void Mesh1D::put(ugridapi::Mesh1D const& mesh1d)
{
    if (mesh1d.name == nullptr)
    {
        throw std::invalid_argument("Mesh1D::put invalid mesh name");
    }
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh1d.branch_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh1d.branch_id);
    }
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh1d.branch_offset != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).putVar(mesh1d.branch_offset);
    }
    if (auto const it = m_topology_attribute_variables.find("node_name_id"); mesh1d.node_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh1d.node_name_id);
    }
    if (auto const it = m_topology_attribute_variables.find("node_name_long"); mesh1d.node_name_long != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh1d.node_name_long);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); mesh1d.edge_node != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh1d.edge_node);
    }
}

void Mesh1D::inquire(ugridapi::Mesh1D& mesh1d) const
{
    if (m_dimensions.find(UGridDimensions::nodes) != m_dimensions.end())
    {
        mesh1d.num_nodes = m_dimensions.at(UGridDimensions::nodes).getSize();
    }
    if (m_dimensions.find(UGridDimensions::edges) != m_dimensions.end())
    {
        mesh1d.num_edges = m_dimensions.at(UGridDimensions::edges).getSize();
    }
}

void Mesh1D::get(ugridapi::Mesh1D& mesh1d) const
{
    fill_char_array_with_string_values(mesh1d.name, m_entity_name);

    auto const network_name = m_topology_attributes_names.at("coordinate_space").at(0);
    fill_char_array_with_string_values(mesh1d.network_name, network_name);

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh1d.branch_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh1d.branch_id);
    }

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh1d.branch_offset != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).getVar(mesh1d.branch_offset);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); mesh1d.edge_node != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh1d.edge_node);
    }
    if (auto const it = find_variable_with_aliases("node_id"); mesh1d.node_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh1d.node_name_id);
    }
    if (auto const it = find_variable_with_aliases("node_long_name"); mesh1d.node_name_long != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh1d.node_name_long);
    }
}
