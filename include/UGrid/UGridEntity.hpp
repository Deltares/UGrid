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
#include <vector>
#include <map>

#include <ncFile.h>
#include <NcDim.h>
#include <ncVar.h>
#include <NcVarAtt.h>
#include <UGridApi/Mesh2D.hpp>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    /// @brief A class containing the ids of UGrid netcdf file
    struct UGridEntity
    {
        UGridEntity(
            std::shared_ptr<netCDF::NcFile> const& ncFile,
            std::string const& entity_name,
            std::map<std::string, std::vector<std::string>> const& attribute_variable_names)
            :
            m_ncFile(ncFile),
            m_entity_name(entity_name),
            m_attribute_variable_names(attribute_variable_names)
        {
        }
        /// A pointer to the opened file
        std::shared_ptr<netCDF::NcFile> m_ncFile;                                               /// a reference to the nc file
        std::string m_entity_name;                                                              /// the name of the entity
        std::map<std::string, std::vector<std::string>>  m_attribute_variable_names;            /// for each UGridEntity attribute, get the corresponding names

    };
} // namespace ugrid
