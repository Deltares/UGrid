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
#include <UGrid/UGridVariable.hpp>

using ugrid::Mesh2D;

void Mesh2D::Define(ugridapi::Mesh2D const& mesh2d)
{
    if (mesh2d.name == nullptr)
    {
        throw std::invalid_argument("Mesh2D::Define invalid mesh name");
    }

    m_entity_name = std::string(mesh2d.name);
    m_spherical_coordinates = mesh2d.is_spherical == 0 ? false : true;
    m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_CHAR);

    // define topology variable
    m_topology_attributes.insert({ "cf_role", m_topology_variable.putAtt("cf_role", "mesh_topology") });
    m_topology_attributes.insert({ "long_name",m_topology_variable.putAtt("long_name", "Topology data of " + m_entity_name) });
    m_topology_attributes.insert({ "topology_dimension", m_topology_variable.putAtt("topology_dimension", netCDF::NcType::nc_INT, 2) });
    m_nc_file->addDim("Two", 2);

    UGridVarAttributeStringBuilder string_builder(m_entity_name);
    // node variables
    if (mesh2d.num_nodes > 0)
    {
        bool add_coordinate_variable = mesh2d.node_x != nullptr && mesh2d.node_y != nullptr;
        // define the node variable
        define_locations(UGridEntityLocations::nodes, mesh2d.num_nodes, add_coordinate_variable);

        string_builder.clear(); string_builder << "_nMax_face_nodes";
        m_topology_attributes.insert({ "max_face_nodes_dimension", m_topology_variable.putAtt("max_face_nodes_dimension", string_builder.str()) });

        // optional location nodes
        m_related_variables.insert({ "node_z", UGridVariable::define_node_variables("node_z", *this, netCDF::NcType::nc_DOUBLE, "altitude", "z-coordinate of mesh nodes", "m", double_missing_value) });
    }

    // edge variables
    if (mesh2d.num_edges)
    {
        bool add_coordinate_variable = mesh2d.edge_x != nullptr && mesh2d.edge_y != nullptr;

        define_locations(UGridEntityLocations::edges, mesh2d.num_edges, add_coordinate_variable);

        // optional location edges
        // define related variables eventually required
        m_related_variables.insert({ "node_z", UGridVariable::define_node_variables("node_z", *this, netCDF::NcType::nc_DOUBLE, "altitude", "z-coordinate of mesh nodes", "m", double_missing_value) });
    }

    // faces variables
    if (mesh2d.num_faces > 0)
    {
        bool add_coordinate_variable = mesh2d.face_x != nullptr && mesh2d.face_y != nullptr;

        define_locations(UGridEntityLocations::faces, mesh2d.num_faces, add_coordinate_variable);

        if (mesh2d.face_edges != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_face_edges";
            m_topology_attributes.insert({ "face_edge_connectivity", m_topology_variable.putAtt("face_edge_connectivity", string_builder.str()) });
        }
        if (mesh2d.face_face != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_face_links";
            m_topology_attributes.insert({ "face_face_connectivity", m_topology_variable.putAtt("face_face_connectivity", string_builder.str()) });
        }
        if (mesh2d.edge_faces != nullptr)
        {
            string_builder.clear(); string_builder << m_entity_name << "_edge_faces";
            m_topology_attributes.insert({ "edge_face_connectivity", m_topology_variable.putAtt("edge_face_connectivity", string_builder.str()) });
        }
    }

    // Layers, optionally required if layers are present (1D or 2D layered mesh topology)
    if (mesh2d.num_layers > 0)
    {
        define_locations(UGridEntityLocations::layer, mesh2d.num_layers, false);
        define_locations(UGridEntityLocations::layer_interface, mesh2d.num_layers + 1, false);
        define_locations(UGridEntityLocations::vertical, mesh2d.num_layers + 1, false);
    }

}

void Mesh2D::Put(ugridapi::Mesh2D const& mesh2d)
{
    if (mesh2d.name == nullptr)
    {
        throw std::invalid_argument("Mesh2D::Define invalid mesh name");
    }
    m_entity_name = std::string(mesh2d.name);
    m_spherical_coordinates = mesh2d.is_spherical == 0 ? false : true;

    UGridVarAttributeStringBuilder string_builder(m_entity_name);

    if (mesh2d.num_nodes > 0)
    {
    }

    if (mesh2d.num_edges > 0)
    {

    }
    if (mesh2d.num_faces > 0)
    {

    }
    if (mesh2d.num_layers > 0)
    {
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
