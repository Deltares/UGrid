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
#include <UGrid/UGridEntity.hpp>

#include <UGridApi/Network1D.hpp>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    /// @brief A class implementing the methods for reading/writing a mesh2d in UGrid format
    struct Network1D : UGridEntity
    {
        /// @brief Constructor setting the NetCDF file
        /// @param nc_file The NetCDF file pointer
        explicit Network1D(std::shared_ptr<netCDF::NcFile> nc_file);

        /// @brief Constructor setting nc_file and all internal state
        /// @param nc_file The nc file pointer
        /// @param topology_variable The network topology variable
        /// @param entity_attributes The topological attributes (key value pair with key the attribute name and value the associated vector of variables)
        /// @param entity_attribute_names The topological attributes names (key value pair with key the attribute name and value the associated vector of variables names)
        /// @param entity_dimensions The dimensions associated with the mesh2d (key value pair with key the dimension enumeration and value the associated NetCDF dimension)
        explicit Network1D(
            std::shared_ptr<netCDF::NcFile> nc_file,
            netCDF::NcVar const& topology_variable,
            std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
            std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
            std::map<UGridDimensions, netCDF::NcDim> const& entity_dimensions
        );

        /// @brief Defines the network1d header (ug_create_1d_network_v1)
        /// @param mesh2d The network1d api structure with the fields to write and all optional flags  
        void define(ugridapi::Network1D const& mesh2d);

        /// @brief Writes a network1d to file
        /// @param mesh2d network1d The mesh2d api structure with the fields to write and all optional flags  
        void put(ugridapi::Network1D const& mesh2d);

        /// @brief Inquires the network1d dimensions
        /// @param mesh2d The network1d api structure with the fields where to assign the dimensions
        void inquire(ugridapi::Network1D& mesh2d) const;

        /// @brief Inquires the network1d arrays
        /// @param mesh2d The network1d api structure with the fields where to assign the data
        void get(ugridapi::Network1D& mesh2d) const;

        /// @brief A function to determine if a variable is a network variable
        /// @param attributes [in] The variable attributes
        /// @return True if the variable is a topology variable 
        static bool is_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes);

        /// @brief Get the dimensionality of a Network1D
        /// @return The dimensionality
        static int get_dimensionality() { return 1; }

    private:

        netCDF::NcVar                                      m_network_geometry_variable;                 ///< The network topology variable
        std::map<std::string, std::vector<netCDF::NcVar>>  m_network_geometry_attribute_variables;      ///< For each network attribute, the corresponding attributes
        std::map<std::string, std::vector<std::string>>    m_network_geometry_attributes_names;         ///< For each network attribute, the corresponding names
        std::map<UGridDimensions, netCDF::NcDim>           m_network_geometry_dimensions;               ///< The network entity dimensions

    };
} // namespace ugrid