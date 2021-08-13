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
    class UGridFile;

    /// @brief A class containing the ids of UGrid netcdf file
    struct Mesh2D : public UGridEntity
    {
        /// @brief The default constructor
        Mesh2D() = default;

        /// @brief Constructor producing a class instance
        /// @param topology_variable The topology variable 
        /// @param mapped_variables The variables referred in mesh topology
        explicit Mesh2D(std::map<std::string, netCDF::NcVarAtt> const& topology_variable,
            std::map<std::string, netCDF::NcVar> const& mapped_variables,
            std::map<std::string, std::vector<netCDF::NcDim>> const& mapped_variables_dimensions) :
            UGridEntity(topology_variable, mapped_variables, mapped_variables_dimensions) {};

        /// @brief Define mesh dimensions
        /// @param mes2d The mesh2d  
        /// @return A NetCDF error code
        int Define(const ugridapi::Mesh2D& mes2d);

        /// @brief Put mesh2d data
        /// @param mes2d The mesh2d
        /// @return A NetCDF error code
        int Put(const ugridapi::Mesh2D& mes2d);

        /// @brief Inquires the mesh dimensions
        /// @param mes2d The mesh2d filled with dimension values
        /// @return A NetCDF error code
        int Inquire(ugridapi::Mesh2D& mes2d) const;

        /// @brief Inquires the mesh arrays
        /// @param mes2d The mesh2d with pointers to mesh arrays
        /// @return A NetCDF error code
        int Get(ugridapi::Mesh2D& mes2d) const;

        /// @brief Factory method producing current class instances for the file variables
        /// @return The vector of produced class instances
        static std::vector<Mesh2D> Create(std::multimap<std::string, netCDF::NcVar> const& variables);

    };
} // namespace ugrid
