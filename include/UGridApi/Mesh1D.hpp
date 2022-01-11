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
    /// @brief A struct used to describe UGrid mesh1d in a C-compatible manner
    struct Mesh1D
    {
        /// @brief The mesh1d name
        std::string name;

        /// @brief The network name
        std::string network_name;

        /// @brief The node x coordinate
        std::vector<double> node_x;

        /// @brief The node y coordinate
        std::vector<double> node_y;

        /// @brief The edge node connectivity
        std::vector<int> edge_node;

        /// @brief The network edge id where every node lies
        std::vector<int> node_edge_id;

        /// @brief The offset of each node on the network branch
        std::vector<double> node_edge_offset;

        /// @brief The node name
        std::string node_id;

        /// @brief The node long name
        std::string node_long_name;

        /// @brief The network edge id where every edge lies
        std::vector<int> edge_edge_id;

        /// @brief The offset of each edge on the network branch
        std::vector<double> edge_edge_offset;

        /// @brief The edge x coordinate
        std::vector<double> edge_x;

        /// @brief The edge y coordinate
        std::vector<double> edge_y;

        /// @brief The number of node
        int num_nodes = 0;

        /// @brief The number of edge
        int num_edges = 0;

        /// @brief 1 If coordinates are in a spherical system, 0 otherwise
        int is_spherical = -1;

        /// @brief The start index used in arrays using indices, such as edge_node
        int start_index = -1;

        /// @brief The fill value for array of doubles
        double double_fill_value = ugrid::double_missing_value;

        /// @brief The fill value for array of integers
        int int_fill_value = ugrid::int_missing_value;
    };
} // namespace ugridapi
