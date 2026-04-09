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

#include <UGrid/Mesh1D.hpp>
#include <UGrid/Operations.hpp>
#include <UGrid/UGridVarAttributeStringBuilder.hpp>

using ugrid::Mesh1D;

Mesh1D::Mesh1D(std::shared_ptr<netCDF::NcFile> nc_file)
    : UGridEntity(nc_file)
{
}

Mesh1D::Mesh1D(
    std::shared_ptr<netCDF::NcFile> nc_file,
    netCDF::NcVar const& topology_variable,
    std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
    std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
    std::map<UGridFileDimensions, netCDF::NcDim> const& entity_dimensions)
    : UGridEntity(nc_file, topology_variable, entity_attributes, entity_attribute_names, entity_dimensions)
{
}

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
    if (mesh1d.node_edge_id == nullptr)
    {
        throw std::invalid_argument("Mesh1D::define mesh1d node edge id is empty");
    }
    if (mesh1d.node_edge_offset == nullptr)
    {
        throw std::invalid_argument("Mesh1D::define mesh1d node edge offset is empty");
    }

    UGridEntity::define(mesh1d.name, mesh1d.start_index, "Topology data of 1D mesh", 1, mesh1d.is_spherical);
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);

    std::string network_name(mesh1d.network_name);
    auto topology_attribute = m_topology_variable.putAtt("coordinate_space", network_name);

    // node variables
    if (mesh1d.num_nodes > 0)
    {
        // Define node dimensions
        string_builder.clear();
        string_builder << "_nNodes";
        m_dimensions.insert({UGridFileDimensions::node, m_nc_file->addDim(string_builder.str(), mesh1d.num_nodes)});
        define_topological_attribute("node_dimension", string_builder.str());

        // Define node branch and offset topology attributes
        string_builder.clear();
        string_builder << "_node_edge " << m_entity_name << "_node_edge_offset";
        define_topological_attribute("node_coordinates", string_builder.str());

        // Define node branch variable
        define_topological_variable("node_coordinates",
                                    "node_edge",
                                    netCDF::NcType::nc_INT,
                                    {UGridFileDimensions::node},
                                    {{"long_name", "Index of branch on which mesh node are located"}});

        // Define node offset variable
        define_topological_variable("node_coordinates",
                                    "node_edge_offset",
                                    netCDF::NcType::nc_DOUBLE,
                                    {UGridFileDimensions::node},
                                    {{"long_name", "Offset along branch of mesh node"}});

        // add node x an y if present, TODO: consider lat/lon
        bool const add_coordinate_variables = mesh1d.node_x != nullptr && mesh1d.node_y != nullptr;
        if (add_coordinate_variables)
        {
            define_topology_coordinates(UGridFileDimensions::node, "{} of mesh node");
        }

        // Define node id attribute and variable
        string_builder.clear();
        string_builder << "_node_id";
        define_topological_attribute("node_id", string_builder.str());
        define_topological_variable("node_id",
                                    "node_id",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::node, UGridFileDimensions::id},
                                    {{"long_name", "ID of mesh node"}});

        // Define node long name attribute and variable
        define_topological_attribute("node_long_name");
        define_topological_variable("node_long_name",
                                    "node_long_name",
                                    netCDF::NcType::nc_CHAR,
                                    {UGridFileDimensions::node, UGridFileDimensions::long_name},
                                    {{"long_name", "Long name of mesh node"}});
    }

    // edge variables
    if (mesh1d.num_edges > 0)
    {
        // Define node dimension
        string_builder.clear();
        string_builder << "_nEdges";
        m_dimensions.insert({UGridFileDimensions::edge, m_nc_file->addDim(string_builder.str(), mesh1d.num_edges)});

        // Define node dimension attribute
        define_topological_attribute("edge_dimension", string_builder.str());
        m_topology_attributes.insert({topology_attribute.getName(), topology_attribute});

        // Define edge_nodes attribute and variable
        string_builder.clear();
        string_builder << "_edge_nodes";
        define_topological_attribute("edge_node_connectivity", string_builder.str());
        define_topological_variable("edge_node_connectivity",
                                    "edge_nodes",
                                    netCDF::NcType::nc_INT,
                                    {UGridFileDimensions::edge, UGridFileDimensions::Two},
                                    {{"cf_role", topology_attribute.getName()},
                                     {"long_name", "Maps every edge to the two node that it connects"}});

        if (mesh1d.edge_edge_offset != nullptr && mesh1d.edge_edge_id)
        {
            // Define edge branch and offset attributes
            string_builder.clear();
            string_builder << "_edge_edge " << m_entity_name << "_edge_edge_offset";
            define_topological_attribute("edge_coordinates", string_builder.str());

            // Define edge_edge variable
            define_topological_variable("edge_coordinates",
                                        "edge_edge",
                                        netCDF::NcType::nc_INT,
                                        {UGridFileDimensions::edge},
                                        {{"long_name", "Index of branch on which mesh edge are located"}});
            // Define edge_offset variable
            define_topological_variable("edge_coordinates",
                                        "edge_edge_offset",
                                        netCDF::NcType::nc_INT,
                                        {UGridFileDimensions::edge},
                                        {{"long_name", "Offset along branch of mesh edge"}});
        }

        const bool add_coordinate_variables = mesh1d.edge_x != nullptr && mesh1d.edge_y != nullptr;
        if (add_coordinate_variables)
        {
            define_topology_coordinates(UGridFileDimensions::edge, "{} of mesh edge");
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
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh1d.node_edge_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh1d.node_edge_id);
    }
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh1d.node_edge_offset != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).putVar(mesh1d.node_edge_offset);
    }
    if (auto const it = find_attribute_variable_name_with_aliases("node_id"); mesh1d.node_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh1d.node_id);
    }
    if (auto const it = find_attribute_variable_name_with_aliases("node_long_name"); mesh1d.node_long_name != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh1d.node_long_name);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); mesh1d.edge_nodes != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh1d.edge_nodes);
    }
}

