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
    struct Mesh1D
    {
        double* node_x = nullptr;

        double* node_y = nullptr;

        int* edge_nodes = nullptr;

        int* node_branch_id = nullptr;

        double* node_branch_offset = nullptr;

        char* node_name_id = nullptr;

        char* node_name_long = nullptr;

        int* edge_branch_id = nullptr;

        double* edge_branch_offset = nullptr;

        double* edge_x = nullptr;

        double* edge_y = nullptr;

        char* name = nullptr;

        char* network_name = nullptr;

        int num_nodes = 0;

        int num_edges = 0;

        int start_index = 0;

        int is_spherical = 0;

        double double_fill_value = ugrid::double_missing_value;

        int int_fill_value = ugrid::int_missing_value;
    };
}
