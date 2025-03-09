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

#include <UGrid/Mesh2D.hpp>
#include <UGrid/Operations.hpp>
#include <UGrid/UGridVarAttributeStringBuilder.hpp>

using ugrid::Mesh2D;

Mesh2D::Mesh2D(std::shared_ptr<netCDF::NcFile> nc_file) : UGridEntity(nc_file)
{
}

Mesh2D::Mesh2D(
    std::shared_ptr<netCDF::NcFile> nc_file,
    netCDF::NcVar const& topology_variable,
    std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
    std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
    std::map<UGridFileDimensions, netCDF::NcDim> const& entity_dimensions)
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
        // Define node dimensions
        string_builder.clear();
        string_builder << "_nNodes";
        m_dimensions.insert({UGridFileDimensions::node, m_nc_file->addDim(string_builder.str(), mesh2d.num_nodes)});
        m_topology_attributes.insert({"node_dimension", m_topology_variable.putAtt("node_dimension", string_builder.str())});

        // Define coordinates
        bool const add_coordinate_variable = mesh2d.node_x != nullptr && mesh2d.node_y != nullptr;
        if (add_coordinate_variable)
        {
            define_topology_coordinates(UGridFileDimensions::node, "%s of mesh node");
        }

        // Define optional related variables
        if (mesh2d.node_z != nullptr)
        {
            auto const location_attribute_value = get_location_attribute_value("node");
            define_topology_related_variables(
                "node_z",
                netCDF::NcType::nc_DOUBLE,
                {UGridFileDimensions::node},
                {{"standard_name", "altitude"},
                 {"long_name", "z-coordinate of mesh node"},
                 {"units", "m"},
                 {"coordinates", "node"},
                 {"location", location_attribute_value}});
        }
    }

    // edge variables
    if (mesh2d.num_edges)
    {
        // Define edge dimensions
        string_builder.clear();
        string_builder << "_nEdges";
        m_dimensions.insert({UGridFileDimensions::edge, m_nc_file->addDim(string_builder.str(), mesh2d.num_edges)});

        // Define edge_dimension topology attribute
        define_topological_attribute("edge_dimension", string_builder.str());

        // Define edge_nodes topology attribute and variable
        string_builder.clear();
        string_builder << "_edge_nodes";
        define_topological_attribute("edge_node_connectivity", string_builder.str());
        define_topological_variable("edge_node_connectivity",
                                    "edge_nodes",
                                    netCDF::NcType::nc_INT,
                                    {UGridFileDimensions::edge, UGridFileDimensions::Two},
                                    {{"long_name", "Start and end node of mesh edge"}});

        // Define edge_nodes coordinates
        if (mesh2d.edge_x != nullptr && mesh2d.edge_y != nullptr)
        {
            define_topology_coordinates(UGridFileDimensions::edge, "characteristic %s of the mesh edge (e.g. midpoint)");
        }
    }

    // face variables
    if (mesh2d.num_faces > 0)
    {
        // Define face dimensions
        string_builder.clear();
        string_builder << "_nFaces";
        m_dimensions.insert({UGridFileDimensions::face, m_nc_file->addDim(string_builder.str(), mesh2d.num_faces)});
        define_topological_attribute("face_dimension", string_builder.str());

        string_builder.clear();
        string_builder << "_nMax_face_nodes";
        define_topological_attribute("max_face_nodes_dimension", string_builder.str());
        m_dimensions.insert({UGridFileDimensions::max_face_node, m_nc_file->addDim(string_builder.str(), mesh2d.num_face_nodes_max)});

        // Define face_node topology attribute and variable
        string_builder.clear();
        string_builder << "_face_nodes";
        define_topological_attribute("face_node_connectivity", string_builder.str());
        define_topological_variable("face_node_connectivity",
                                    "face_nodes",
                                    netCDF::NcType::nc_INT,
                                    {UGridFileDimensions::face, UGridFileDimensions::max_face_node},
                                    {{"long_name", "Vertex node of mesh face(counterclockwise)"}}, true);

        // Define face coordinates
        bool const add_coordinate_variable = mesh2d.face_x != nullptr && mesh2d.face_y != nullptr;
        if (add_coordinate_variable)
        {
            define_topology_coordinates(UGridFileDimensions::face, "characteristic %s of the mesh face");
        }

        // Define face bounds (is this always required?)
        // define_topology_coordinates(UGridFileDimensions::face, "%s bounds of mesh face (i.e. corner coordinates)", "%s%s_bnd");

        // Define optional variables
        if (mesh2d.face_edges != nullptr)
        {
            // Define face_edges topology attribute and variable
            string_builder.clear();
            string_builder << "_face_edge";
            define_topological_attribute("face_edge_connectivity", string_builder.str());
            define_topological_variable("face_edge_connectivity",
                                        "face_edge",
                                        netCDF::NcType::nc_INT,
                                        {UGridFileDimensions::face, UGridFileDimensions::max_face_node},
                                        {{"long_name", "Side edge of mesh face (counterclockwise)"}}, true);
        }
        if (mesh2d.face_faces != nullptr)
        {
            // Define face_links topology attribute and variable
            string_builder.clear();
            string_builder << "_face_links";
            define_topological_attribute("face_face_connectivity", string_builder.str());
            define_topological_variable("face_face_connectivity",
                                        "face_links",
                                        netCDF::NcType::nc_INT,
                                        {UGridFileDimensions::face, UGridFileDimensions::max_face_node},
                                        {{"long_name", "Neighboring face of mesh face (counterclockwise)"}});
        }
        if (mesh2d.edge_faces != nullptr)
        {
            // Define edge_face topology attribute variable and variable
            string_builder.clear();
            string_builder << "_edge_faces";
            define_topological_attribute("edge_face_connectivity", string_builder.str());
            define_topological_variable("edge_face_connectivity",
                                        "edge_faces",
                                        netCDF::NcType::nc_INT,
                                        {UGridFileDimensions::edge, UGridFileDimensions::Two},
                                        {{"long_name", "Neighboring face of mesh edge"}});
        }
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
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh2d.node_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.node_x);
    }
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh2d.node_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).putVar(mesh2d.node_y);
    }
    if (auto const it = m_related_variables.find("node_z"); mesh2d.node_z != nullptr && it != m_related_variables.end())
    {
        it->second.putVar(mesh2d.node_z);
    }

    // Edges
    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); mesh2d.edge_nodes != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.edge_nodes);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_face_connectivity"); mesh2d.edge_faces != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.edge_faces);
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
    if (auto const it = m_topology_attribute_variables.find("face_node_connectivity"); mesh2d.face_nodes != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.face_nodes);
    }
    if (auto const it = m_topology_attribute_variables.find("face_edge_connectivity"); mesh2d.face_edges != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.face_edges);
    }
    if (auto const it = m_topology_attribute_variables.find("face_face_connectivity"); mesh2d.face_faces != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(mesh2d.face_faces);
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
    if (m_dimensions.find(UGridFileDimensions::node) != m_dimensions.end())
    {
        mesh2d.num_nodes = static_cast<int>(m_dimensions.at(UGridFileDimensions::node).getSize());
    }
    if (m_dimensions.find(UGridFileDimensions::edge) != m_dimensions.end())
    {
        mesh2d.num_edges = static_cast<int>(m_dimensions.at(UGridFileDimensions::edge).getSize());
    }
    if (m_dimensions.find(UGridFileDimensions::face) != m_dimensions.end())
    {
        mesh2d.num_faces = static_cast<int>(m_dimensions.at(UGridFileDimensions::face).getSize());
    }
    if (m_dimensions.find(UGridFileDimensions::max_face_node) != m_dimensions.end())
    {
        mesh2d.num_face_nodes_max = static_cast<int>(m_dimensions.at(UGridFileDimensions::max_face_node).getSize());
    }
}

