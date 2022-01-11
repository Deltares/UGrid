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

#pragma once

#include <UGrid/Constants.hpp>

namespace ugridapi
{
    /// @brief A struct used to describe UGrid mesh2d in a C-compatible manner
    struct Mesh2D
    {
        /// @brief The mesh2d name
        std::string name;

        /// @brief The edge node connectivity
        std::vector<int> edge_node;

        /// @brief The node composing each face
        std::vector<int> face_node;

        /// @brief The node x coordinates
        std::vector<double> node_x;

        /// @brief The node y coordinates
        std::vector<double> node_y;

        /// @brief The edge x coordinate
        std::vector<double> edge_x;

        /// @brief The edge y coordinate
        std::vector<double> edge_y;

        /// @brief The face x coordinates
        std::vector<double> face_x;

        /// @brief The face y coordinates
        std::vector<double> face_y;

        /// @brief The edge composing each face
        std::vector<int> edge_face;

        /// @brief For each face, the edge composing it
        std::vector<int> face_edge;

        /// @brief For each face, the neighboring face
        std::vector<int> face_face;

        /// @brief The node z coordinates
        std::vector<double> node_z;

        /// @brief The edge z coordinates
        std::vector<double> edge_z;

        /// @brief The face z coordinates
        std::vector<double> face_z;

        /// @brief The z coordinates of a layer
        std::vector<double> layer_zs;

        /// @brief The z coordinates of a layer interface
        std::vector<double> interface_zs;

        /// @brief TODO to be detailed
        std::vector<double> boundary_node_connectivity;

        /// @brief TODO to be detailed
        std::vector<double> volume_coordinates;

        /// @brief The number of node
        int num_nodes = 0;

        /// @brief The number of edge
        int num_edges = 0;

        /// @brief The number of face
        int num_faces = 0;

        /// @brief The number of layers
        int num_layers = 0;

        /// @brief The start index used in arrays using indices, such as edge_node
        int start_index = -1;

        /// @brief The maximum number of face node
        int num_face_nodes_max = 0;

        /// @brief 1 if coordinates are in a spherical system, 0 otherwise
        int is_spherical = -1;

        /// @brief The fill value for array of doubles
        double double_fill_value = ugrid::double_missing_value;

        /// @brief The fill value for array of integers
        int int_fill_value = ugrid::int_missing_value;
    };
} // namespace ugridapi
