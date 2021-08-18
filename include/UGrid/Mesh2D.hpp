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
#include <unordered_map>
#include <vector>

#include <ncFile.h>
#include <ncVar.h>
#include <NcVarAtt.h>
#include <UGridApi/Mesh2D.hpp>

#include "UGridEntity.hpp"

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    struct Mesh2DOptions;

    /// @brief A class containing all variable names related to a mesh2d and methods to write geometrical data to file
    struct Mesh2D : UGridEntity
    {
        /// @brief The default constructor
        Mesh2D() = default;


        /// @brief Constructor setting the nc_file 
        /// @param nc_file 
        Mesh2D(
            std::shared_ptr<netCDF::NcFile> const& nc_file) : UGridEntity(nc_file)
        {
        }

        /// @brief 
        /// @param nc_file 
        /// @param name 
        /// @param attribute_variable_names 
        Mesh2D(
            std::shared_ptr<netCDF::NcFile> const& nc_file,
            std::string const& name,
            std::map<std::string, std::vector<std::string>> const& attribute_variable_names)
            : UGridEntity(nc_file, name, attribute_variable_names)
        {
        }

        /// @brief Define mesh dimensions
        /// @param mesh2d The mesh2d  
        /// @return A NetCDF error code
        void Define(ugridapi::Mesh2D const& mesh2d);

        /// @brief Put mesh2d data
        /// @param mesh2d The mesh2d
        /// @return A NetCDF error code
        void Put(ugridapi::Mesh2D const& mesh2d);

        /// @brief Inquires the mesh dimensions
        /// @param mesh2d The mesh2d filled with dimension values
        void Inquire(ugridapi::Mesh2D& mesh2d) const;

        /// @brief Inquires the mesh arrays
        /// @param mesh2d The mesh2d with pointers to mesh arrays
        void Get(ugridapi::Mesh2D& mesh2d) const;

        /// @brief Factory method producing current class instances for the file variables
        /// @return The vector of produced class instances
        static std::vector<Mesh2D> Create(std::shared_ptr<netCDF::NcFile> const& nc_file);

    };
} // namespace ugrid
