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

namespace ugridapi
{
    struct Network1d
    {
        /// @brief The name of the network
        char* name = nullptr;

        /// @brief The node x coordinates
        double* node_x = nullptr;

        /// @brief The node y coordinates
        double* node_y = nullptr;

        /// @brief The node names 
        char* node_name_id = nullptr;

        /// @brief The node long names
        char* node_name_long = nullptr;

        /// @brief The edge node connectivity (also called branches for a network) 
        int* edge_node = nullptr;

        /// @brief The edge lengths
        double* edge_length = nullptr;

        /// @brief The order of the edges
        int* edge_order = nullptr;

        /// @brief The name of the edges
        char* edge_name_id = nullptr;

        /// @brief The long name of the edges
        char* edge_name_long = nullptr;

        /// @brief The geometry nodes x coordinates
        double* geometry_nodes_x = nullptr;

        /// @brief The geometry nodes y coordinates
        double* geometry_nodes_y = nullptr;

        /// @brief The number of geometry nodes
        int num_geometry_nodes = 0;

        /// @brief The number of network1d nodes
        int num_nodes = 0;

        /// @brief The number of network1d edges
        int num_edges = 0;

        /// @brief 1 if coordinates are in a spherical system, 0 otherwise 
        int is_spherical = 0;

        /// @brief The start index used in arrays using indices, such as edge_nodes
        int start_index = 0;
    };
}
