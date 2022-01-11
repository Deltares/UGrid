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
    /// @brief A struct used to describe UGrid contact in a C-compatible manner
    struct Contacts
    {
        /// @brief The name of the contact entity
        std::string name;

        /// @brief The actual contact, expressed as pair of indices from a mesh index to another mesh index
        std::vector<int> edges;

        /// @brief For each contact its type
        std::vector<int> contact_type;

        /// @brief The name of each contact
        std::string contact_name_id;

        /// @brief The long name of each contact
        std::string contact_name_long ;

        /// @brief The name of the mesh where the contact start
        std::string mesh_from_name;

        /// @brief The name of the mesh where the contact ends
        std::string mesh_to_name;

        /// @brief The location type (node, edge or face) at the contact start
        int mesh_from_location = 0;

        /// @brief The location type (node, edge or face) at the contact end
        int mesh_to_location = 0;

        /// @brief The number of contact
        int num_contacts = 0;
    };
} // namespace ugridapi
