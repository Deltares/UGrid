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

#include <UGrid/Mesh2D.hpp>
#include <UGrid/Operations.hpp>

using ugrid::Mesh2D;

void Mesh2D::Define(ugridapi::Mesh2D const& mesh2d)
{
    if (mesh2d.name == nullptr)
    {
        throw std::invalid_argument("Mesh2D::Define mesh name field is empty");
    }

    UGridEntity::Define(mesh2d.name, mesh2d.start_index, "Topology data of 2D mesh", 2, mesh2d.is_spherical);
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);

    // node variables
    if (mesh2d.num_nodes > 0)
    {
        // Add dimensions and topology attributes
        string_builder.clear(); string_builder << "_nNodes";
        m_dimensions.insert({ UGridDimensions::nodes, m_nc_file->addDim(string_builder.str(),mesh2d.num_nodes) });
        m_topology_attributes.insert({ "node_dimension", m_topology_variable.putAtt("node_dimension", string_builder.str()) });

        // Add coordinates
        bool const add_coordinate_variable = mesh2d.node_x != nullptr && mesh2d.node_y != nullptr;
        define_topological_variable_with_coordinates(UGridEntityLocations::nodes, UGridDimensions::nodes, add_coordinate_variable, "%s of mesh nodes");

        // Add optional related variables
        if (mesh2d.node_z != nullptr)
        {
            auto const variable = define_variable_on_location("node_z", UGridDimensions::nodes, "altitude", "z-coordinate of mesh nodes", "m", mesh2d.double_fill_value);
            AddTopologyRelatedVariables(variable);
        }
    }

    // edge variables
    if (mesh2d.num_edges)
    {
        // Add dimensions and topology attributes
        string_builder.clear(); string_builder << "_nEdges";
        m_dimensions.insert({ UGridDimensions::edges, m_nc_file->addDim(string_builder.str(),mesh2d.num_edges) });
        auto topology_attribute = m_topology_variable.putAtt("edge_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);

        // Add edge connectivity
        string_builder.clear(); string_builder << "_edge_nodes";
        topology_attribute = m_topology_variable.putAtt("edge_node_connectivity", string_builder.str());
        add_topology_attribute(topology_attribute);
        auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, { m_dimensions[UGridDimensions::edges], m_dimensions[UGridDimensions::Two] });
        auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Start and end nodes of mesh edges");
        add_start_index(m_start_index, topology_attribute_variable, m_double_fill_value, m_int_fill_value);
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        // Add coordinates
        bool const add_coordinate_variable = mesh2d.edge_x != nullptr && mesh2d.edge_y != nullptr;
        define_topological_variable_with_coordinates(UGridEntityLocations::edges, UGridDimensions::edges, add_coordinate_variable, "characteristic %s of the mesh edge (e.g. midpoint)");
    }

    // faces variables
    if (mesh2d.num_faces > 0)
    {
        // Add dimensions
        string_builder.clear(); string_builder << "_nFace";
        m_dimensions.insert({ UGridDimensions::faces, m_nc_file->addDim(string_builder.str(),mesh2d.num_faces) });
        auto topology_attribute = m_topology_variable.putAtt("face_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);

        string_builder.clear(); string_builder << "_nMax_face_nodes";
        topology_attribute = m_topology_variable.putAtt("max_face_nodes_dimension", string_builder.str());
        add_topology_attribute(topology_attribute);
        m_dimensions.insert({ UGridDimensions::max_face_nodes, m_nc_file->addDim(string_builder.str(), mesh2d.num_face_nodes_max) });

        // Add face_node_connectivity
        string_builder.clear(); string_builder << "_face_nodes";
        topology_attribute = m_topology_variable.putAtt("face_node_connectivity", string_builder.str());
        add_topology_attribute(topology_attribute);

        auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
        auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
        topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Vertex nodes of mesh faces (counterclockwise)");
        add_start_index(m_start_index, topology_attribute_variable, m_double_fill_value, m_int_fill_value);
        add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        // Add face coordinates 
        bool add_coordinate_variable = mesh2d.face_x != nullptr && mesh2d.face_y != nullptr;
        define_topological_variable_with_coordinates(UGridEntityLocations::faces, UGridDimensions::faces, add_coordinate_variable, "characteristic % of the mesh face");

        // Add face bounds (is this always required?)
        define_topological_variable_with_coordinates(UGridEntityLocations::faces, UGridDimensions::faces, true, "%s bounds of mesh faces (i.e. corner coordinates)", "%s%s_bnd");

        // Add optional variables
        if (mesh2d.face_edges != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_face_edges";
            topology_attribute = m_topology_variable.putAtt("face_edge_connectivity", string_builder.str());
            add_topology_attribute(topology_attribute);

            topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Side edges of mesh faces (counterclockwise)");
            add_start_index(m_start_index, topology_attribute_variable, m_double_fill_value, m_int_fill_value);
            add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
        }
        if (mesh2d.face_face != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_face_links";
            topology_attribute = m_topology_variable.putAtt("face_face_connectivity", string_builder.str());
            add_topology_attribute(topology_attribute);

            topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("cf_role", topology_attribute.getName());
            topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Neighboring faces of mesh faces (counterclockwise)");
            add_start_index(m_start_index, topology_attribute_variable, m_double_fill_value, m_int_fill_value);
            add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
        }
        if (mesh2d.edge_faces != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_edge_faces";
            topology_attribute = m_topology_variable.putAtt("edge_face_connectivity", string_builder.str());
            add_topology_attribute(topology_attribute);

            topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
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

void Mesh2D::Put(ugridapi::Mesh2D const& mesh2d)
{
    if (mesh2d.name == nullptr)
    {
        throw std::invalid_argument("Mesh2D::Put invalid mesh name");
    }

    // Nodes
    if (mesh2d.node_x != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(0).putVar(mesh2d.node_x);
    }
    if (mesh2d.node_y != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(1).putVar(mesh2d.node_y);
    }
    if (mesh2d.node_z != nullptr)
    {
        m_related_variables.at("node_z").putVar(mesh2d.node_z);
    }

    // Edges
    if (mesh2d.edge_nodes != nullptr)
    {
        m_topology_attribute_variables.at("edge_node_connectivity").at(0).putVar(mesh2d.edge_nodes);
    }
    if (mesh2d.edge_x != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(0).putVar(mesh2d.edge_x);
    }
    if (mesh2d.edge_y != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(1).putVar(mesh2d.edge_y);
    }

    // Faces
    if (mesh2d.face_nodes != nullptr)
    {
        m_topology_attribute_variables.at("face_node_connectivity").at(0).putVar(mesh2d.face_nodes);
    }
    if (mesh2d.face_face != nullptr)
    {
        m_topology_attribute_variables.at("face_edge_connectivity").at(0).putVar(mesh2d.face_face);
    }
    if (mesh2d.edge_faces != nullptr)
    {
        m_topology_attribute_variables.at("edge_face_connectivity").at(0).putVar(mesh2d.edge_faces);
    }
    if (mesh2d.face_x != nullptr)
    {
        m_topology_attribute_variables.at("face_coordinates").at(0).putVar(mesh2d.face_x);
    }
    if (mesh2d.face_y != nullptr)
    {
        m_topology_attribute_variables.at("face_coordinates").at(1).putVar(mesh2d.face_y);
    }

    if (mesh2d.num_layers > 0)
    {
        // to complete
    }
}

void Mesh2D::Inquire(ugridapi::Mesh2D& mesh2d) const
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



void Mesh2D::Get(ugridapi::Mesh2D& mesh2d) const
{
    FillCharArrayWithStringValues(mesh2d.name, m_entity_name);

    if (mesh2d.node_x != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(0).getVar(mesh2d.node_x);
    }

    if (mesh2d.node_y != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(1).getVar(mesh2d.node_y);
    }

    if (mesh2d.edge_nodes != nullptr)
    {
        m_topology_attribute_variables.at("edge_node_connectivity").at(0).getVar(mesh2d.edge_nodes);
    }

    if (mesh2d.face_x != nullptr)
    {
        m_topology_attribute_variables.at("face_coordinates").at(0).getVar(mesh2d.face_x);
    }

    if (mesh2d.face_y != nullptr)
    {
        m_topology_attribute_variables.at("face_coordinates").at(1).getVar(mesh2d.face_y);
    }

    if (mesh2d.face_nodes != nullptr)
    {
        m_topology_attribute_variables.at("face_node_connectivity").at(0).getVar(mesh2d.face_nodes);
    }
}
