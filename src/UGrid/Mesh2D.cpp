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
        throw std::invalid_argument("Mesh2D::Define invalid mesh name");
    }

    m_entity_name = std::string(mesh2d.name);
    m_spherical_coordinates = mesh2d.is_spherical == 0 ? false : true;
    m_start_index = mesh2d.start_index;

    // add and define the topology variable
    m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_CHAR);
    m_topology_attributes.insert({ "cf_role", m_topology_variable.putAtt("cf_role", "mesh_topology") });
    m_topology_attributes.insert({ "long_name",m_topology_variable.putAtt("long_name", "Topology data of " + m_entity_name) });
    m_topology_attributes.insert({ "topology_dimension", m_topology_variable.putAtt("topology_dimension", netCDF::NcType::nc_INT, 2) });

    // add dimension two 
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
        std::string long_name_pattern = "% of mesh nodes";
        bool const add_coordinate_variable = mesh2d.node_x != nullptr && mesh2d.node_y != nullptr;
        define_topological_variable_with_coordinates(UGridEntityLocations::nodes, UGridDimensions::nodes, add_coordinate_variable, long_name_pattern);

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

        string_builder.clear(); string_builder << "_" << "_edge_nodes";
        m_topology_attributes.insert({ "edge_node_connectivity", m_topology_variable.putAtt("edge_node_connectivity", string_builder.str()) });

        // Add connectivity
        auto variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, { m_dimensions[UGridDimensions::edges], m_dimensions[UGridDimensions::Two] });
        define_topological_variable_attributes(variable, "edge_node_connectivity", "Start and end nodes of mesh edges");

        // Add coordinates
        std::string long_name_pattern = "characteristic % of the mesh edge (e.g. midpoint)";
        bool const add_coordinate_variable = mesh2d.edge_x != nullptr && mesh2d.edge_y != nullptr;
        define_topological_variable_with_coordinates(UGridEntityLocations::edges, UGridDimensions::edges, add_coordinate_variable, long_name_pattern);
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
        auto variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
        define_topological_variable_attributes(variable, "face_node_connectivity", "Vertex nodes of mesh faces (counterclockwise)");

        // add coordinates 
        bool add_coordinate_variable = mesh2d.face_x != nullptr && mesh2d.face_y != nullptr;
        std::string long_name_pattern = "characteristic % of the mesh face";
        define_topological_variable_with_coordinates(UGridEntityLocations::faces, UGridDimensions::faces, add_coordinate_variable, long_name_pattern);

        // add face bounds (is this always required?)
        long_name_pattern = "% bounds of mesh faces (i.e. corner coordinates)";
        std::string name_pattern = "_face_%_bnd";
        add_coordinate_variable = true;
        define_topological_variable_with_coordinates(UGridEntityLocations::edges, UGridDimensions::edges, add_coordinate_variable, long_name_pattern, name_pattern);

        // the optional variables
        if (mesh2d.face_edges != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_face_edges";
            m_topology_attributes.insert({ "face_edge_connectivity", m_topology_variable.putAtt("face_edge_connectivity", string_builder.str()) });

            std::string variable_name(string_builder.str());
            auto variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
            define_topological_variable_attributes(variable, "face_edge_connectivity", "Side edges of mesh faces (counterclockwise)");
        }
        if (mesh2d.face_face != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_face_links";
            m_topology_attributes.insert({ "face_face_connectivity", m_topology_variable.putAtt("face_face_connectivity", string_builder.str()) });

            std::string variable_name(string_builder.str());
            auto variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
            define_topological_variable_attributes(variable, "face_face_connectivity", "Neighboring faces of mesh faces (counterclockwise)");
        }
        if (mesh2d.edge_faces != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_edge_faces";
            m_topology_attributes.insert({ "edge_face_connectivity", m_topology_variable.putAtt("edge_face_connectivity", string_builder.str()) });

            std::string variable_name(string_builder.str());
            auto variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, { m_dimensions[UGridDimensions::faces], m_dimensions[UGridDimensions::max_face_nodes] });
            define_topological_variable_attributes(variable, "edge_face_connectivity", "Neighboring faces of mesh edges");
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
        m_topology_attribute_variables["face_coordinates"].at(0).putVar(mesh2d.face_nodes);
    }
    if (mesh2d.face_y != nullptr)
    {
        m_topology_attribute_variables["face_coordinates"].at(1).putVar(mesh2d.face_nodes);
    }

    if (mesh2d.num_layers > 0)
    {
        // to complete
    }
}

void Mesh2D::Inquire(ugridapi::Mesh2D& mesh2d) const
{
    mesh2d.num_nodes = m_topology_attribute_variables.at("node_coordinates").at(0).getDims().at(0).getSize();
    mesh2d.num_edges = m_topology_attribute_variables.at("edge_coordinates").at(0).getDims().at(0).getSize();
    mesh2d.num_faces = m_topology_attribute_variables.at("face_coordinates").at(0).getDims().at(0).getSize();

    // Other optional numerical values
    if (m_topology_attributes_names.find("face_node_connectivity") != m_topology_attributes_names.end())
    {
        mesh2d.num_face_nodes_max = m_topology_attribute_variables.at("face_node_connectivity").at(0).getDims().at(1).getSize();
    }
}



void Mesh2D::Get(ugridapi::Mesh2D& mesh2d) const
{
    mesh2d.name = m_entity_name.c_str();

    if (mesh2d.node_x != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(0).getVar(mesh2d.node_x);
    }

    if (mesh2d.node_y != nullptr)
    {
        m_topology_attribute_variables.at("node_coordinates").at(1).getVar(mesh2d.node_x);
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
    const auto variables = nc_file->getVars();
    std::vector<Mesh2D> result;
    for (auto const& variable : variables)
    {
        auto attributes = variable.second.getAtts();

        if (!is_mesh_topology_variable(attributes))
        {
            continue;
        }

        int dimensionality;
        attributes["topology_dimension"].getValues(&dimensionality);

        if (dimensionality == 2)
        {
            auto const [attribute_variables, attributes_variable_names] = GetAttributesNames(attributes, variables);
            result.emplace_back(nc_file, variable.first, attribute_variables, attributes_variable_names);
        }
    }

    return result;
};
