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

    m_entity_name = std::string(mesh2d.name);
    m_spherical_coordinates = mesh2d.is_spherical == 0 ? false : true;
    m_start_index = mesh2d.start_index;

    // add and define the topology variable
    m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_CHAR);
    m_topology_attributes.insert({ "cf_role", m_topology_variable.putAtt("cf_role", "mesh_topology") });
    m_topology_attributes.insert({ "long_name",m_topology_variable.putAtt("long_name", "Topology data of 2D mesh") });
    m_topology_attributes.insert({ "topology_dimension", m_topology_variable.putAtt("topology_dimension", netCDF::NcType::nc_INT, 2) });

    // add dimension Two 
    m_dimensions.insert({ UGridDimensions::Two, m_nc_file->addDim("Two", 2) });

    // string builder with the entity name as first part of the string
    UGridVarAttributeStringBuilder string_builder(m_entity_name);

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
            m_related_variables.insert({ "node_z", define_variable_on_location("node_z", UGridDimensions::nodes, "altitude", "z-coordinate of mesh nodes", "m", mesh2d.double_fill_value) });
        }
    }

    // edge variables
    if (mesh2d.num_edges)
    {
        // Add dimensions and topology attributes
        string_builder.clear(); string_builder << "_nEdges";
        m_dimensions.insert({ UGridDimensions::edges, m_nc_file->addDim(string_builder.str(),mesh2d.num_edges) });
        m_topology_attributes.insert({ "edge_dimension", m_topology_variable.putAtt("edge_dimension", string_builder.str()) });

        string_builder.clear(); string_builder << "_edge_nodes";
        m_topology_attributes.insert({ "edge_node_connectivity", m_topology_variable.putAtt("edge_node_connectivity", string_builder.str()) });

        // Add connectivity
        auto topological_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, { m_dimensions[UGridDimensions::edges], m_dimensions[UGridDimensions::Two] });
        define_topological_variable_attributes("edge_node_connectivity", topological_variable, "Start and end nodes of mesh edges");

        // Add coordinates
        bool const add_coordinate_variable = mesh2d.edge_x != nullptr && mesh2d.edge_y != nullptr;
        define_topological_variable_with_coordinates(UGridEntityLocations::edges, UGridDimensions::edges, add_coordinate_variable, "characteristic %s of the mesh edge (e.g. midpoint)");
    }

    // faces variables
    if (mesh2d.num_faces > 0)
    {
        // Add dimensions and topology attributes
        string_builder.clear(); string_builder << "_nFace";
        m_dimensions.insert({ UGridDimensions::faces, m_nc_file->addDim(string_builder.str(),mesh2d.num_faces) });
        m_topology_attributes.insert({ "face_dimension", m_topology_variable.putAtt("face_dimension", string_builder.str()) });

        string_builder.clear(); string_builder << "_nMax_face_nodes";
        m_topology_attributes.insert({ "max_face_nodes_dimension", m_topology_variable.putAtt("max_face_nodes_dimension", string_builder.str()) });
        m_dimensions.insert({ UGridDimensions::max_face_nodes, m_nc_file->addDim(string_builder.str(), mesh2d.num_face_nodes_max) });

        string_builder.clear(); string_builder << "_face_nodes";
        m_topology_attributes.insert({ "face_node_connectivity", m_topology_variable.putAtt("face_node_connectivity", string_builder.str()) });

        // add the required variable face nodes
        auto topological_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
        define_topological_variable_attributes("face_node_connectivity", topological_variable, "Vertex nodes of mesh faces (counterclockwise)");

        // add coordinates 
        bool add_coordinate_variable = mesh2d.face_x != nullptr && mesh2d.face_y != nullptr;
        define_topological_variable_with_coordinates(UGridEntityLocations::faces, UGridDimensions::faces, add_coordinate_variable, "characteristic % of the mesh face");

        // add face bounds (is this always required?)
        define_topological_variable_with_coordinates(UGridEntityLocations::faces, UGridDimensions::faces, true, "%s bounds of mesh faces (i.e. corner coordinates)", "%s%s_bnd");

        // the optional variables
        if (mesh2d.face_edges != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_face_edges";
            m_topology_attributes.insert({ "face_edge_connectivity", m_topology_variable.putAtt("face_edge_connectivity", string_builder.str()) });

            topological_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
            define_topological_variable_attributes("face_edge_connectivity", topological_variable, "Side edges of mesh faces (counterclockwise)");
        }
        if (mesh2d.face_face != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_face_links";
            m_topology_attributes.insert({ "face_face_connectivity", m_topology_variable.putAtt("face_face_connectivity", string_builder.str()) });

            m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
            define_topological_variable_attributes("face_face_connectivity", topological_variable, "Neighboring faces of mesh faces (counterclockwise)");
        }
        if (mesh2d.edge_faces != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_edge_faces";
            m_topology_attributes.insert({ "edge_face_connectivity", m_topology_variable.putAtt("edge_face_connectivity", string_builder.str()) });

            topological_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
            define_topological_variable_attributes("edge_face_connectivity", topological_variable, "Neighboring faces of mesh edges");
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
        m_topology_attribute_variables["node_coordinates"].at(0).putVar(mesh2d.node_x);
    }
    if (mesh2d.node_y != nullptr)
    {
        m_topology_attribute_variables["node_coordinates"].at(1).putVar(mesh2d.node_y);
    }
    if (mesh2d.node_z != nullptr)
    {
        m_related_variables["node_z"].putVar(mesh2d.node_z);
    }

    // Edges
    if (mesh2d.edge_nodes != nullptr)
    {
        m_topology_attribute_variables["edge_node_connectivity"].at(0).putVar(mesh2d.edge_nodes);
    }
    if (mesh2d.edge_x != nullptr)
    {
        m_topology_attribute_variables["node_coordinates"].at(0).putVar(mesh2d.edge_x);
    }
    if (mesh2d.edge_y != nullptr)
    {
        m_topology_attribute_variables["node_coordinates"].at(1).putVar(mesh2d.edge_y);
    }

    // Faces
    if (mesh2d.face_nodes != nullptr)
    {
        m_topology_attribute_variables["face_node_connectivity"].at(0).putVar(mesh2d.face_nodes);
    }
    if (mesh2d.face_face != nullptr)
    {
        m_topology_attribute_variables["face_edge_connectivity"].at(0).putVar(mesh2d.face_face);
    }
    if (mesh2d.edge_faces != nullptr)
    {
        m_topology_attribute_variables["edge_face_connectivity"].at(0).putVar(mesh2d.edge_faces);
    }
    if (mesh2d.face_x != nullptr)
    {
        m_topology_attribute_variables["face_coordinates"].at(0).putVar(mesh2d.face_x);
    }
    if (mesh2d.face_y != nullptr)
    {
        m_topology_attribute_variables["face_coordinates"].at(1).putVar(mesh2d.face_y);
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
    if (mesh2d.name != nullptr)
    {
        for (auto i = 0; i < m_entity_name.size(); ++i)
        {
            mesh2d.name[i] = m_entity_name[i];
        }
        mesh2d.name[m_entity_name.size()] = '\0';
    }

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

std::vector<Mesh2D> Mesh2D::Create(std::shared_ptr<netCDF::NcFile> const& nc_file)
{
    // Get all vars in this file
    auto const  file_variables = nc_file->getVars();
    auto const  file_dimensions = nc_file->getDims();

    std::vector<Mesh2D> result;
    for (auto const& variable : file_variables)
    {
        auto variable_attributes = variable.second.getAtts();

        if (!is_mesh_topology_variable(variable_attributes))
        {
            continue;
        }

        int dimensionality;
        variable_attributes["topology_dimension"].getValues(&dimensionality);

        if (dimensionality == m_dimensionality)
        {
            // entity_attribute_keys, entity_attribute_values, entity_dimensions
            auto const [entity_attribute_variables, entity_attribute_strings, entity_dimensions] = GetUGridEntity(variable.second, file_dimensions, file_variables);
            result.emplace_back(nc_file, variable.second, entity_attribute_variables, entity_attribute_strings, entity_dimensions);

        }
    }

    return result;
};
