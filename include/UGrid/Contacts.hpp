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

#include <UGridApi/Contacts.hpp>

#include <UGrid/UGridEntity.hpp>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    /// @brief A class implementing the methods for reading/writing contacts in UGrid format
    struct Contacts : UGridEntity
    {
        /// @brief Constructor setting the NetCDF file
        /// @param nc_file The NetCDF file pointer
        explicit Contacts(std::shared_ptr<netCDF::NcFile> nc_file) : UGridEntity(nc_file)
        {
        }

        /// @brief Constructor setting nc_file and all internal state
        /// @param nc_file The NcFile file pointer
        /// @param entity_name The contacts name
        /// @param entity_attributes The topological attributes (key value pair with key the topological attribute name and value the associated vector of variables)
        /// @param entity_attribute_names The topological attributes names (key value pair with key the topological attribute name and value the associated vector of variables names)
        /// @param entity_dimensions The dimensions associated with the contacts (key value pair with key the dimension enumeration and value the associated NetCDF dimension)
        explicit Contacts(
            std::shared_ptr<netCDF::NcFile> nc_file,
            netCDF::NcVar const& topology_variable,
            std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
            std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
            std::map<UGridFileDimensions, netCDF::NcDim> const& entity_dimensions);

        /// @brief Defines the contacts header (ug_write_mesh_arrays)
        /// @param contacts The contacts api structure with the fields to write and all optional flags
        void define(ugridapi::Contacts const& contacts);

        /// @brief Writes a contacts to file
        /// @param contacts contacts The contacts api structure with the fields to write and all optional flags
        void put(ugridapi::Contacts const& contacts);

        /// @brief Inquires the contacts dimensions
        /// @param contacts The contacts api structure with the fields where to assign the dimensions
        void inquire(ugridapi::Contacts& contacts) const;

        /// @brief Inquires the contacts arrays
        /// @param contacts The contacts api structure with the fields where to assign the data
        void get(ugridapi::Contacts& contacts) const;

        /// @brief Function containing the criteria to determine if a variable is a mesh topology contact
        /// @param attributes The file attributes
        /// @return True if is a mesh topology contact, false otherwise
        static bool is_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes);

        /// @brief A function to determine if a topology variable has a matching dimensionality
        /// @param attributes [in] The variable attributes
        /// @param entity_dimensionality [in] The sought dimensionality
        /// @return If The topology variable has a matching functionality
        static bool has_matching_dimensionality(std::map<std::string, netCDF::NcVarAtt> const& attributes, int entity_dimensionality)
        {
            return true;
        }

        /// @brief Get the dimensionality of a Network1D
        /// @return The dimensionality
        static int get_dimensionality() { return 1; }

    private:
        std::string m_mesh_from_name = "";
        std::string m_mesh_to_name = "";
        UGridEntityLocations m_mesh_from_location = UGridEntityLocations::invalid_location;
        UGridEntityLocations m_mesh_to_location = UGridEntityLocations::invalid_location;
    };
} // namespace ugrid
