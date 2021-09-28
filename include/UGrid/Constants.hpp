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
#include <string>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    /// @brief The UGrid entity locations
    enum class UGridEntityLocations
    {
        nodes,
        edges,
        faces,
        layer,
        layer_interface,
        vertical,
        contact,
        invalid_location
    };

    /// @brief The dimensions on a UGrid file
    enum class UGridFileDimensions
    {
        Two,
        nodes,
        nodes_geometry,
        edges,
        faces,
        layer,
        layer_interface,
        max_face_nodes,
        contacts,
        ids,
        long_names
    };

    /// @brief The UGrid Entity coordinates
    enum class UGridEntityCoordinates
    {
        x,
        y,
        lat,
        lon
    };

    size_t const name_lengths = 40;                                         ///< length of the names
    size_t const name_long_lengths = 80;                                    ///< length of the long names
    int const int_invalid_value = std::numeric_limits<int>::max();          ///< integer invalid value
    double const double_invalid_value = std::numeric_limits<double>::max(); ///< double invalid value

    int const int_missing_value = -999;                         ///< integer missing value
    double const double_missing_value = -999.0;                 ///< double missing value
    int const num_face_nodes_max = 6;                           ///< default maximum number of nodes per face
    std::string const two_string("Two");                        ///< Name of variable dimension containing two
    std::string const strLengthIds("strLengthIds");             ///< Name of variable dimension containing two
    std::string const strLengthLongNames("strLengthLongNames"); ///< Name of variable dimension containing two

} // namespace ugrid