void Mesh1D::inquire(ugridapi::Mesh1D& mesh1d) const
{
    if (m_dimensions.find(UGridFileDimensions::node) != m_dimensions.end())
    {
        mesh1d.num_nodes = static_cast<int>(m_dimensions.at(UGridFileDimensions::node).getSize());
    }
    if (m_dimensions.find(UGridFileDimensions::edge) != m_dimensions.end())
    {
        mesh1d.num_edges = static_cast<int>(m_dimensions.at(UGridFileDimensions::edge).getSize());
    }
}

void Mesh1D::get(ugridapi::Mesh1D& mesh1d) const
{
    string_to_char_array(m_entity_name, name_long_length, mesh1d.name);

    if (auto const it = m_topology_attribute_variables.find("coordinate_space"); mesh1d.network_name != nullptr && it != m_topology_attribute_variables.end())
    {
        auto const network_name = it->second.at(0).getName();
        string_to_char_array(network_name, name_long_length, mesh1d.network_name);
    }
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh1d.node_edge_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh1d.node_edge_id);
    }
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh1d.node_edge_offset != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).getVar(mesh1d.node_edge_offset);
    }
    if (auto const it = find_attribute_variable_name_with_aliases("node_id"); mesh1d.node_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh1d.node_id);
    }
    if (auto const it = find_attribute_variable_name_with_aliases("node_long_name"); mesh1d.node_long_name != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh1d.node_long_name);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); mesh1d.edge_nodes != nullptr && it != m_topology_attribute_variables.end())
    {
        const auto var = it->second.at(0);
        var.getVar(mesh1d.edge_nodes);
        apply_start_index_offset(var, mesh1d.start_index, mesh1d.num_edges * 2, mesh1d.edge_nodes);
    }
}
