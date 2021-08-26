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
#include <ncVar.h>
#include <NcVarAtt.h>
#include <UGridApi/Mesh2D.hpp>
#include <UGrid/Operations.hpp>
#include <UGrid/Constants.hpp>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{


    /// @brief A class containing the ids of UGrid netcdf file
    struct UGridEntity
    {

        UGridEntity() = default;

        explicit UGridEntity(const std::shared_ptr<netCDF::NcFile>& m_nc_file);

        UGridEntity(
            std::shared_ptr<netCDF::NcFile> nc_file,
            netCDF::NcVar const& topology_variable,
            std::map<std::string, std::vector<netCDF::NcVar>> const& attribute_variables,
            std::map<std::string, std::vector<std::string>> const& attribute_variable_names,
            std::map<UGridDimensions, netCDF::NcDim> const& dimensions);

    protected:

        /// @brief Defines the topology attributes for each location variable (name, dimension and coordinate variable)
        /// @param entity_location
        /// @param dimension
        /// @param add_coordinate_variables 
        void define_topological_variable_with_coordinates(UGridEntityLocations const& location,
            UGridDimensions const& dimension,
            bool add_coordinate_variables,
            std::string const& long_name_pattern,
            std::string const& name_pattern = "%s%s");

        void define_variable_with_coordinate(
            std::string const& attribute_name,
            std::string const& attribute_variable,
            UGridDimensions const& ugrid_entity_dimension,
            UGridCoordinates const& coordinate,
            std::string const& long_name_pattern);

        void define_topological_variable_attributes(
            std::string const& variable_name,
            netCDF::NcVar& variable,
            std::string const& long_name);

        netCDF::NcVar define_variable_on_location(std::string const& variable_name,
            UGridDimensions const& ugrid_entity_dimension,
            std::string const& standard_name,
            std::string const& long_name,
            std::string const& units,
            double const& double_fill_value = double_invalid_value);

        netCDF::NcVar define_variable_on_location(std::string const& variable_name,
            UGridDimensions const& ugrid_entity_dimension,
            std::string const& standard_name,
            std::string const& long_name,
            std::string const& units,
            int const& int_fill_value = int_invalid_value);


        std::shared_ptr<netCDF::NcFile>                    m_nc_file;                           /// A pointer to the opened file
        netCDF::NcVar                                      m_topology_variable;                 /// The topology variable
        std::map<std::string, std::vector<netCDF::NcVar>>  m_topology_attribute_variables;      /// For each attribute, the corresponding attributes
        std::map<std::string, std::vector<std::string>>    m_topology_attributes_names;         /// For each attribute, the corresponding names
        std::map<UGridDimensions, netCDF::NcDim>           m_dimensions;                        /// The entity dimensions

        std::map<std::string, netCDF::NcVarAtt>            m_topology_attributes;               /// The topology attributes
        std::map<std::string, netCDF::NcVar>               m_related_variables;                 /// Variables defined on the entity (foe example on nodes, edges or faces)
        std::string                                        m_entity_name;                       /// The entity name

        bool m_spherical_coordinates = false;                                                   /// If it is a spherical entity
        int m_start_index = 0;
        int m_int_fill_value = int_missing_value;
        int m_double_fill_value = double_missing_value;
        int m_epsg_code;                                                                        /// The epsg code

    private:

        netCDF::NcVar define_variable_on_location(std::string const& variable_name,
            UGridDimensions const& ugrid_entity_dimension,
            netCDF::NcType const& nc_type,
            std::string const& standard_name,
            std::string const& long_name,
            std::string const& units,
            int const& int_fill_value = int_invalid_value,
            double const& double_fill_value = double_invalid_value);

        void define_additional_attributes(netCDF::NcVar& variable,
            std::string const& standard_name,
            std::string const& long_name,
            std::string const& units,
            int const& int_fill_value = int_invalid_value,
            double const& double_fill_value = double_invalid_value);



    };
} // namespace ugrid
