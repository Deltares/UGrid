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

#include <UGrid/Mesh2D.hpp>
#include <UGrid/Operations.hpp>

using ugrid::Mesh2D;

Mesh2D::Mesh2D(std::shared_ptr<netCDF::NcFile> nc_file) : UGridEntity(nc_file)
{
}

Mesh2D::Mesh2D(
    std::shared_ptr<netCDF::NcFile> nc_file,
    netCDF::NcVar const& topology_variable,
    std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
    std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
    std::map<UGridDimensions, netCDF::NcDim> const& entity_dimensions)
    : UGridEntity(nc_file, topology_variable, entity_attributes, entity_attribute_names, entity_dimensions)
{
}

void Mesh2D::define(ugridapi::Mesh2D const& mesh2d)
{
    if (mesh2d.name == nullptr)
    {
        throw std::invalid_argument("Mesh2D::define mesh name field is empty");
    }

    UGridEntity::define(mesh2d.name, mesh2d.start_index, "Topology data of 2D mesh", 2, mesh2d.is_spherical);
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);

    // node variables
    if (mesh2d.num_nodes > 0)
    {
        // Add dimensions and topology attributes
        string_builder.clear();
        string_builder << "_nNodes";
        m_dimensions.insert({UGridDimensions::nodes, m_nc_file->addDim(string_builder.str(), mesh2d.num_nodes)});
        m_topology_attributes.insert({"node_dimension", m_topology_variable.putAtt("node_dimension", string_builder.str())});

        // Add coordinates
        bool const add_coordinate_variable = mesh2d.node_x != nullptr && mesh2d.node_y != nullptr;
        if (add_coordinate_variable)
        {
            define_topological_variable_with_coordinates(UGridEntityLocations::nodes, UGridDimensions::nodes, "%s of mesh nodes");
        }

        // Add optional related variables
        if (mesh2d.node_z != nullptr)
        {
            auto const variable = define_variable_on_location("node_z", UGridDimensions::nodes, "altitude", "z-coordinate of mesh nodes", "m", mesh2d.double_fill_value);
            add_topology_related_variables(variable);
        }
    }

    // edge variables
    if (mesh2d.num_edges)
    {
        // Add dimensions and topology attributes
        string_builder.clear();
        string_builder << "_nEdges";
        m_dimensions.insert({UGridDimensions::edges, m_nc_file->addDim(string_builder.str(), mesh2d.num_edges)});
        auto topology_attribute = m_topology_variable.putAtt("edge_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);

        // Add edge connectivity
        string_builder.clear();
        string_builder << "_edge_nodes";
        topology_attribute = m_topology_variable.putAtt("edge_node_connectivity", string_builder.str());
        add_topology_attribute(topology_attribute);
        auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, {m_dimensions[UGridDimensions::edges], m_dimensions[UGridDimensions::Two]});
        auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Start and end nodes of mesh edges");
        add_start_index(m_start_index, topology_attribute_variable, m_double_fill_value, m_int_fill_value);
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        // Add coordinates
        bool const add_coordinate_variable = mesh2d.edge_x != nullptr && mesh2d.edge_y != nullptr;
        if (add_coordinate_variable)
        {
            define_topological_variable_with_coordinates(UGridEntityLocations::edges, UGridDimensions::edges, "characteristic %s of the mesh edge (e.g. midpoint)");
        }
    }

    // faces variables
    if (mesh2d.num_faces > 0)
    {
        // Add dimensions
        string_builder.clear();
        string_builder << "_nFace";
        m_dimensions.insert({UGridDimensions::faces, m_nc_file->addDim(string_builder.str(), mesh2d.num_faces)});
        auto topology_attribute = m_topology_variable.putAtt("face_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);

        string_builder.clear();
        string_builder << "_nMax_face_nodes";
        topology_attribute = m_topology_variable.putAtt("max_face_nodes_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);
        m_dimensions.insert({UGridDimensions::max_face_nodes, m_nc_file->addDim(string_builder.str(), mesh2d.num_face_nodes_max)});

        // Add face_node_connectivity
        string_builder.clear();
        string_builder << "_face_nodes";
        topology_attribute = m_topology_variable.putAtt("face_node_connectivity", string_builder.str());
        add_topology_attribute(topology_attribute);

        auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, {m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes]});
        auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Vertex nodes of mesh faces (counterclockwise)");
        add_start_index(m_start_index, topology_attribute_variable, m_double_fill_value, m_int_fill_value);
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        // Add face coordinates
        bool add_coordinate_variable = mesh2d.face_x != nullptr && mesh2d.face_y != nullptr;
        if (add_coordinate_variable)
        {
            define_topological_variable_with_coordinates(UGridEntityLocations::faces, UGridDimensions::faces, "characteristic % of the mesh face");
        }

        // Add face bounds (is this always required?)
        define_topological_variable_with_coordinates(UGridEntityLocations::faces, UGridDimensions::faces, "%s bounds of mesh faces (i.e. corner coordinates)", "%s%s_bnd");

        // Add optional variables
        if (mesh2d.face_edge != nullptr)
        {
            string_builder.clear();
            string_builder << m_entity_name << "_face_edges";
            topology_attribute = m_topology_variable.putAtt("face_edge_connectivity", string_builder.str());
            add_topology_attribute(topology_attribute);

            topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, {m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes]});
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Side edges of mesh faces (counterclockwise)");
            add_start_index(m_start_index, topology_attribute_variable, m_double_fill_value, m_int_fill_value);
            add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
        }
        if (mesh2d.face_face != nullptr)
        {
            string_builder.clear();
            string_builder << m_entity_name << "_face_links";
            topology_attribute = m_topology_variable.putAtt("face_face_connectivity", string_builder.str());
            add_topology_attribute(topology_attribute);

            topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, {m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes]});
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Neighboring faces of mesh faces (counterclockwise)");
            add_start_index(m_start_index, topology_attribute_variable, m_double_fill_value, m_int_fill_value);
            add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
        }
        if (mesh2d.edge_face != nullptr)
        {
            string_builder.clear();
            string_builder << m_entity_name << "_edge_faces";
            topology_attribute = m_topology_variable.putAtt("edge_face_connectivity", string_builder.str());
            add_topology_attribute(topology_attribute);

            topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, {m_dimensions[UGridDimensions::edges], m_dimensions[UGridDimensions::Two]});
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Neighboring faces of mesh edges");
            add_start_index(m_start_index, topology_attribute_variable, m_double_fill_value, m_int_fill_value);
            add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
        }
    }

    // Layers, optionally required if layers are present (1D or 2D layered mesh topology)
    if (mesh2d.num_layers > 0)
    {
        // Part to complete
        //std::string long_name_patter;
        //define_topological_variable_with_coordinates(UGridEntityLocations::layer, UGridDimensions::layer, mesh2d.num_layers, false, long_name_patter);
        //define_topological_variable_with_coordinates(UGridEntityLocations::layer_interface, UGridDimensions::layer_interface, mesh2d.num_layers + 1, false, long_name_patter);
        //define_topological_variable_with_coordinates(UGridEntityLocations::vertical, UGridDimensions::layer, mesh2d.num_layers + 1, false, long_name_patter);
    }

    m_nc_file->enddef();
}

