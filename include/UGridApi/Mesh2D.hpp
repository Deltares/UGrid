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
    struct Mesh2D
    {
        /// @brief The nodes composing each mesh 2d edge
        int* edge_nodes = nullptr;

        /// @brief The nodes composing each mesh 2d face
        int* face_nodes = nullptr;

        /// @brief The number of nodes for each mesh 2d face
        int* nodes_per_face = nullptr;

        /// @brief The x-coordinates of network1d nodes
        double* node_x = nullptr;

        /// @brief The y-coordinates of network1d nodes
        double* node_y = nullptr;

        /// @brief The x-coordinates of the mesh edges middle points
        double* edge_x = nullptr;

        /// @brief The y-coordinates of the mesh edges middle points
        double* edge_y = nullptr;

        /// @brief The x-coordinates of the mesh faces mass centers
        double* face_x = nullptr;

        /// @brief The x-coordinates of the mesh faces mass centers
        double* face_y = nullptr;

        int* edge_faces = nullptr;

        int* face_edges = nullptr;

        int* face_face = nullptr;

        double* node_z = nullptr;

        double* edge_z = nullptr;

        double* face_z = nullptr;

        double* layer_zs = nullptr;

        double* interface_zs = nullptr;

        double* boundary_node_connectivity = nullptr;

        double* volume_coordinates = nullptr;

        const char* name = nullptr;

        /// @brief The number of mesh nodes
        int num_nodes = 0;

        /// @brief The number of edges
        int num_edges = 0;

        /// @brief The number of faces
        int num_faces = 0;

        /// @brief The total number of nodes composing the mesh 2d faces
        int num_face_nodes = 0;

        int num_layers = 0;

        int start_index = 0;

        int num_face_nodes_max = ugrid::num_face_nodes_max;

        int is_spherical = 0;

        double double_fill_value = ugrid::double_missing_value;

        int int_fill_value = ugrid::int_missing_value;

    };
}
