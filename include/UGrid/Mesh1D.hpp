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

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    /// @brief A class containing the variables ids for a mesh2d
    struct Mesh1D
    {

        Mesh1D(int ugrid_id) : m_file_id(ugrid_id) {};

        int m_file_id;

        /// @brief Topology id
        int m_topology = -1;

        /// @brief Name id
        int m_name = -1;

        /// @brief Dimension ids
        int m_num_nodes = -1;
        int m_num_edges = -1;
        int m_num_faces = -1;
        int m_num_face_nodes = -1;

        /// @brief Data ids
        // TO DO
    };
} // namespace ugrid