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


    enum class UGridDimensions
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

    enum class UGridCoordinates
    {
        x,
        y,
        lat,
        lon
    };

    int const name_lengths = 40;
    int const name_long_lengths = 80;
    int const int_invalid_value = std::numeric_limits<int>::max();
    double const double_invalid_value = std::numeric_limits<double>::max();

    int const int_missing_value = -999;
    double const double_missing_value = -999.0;
    int const  num_face_nodes_max = 6;
    std::string const two_string("Two");

} // namespace ugrid
