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
    /// @brief The UGrid entity locations
    enum UGridEntityLocations
    {
        node = 0,
        edge = 1,
        face = 2,
        layer = 3,
        layer_interface = 4,
        vertical = 5,
        contact = 6,
        invalid_location = 7
    };

    /// @brief The dimensions on a UGrid file
    enum class UGridFileDimensions
    {
        Two,
        node,
        node_geometry,
        edge,
        face,
        layer,
        layer_interface,
        max_face_node,
        contact,
        id,
        long_name
    };

    /// @brief The UGrid Entity coordinates
    enum class UGridEntityCoordinates
    {
        x,
        y,
        lat,
        lon
    };

    size_t const name_length = 40;                                          ///< length of the names
    size_t const name_long_length = 80;                                     ///< length of the long names
    int const int_invalid_value = std::numeric_limits<int>::max();          ///< integer invalid value
    double const double_invalid_value = std::numeric_limits<double>::max(); ///< double invalid value

    int const int_missing_value = -999;                                         ///< integer missing value
    double const double_missing_value = -999.0;                                 ///< double missing value
    int const num_face_nodes_max = 6;                                           ///< default maximum number of node per face
    std::string const two_string("Two");                                        ///< Name of variable dimension containing two
    std::string const name_length_dimension("name_length_dimension");           ///< Name of variable dimension containing two
    std::string const name_long_length_dimension("name_long_length_dimension"); ///< Name of variable dimension containing two

    // define topology variable aliases
    static const std::map<std::string, std::vector<std::string>> attribute_aliases{
        // nodes
        {"node_id", {"node_id", "node_ids"}},
        {"node_long_name", {"node_long_name", "node_long_names"}},
        // edges
        {"edge_id", {"edge_id", "branch_id", "branch_ids"}},
        {"edge_long_name", {"branch_long_name", "branch_long_names", "edge_long_name"}},
        // contacts
        {"contact_id", {"contact_id", "contact_ids"}},
        {"contact_long_name", {"contact_long_name", "contact_long_names"}},
        // edge
        {"edge_length", {"edge_length", "branch_length", "branch_lengths"}},
        {"node_edge_id", {"branch_id", "branch_ids", "edge_id", "node_edge_id"}},
    };

} // namespace ugrid