void Mesh2D::put(ugridapi::Mesh2D const& mesh2d)
{
    if (mesh2d.name == nullptr)
    {
        throw std::invalid_argument("Mesh2D::put invalid mesh name");
    }

    // Nodes
    if (auto const it = m_topology_attribute_variables.find("face_coordinates"); mesh2d.node_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.node_x);
    }
    if (auto const it = m_topology_attribute_variables.find("face_coordinates"); mesh2d.node_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).putVar(mesh2d.node_y);
    }
    if (auto const it = m_related_variables.find("node_z"); mesh2d.node_z != nullptr && it != m_related_variables.end())
    {
        m_related_variables.at("node_z").putVar(mesh2d.node_z);
    }

    // Edges
    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); mesh2d.edge_node != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.edge_node);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_coordinates"); mesh2d.edge_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.edge_x);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_coordinates"); mesh2d.edge_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).putVar(mesh2d.edge_y);
    }

    // Faces
    if (auto const it = m_topology_attribute_variables.find("face_node_connectivity"); mesh2d.face_node != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.face_node);
    }
    if (auto const it = m_topology_attribute_variables.find("face_edge_connectivity"); mesh2d.face_face != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.face_face);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_face_connectivity"); mesh2d.edge_face != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.edge_face);
    }
    if (auto const it = m_topology_attribute_variables.find("face_coordinates"); mesh2d.face_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.face_x);
    }
    if (auto const it = m_topology_attribute_variables.find("face_coordinates"); mesh2d.face_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).putVar(mesh2d.face_y);
    }
    if (mesh2d.num_layers > 0)
    {
        // to complete
    }
}

void Mesh2D::inquire(ugridapi::Mesh2D& mesh2d) const
{
    if (m_dimensions.find(UGridDimensions::nodes) != m_dimensions.end())
    {
        mesh2d.num_nodes = m_dimensions.at(UGridDimensions::nodes).getSize();
    }
    if (m_dimensions.find(UGridDimensions::edges) != m_dimensions.end())
    {
        mesh2d.num_edges = m_dimensions.at(UGridDimensions::edges).getSize();
    }
    if (m_dimensions.find(UGridDimensions::faces) != m_dimensions.end())
    {
        mesh2d.num_faces = m_dimensions.at(UGridDimensions::faces).getSize();
    }
    if (m_dimensions.find(UGridDimensions::max_face_nodes) != m_dimensions.end())
    {
        mesh2d.num_face_nodes_max = m_dimensions.at(UGridDimensions::max_face_nodes).getSize();
    }
}

void Mesh2D::get(ugridapi::Mesh2D& mesh2d) const
{
    string_to_char_array(mesh2d.name, m_entity_name, name_lengths);

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh2d.node_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh2d.node_x);
    }

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh2d.node_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).getVar(mesh2d.node_y);
    }

    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); mesh2d.edge_node != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh2d.edge_node);
    }

    if (auto const it = m_topology_attribute_variables.find("face_coordinates"); mesh2d.face_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh2d.face_x);
    }

    if (auto const it = m_topology_attribute_variables.find("face_coordinates"); mesh2d.face_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).getVar(mesh2d.face_y);
    }

    if (auto const it = m_topology_attribute_variables.find("face_node_connectivity"); mesh2d.face_node != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh2d.face_node);
    }
}