void Mesh2D::get(ugridapi::Mesh2D& mesh2d) const
{
    string_to_char_array(m_entity_name, name_long_length, mesh2d.name);

    // Nodes
    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh2d.node_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh2d.node_x);
    }

    if (auto const it = m_topology_attribute_variables.find("node_coordinates"); mesh2d.node_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).getVar(mesh2d.node_y);
    }

    if (auto const it = m_related_variables.find("node_z"); mesh2d.node_z != nullptr && it != m_related_variables.end())
    {
       it->second.getVar(mesh2d.node_z);
    }

    // Edges
    if (auto const it = m_topology_attribute_variables.find("edge_node_connectivity"); mesh2d.edge_nodes != nullptr && it != m_topology_attribute_variables.end())
    {
        const auto var = it->second.at(0);
        var.getVar(mesh2d.edge_nodes);
        apply_start_index_offset(var, mesh2d.start_index, mesh2d.num_edges * 2, mesh2d.edge_nodes);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_face_connectivity"); mesh2d.edge_faces != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh2d.edge_faces);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_coordinates"); mesh2d.edge_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh2d.edge_x);
    }
    if (auto const it = m_topology_attribute_variables.find("edge_coordinates"); mesh2d.edge_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).getVar(mesh2d.edge_y);
    }

    // Faces
    if (auto const it = m_topology_attribute_variables.find("face_node_connectivity"); mesh2d.face_nodes != nullptr && it != m_topology_attribute_variables.end())
    {
        const auto var = it->second.at(0);
        var.getVar(mesh2d.face_nodes);
        apply_start_index_offset(var, mesh2d.start_index, mesh2d.num_faces * mesh2d.num_face_nodes_max, mesh2d.face_nodes);
    }
    if (auto const it = m_topology_attribute_variables.find("face_edge_connectivity"); mesh2d.face_edges != nullptr && it != m_topology_attribute_variables.end())
    {
        const auto var = it->second.at(0);
        var.getVar(mesh2d.face_edges);
        apply_start_index_offset(var, mesh2d.start_index, mesh2d.num_faces * mesh2d.num_face_nodes_max, mesh2d.face_edges);
    }
    if (auto const it = m_topology_attribute_variables.find("face_face_connectivity"); mesh2d.face_faces != nullptr && it != m_topology_attribute_variables.end())
    {
        const auto var = it->second.at(0);
        var.getVar(mesh2d.face_faces);
        apply_start_index_offset(var, mesh2d.start_index, mesh2d.num_faces * mesh2d.num_face_nodes_max, mesh2d.face_faces);
    }
    if (auto const it = m_topology_attribute_variables.find("face_coordinates"); mesh2d.face_x != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(mesh2d.face_x);
    }
    if (auto const it = m_topology_attribute_variables.find("face_coordinates"); mesh2d.face_y != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(1).getVar(mesh2d.face_y);
    }
    if (mesh2d.num_layers > 0)
    {
        // to complete
    }
}
