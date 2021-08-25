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
        char* name = nullptr;

        double* node_x = nullptr;

        double* node_y = nullptr;

        char* node_id = nullptr;

        char* node_long_name = nullptr;

        int* edge_nodes = nullptr;

        double* branch_length = nullptr;

        int* branch_order = nullptr;

        char* branch_id = nullptr;

        char* branch_long_name = nullptr;

        double* geometry_nodes_x = nullptr;

        double* geometry_nodes_y = nullptr;

        int* num_geometry_nodes = nullptr;

        int num_nodes;

        int num_edges;

        int start_index = 0;

        int is_spherical = 0;
    };
}
