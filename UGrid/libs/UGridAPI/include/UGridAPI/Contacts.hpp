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

#include <UGridAPI/MeshLocations.hpp>

namespace ugridapi
{
    /// @brief A struct used to describe UGrid contact in a C-compatible manner
    struct Contacts
    {
        /// @brief The name of the contact entity
        char* name = nullptr;

        /// @brief The actual contact, expressed as pair of indices from a mesh index to another mesh index
        int* edges = nullptr;

        /// @brief For each contact its type
        int* contact_type = nullptr;

        /// @brief The name of each contact
        char* contact_name_id = nullptr;

        /// @brief The long name of each contact
        char* contact_name_long = nullptr;

        /// @brief The name of the mesh where the contact start
        char* mesh_from_name = nullptr;

        /// @brief The name of the mesh where the contact ends
        char* mesh_to_name = nullptr;

        /// @brief The location type (node, edge or face) at the contact start
        MeshLocations mesh_from_location = MeshLocations::Unknown;

        /// @brief The location type (node, edge or face) at the contact end
        MeshLocations mesh_to_location = MeshLocations::Unknown;

        /// @brief The number of contact
        int num_contacts = 0;
    };
} // namespace ugridapi
