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

#include <sstream>

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

    std::string mesh2d_name(mesh2d.name);
    auto topology_variable = m_nc_file->addVar(mesh2d_name, netCDF::NcType::nc_CHAR);

    std::vector<netCDF::NcVarAtt> variableAttributes;
    std::stringstream ss;

    variableAttributes.emplace_back(topology_variable.putAtt("cf_role", "mesh_topology"));
    ss << "Topology data of " << mesh2d_name;
    variableAttributes.emplace_back(topology_variable.putAtt("long_name", ss.str()));

    variableAttributes.emplace_back(topology_variable.putAtt("topology_dimension", netCDF::NcType::nc_INT, 2));

    if (mesh2d.is_spherical == 1)
    {
        ss.str(std::string()); ss << mesh2d_name << "_node_x " << mesh2d_name << "_node_y " << mesh2d_name << "_node_lon " << mesh2d_name << "_node_lat";
        variableAttributes.emplace_back(topology_variable.putAtt("node_coordinates", ss.str()));
    }
    else
    {
        ss.str(std::string());  ss << mesh2d_name << "_node_x " << mesh2d_name << "_node_y";
        variableAttributes.emplace_back(topology_variable.putAtt("node_coordinates", ss.str()));
    }

    ss.str(std::string());  ss << mesh2d_name << "_nNodes";
    variableAttributes.emplace_back(topology_variable.putAtt("node_dimension", ss.str()));

    ss.str(std::string());  ss << mesh2d_name << "_nMax_face_nodes";
    variableAttributes.emplace_back(topology_variable.putAtt("max_face_nodes_dimension", ss.str()));

    if (mesh2d.num_edges > 0)
    {
        ss.str(std::string());  ss << mesh2d_name << "_edge_nodes";
        variableAttributes.emplace_back(topology_variable.putAtt("edge_node_connectivity", ss.str()));

        ss.str(std::string());  ss << mesh2d_name << "_nEdges";
        variableAttributes.emplace_back(topology_variable.putAtt("edge_dimension", ss.str()));

        if (mesh2d.is_spherical == 1)
        {
            ss.str(std::string());  ss << mesh2d_name << "_edge_x " << mesh2d_name << "_edge_y " << mesh2d_name << "_edge_lon " << mesh2d_name << "_edge_lat";
            variableAttributes.emplace_back(topology_variable.putAtt("edge_coordinates", ss.str()));
        }
        else
        {
            ss.str(std::string());  ss << mesh2d_name << "_edge_x " << mesh2d_name << "_edge_y";
            variableAttributes.emplace_back(topology_variable.putAtt("edge_coordinates", ss.str()));
        }
    }

    if (mesh2d.num_faces > 0)
    {
        ss.str(std::string());  ss << mesh2d_name << "_face_nodes";
        variableAttributes.emplace_back(topology_variable.putAtt("face_node_connectivity", ss.str()));

        ss.str(std::string());  ss << mesh2d_name << "_nFaces";
        variableAttributes.emplace_back(topology_variable.putAtt("face_dimension", ss.str()));

        if (mesh2d.face_edges != nullptr)
        {
            ss.str(std::string());  ss << mesh2d_name << "_face_edges";
            variableAttributes.emplace_back(topology_variable.putAtt("face_edge_connectivity", ss.str()));
        }
        if (mesh2d.face_face != nullptr)
        {
            ss.str(std::string());  ss << mesh2d_name << "_face_links";
            variableAttributes.emplace_back(topology_variable.putAtt("face_face_connectivity", ss.str()));
        }
        if (mesh2d.edge_faces != nullptr)
        {
            ss.str(std::string());  ss << mesh2d_name << "_edge_faces";
            variableAttributes.emplace_back(topology_variable.putAtt("edge_face_connectivity", ss.str()));
        }
        if (mesh2d.face_x != nullptr && mesh2d.face_y != nullptr)
        {
            if (mesh2d.is_spherical == 1)
            {
                ss.str(std::string());  ss << mesh2d_name << "_face_x " << mesh2d_name << "_face_y " << mesh2d_name << "_face_lon " << mesh2d_name << "_face_lat";
                variableAttributes.emplace_back(topology_variable.putAtt("face_coordinates", ss.str()));
            }
            else
            {
                ss.str(std::string());  ss << mesh2d_name << "_face_x " << mesh2d_name << "_face_y";
                variableAttributes.emplace_back(topology_variable.putAtt("face_coordinates", ss.str()));
            }
        }
    }

    // Optionally required if layers are present (1D or 2D layered mesh topology)
    if (mesh2d.num_layers > 0)
    {
        ss.str(std::string());  ss << mesh2d_name << "_nLayers";
        variableAttributes.emplace_back(topology_variable.putAtt("layer_dimension", ss.str()));

        ss.str(std::string());  ss << mesh2d_name << "_nInterfaces";
        variableAttributes.emplace_back(topology_variable.putAtt("interface_dimension", ss.str()));

        ss.str(std::string());  ss << mesh2d_name << "nLayers: " << mesh2d_name << "_nInterfaces (padding: none)";
        variableAttributes.emplace_back(topology_variable.putAtt("vertical_dimensions", ss.str()));
    }

}

void Mesh2D::Put(ugridapi::Mesh2D const& mesh2d)
{
    //m_nc_file->addDim()


}

void Mesh2D::Inquire(ugridapi::Mesh2D& mesh2d) const
{
    mesh2d.num_nodes = m_nc_file->getVar(m_attribute_variable_names.at("node_coordinates").at(0)).getDims().at(0).getSize();
    mesh2d.num_edges = m_nc_file->getVar(m_attribute_variable_names.at("edge_coordinates").at(0)).getDims().at(0).getSize();
    mesh2d.num_faces = m_nc_file->getVar(m_attribute_variable_names.at("face_coordinates").at(0)).getDims().at(0).getSize();

    // Other optional numerical values
    if (m_attribute_variable_names.find("face_node_connectivity") != m_attribute_variable_names.end())
    {
        mesh2d.num_face_nodes_max = m_nc_file->getVar(m_attribute_variable_names.at("face_node_connectivity").at(0)).getDims().at(1).getSize();
    }
}

void Mesh2D::Get(ugridapi::Mesh2D& mesh2d) const
{
    Inquire(mesh2d);
    mesh2d.name = m_entity_name.c_str();

    m_nc_file->getVar(m_attribute_variable_names.at("node_coordinates").at(0)).getVar(mesh2d.node_x);
    m_nc_file->getVar(m_attribute_variable_names.at("node_coordinates").at(1)).getVar(mesh2d.node_y);

    m_nc_file->getVar(m_attribute_variable_names.at("edge_node_connectivity").at(0)).getVar(mesh2d.edge_nodes);

    m_nc_file->getVar(m_attribute_variable_names.at("face_coordinates").at(0)).getVar(mesh2d.face_x);
    m_nc_file->getVar(m_attribute_variable_names.at("face_coordinates").at(1)).getVar(mesh2d.face_y);

    m_nc_file->getVar(m_attribute_variable_names.at("face_node_connectivity").at(0)).getVar(mesh2d.face_nodes);
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
            const auto [attribute_variables, attributes_variable_names] = GetAttributesNames(attributes, variables);
            result.emplace_back(nc_file, variable.first, attributes_variable_names);
        }
    }

    return result;
};
