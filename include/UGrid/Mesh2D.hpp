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

#include <UGridApi/Mesh2D.hpp>

#include <UGrid/UGridEntity.hpp>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{

    /// @brief A class implementing the methods for reading/writing a mesh2d in UGrid format
    struct Mesh2D : UGridEntity
    {
        /// @brief Constructor setting the NetCDF file
        /// @param nc_file The NetCDF file pointer
        explicit Mesh2D(std::shared_ptr<netCDF::NcFile> nc_file);

        /// @brief Constructor setting nc_file and all internal state
        /// @param nc_file The nc file pointer
        /// @param topology_variable The mesh2d name
        /// @param entity_attributes The topological attributes (key value pair with key the topological attribute name and value the associated vector of variables)
        /// @param entity_attribute_names The topological attributes names (key value pair with key the topological attribute name and value the associated vector of variables names)
        /// @param entity_dimensions The dimensions associated with the mesh2d (key value pair with key the dimension enumeration and value the associated NetCDF dimension)
        Mesh2D(
            std::shared_ptr<netCDF::NcFile> nc_file,
            netCDF::NcVar const& topology_variable,
            std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
            std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
            std::map<UGridFileDimensions, netCDF::NcDim> const& entity_dimensions);

        /// @brief Defines the mesh2d header
        /// @param mesh2d The mesh2d api structure with the fields to write and all optional flags
        void define(ugridapi::Mesh2D const& mesh2d);

        /// @brief Writes a mesh2d to file
        /// @param mesh2d mesh2d The mesh2d api structure with the fields to write and all optional flags
        void put(ugridapi::Mesh2D const& mesh2d);

        /// @brief Inquires the mesh2d dimensions
        /// @param mesh2d The mesh2d api structure with the fields where to assign the dimensions
        void inquire(ugridapi::Mesh2D& mesh2d) const;

        /// @brief Inquires the mesh2d arrays
        /// @param mesh2d The mesh2d api structure with the fields where to assign the data
        void get(ugridapi::Mesh2D& mesh2d) const;

        /// @brief The dimensionality of a Mesh2D
        /// @return The dimensionality
        static int get_dimensionality() { return 2; }
    };
} // namespace ugrid
