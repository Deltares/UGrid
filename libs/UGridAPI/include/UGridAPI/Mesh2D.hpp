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
        char* name = nullptr;

        /// @brief The edge node connectivity
        int* edge_nodes = nullptr;

        /// @brief The node composing each face
        int* face_nodes = nullptr;

        /// @brief The node x coordinates
        double* node_x = nullptr;

        /// @brief The node y coordinates
        double* node_y = nullptr;

        /// @brief The edge x coordinate
        double* edge_x = nullptr;

        /// @brief The edge y coordinate
        double* edge_y = nullptr;

        /// @brief The face x coordinates
        double* face_x = nullptr;

        /// @brief The face y coordinates
        double* face_y = nullptr;

        /// @brief The edge composing each face
        int* edge_faces = nullptr;

        /// @brief For each face, the edge composing it
        int* face_edges = nullptr;

        /// @brief For each face, the neighboring face
        int* face_faces = nullptr;

        /// @brief The node z coordinates
        double* node_z = nullptr;

        /// @brief The edge z coordinates
        double* edge_z = nullptr;

        /// @brief The face z coordinates
        double* face_z = nullptr;

        /// @brief The z coordinates of a layer
        double* layer_zs = nullptr;

        /// @brief The z coordinates of a layer interface
        double* interface_zs = nullptr;

        /// @brief TODO to be detailed
        double* boundary_node_connectivity = nullptr;

        /// @brief TODO to be detailed
        double* volume_coordinates = nullptr;

        /// @brief The number of node
        int num_nodes = 0;

        /// @brief The number of edge
        int num_edges = 0;

        /// @brief The number of face
        int num_faces = 0;

        /// @brief The number of layers
        int num_layers = 0;

        /// @brief The start index used in arrays using indices, such as edge_node
        int start_index = 0;

        /// @brief The maximum number of face node
        int num_face_nodes_max = 4;

        /// @brief 1 if coordinates are in a spherical system, 0 otherwise
        int is_spherical = 0;

        /// @brief The fill value for array of doubles
        double double_fill_value = ugrid::double_missing_value;

        /// @brief The fill value for array of integers
        int int_fill_value = ugrid::int_missing_value;
    };
} // namespace ugridapi
