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
    struct Contacts
    {
        int* mesh1d_indices = nullptr;

        int* mesh2d_indices = nullptr;

        int* contact_type = nullptr;

        char* contact_name_id = nullptr;

        char* contact_name_long = nullptr;

        char* name = nullptr;

        char* mesh_from_name = nullptr;

        char* mesh_to_name = nullptr;

        int num_contacts = 0;

        int mesh_from_type = -1;

        int mesh_to_type = -1;

        int start_index = 0;

        int is_spherical = 0;
    };
}
