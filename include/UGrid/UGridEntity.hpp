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

#include <map>
#include <ncFile.h>
#include <ncVar.h>
#include <string>
#include <vector>

#include <UGrid/Constants.hpp>
#include <UGrid/Operations.hpp>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    /// @brief A class containing the ids of UGrid netcdf file
    class UGridEntity
    {
    public:
        /// @brief Default constructor
        UGridEntity() = default;

        /// @brief Constructor using a pointer to NcFile
        /// @param nc_file [in] A pointer to NcFile, containing the id of an opened file
        explicit UGridEntity(const std::shared_ptr<netCDF::NcFile>& nc_file);

        /// @brief Constructor with several parameters as defined below
        /// @param nc_file [in] A pointer to NcFile, containing the id of an opened file
        /// @param topology_variable [in] The topology variable defining the entity
        /// @param attribute_variables [in] The attribute variables
        /// @param attribute_variable_names [in] The attribute names
        /// @param dimensions [in] The dimensions of the topology variable
        UGridEntity(
            std::shared_ptr<netCDF::NcFile> nc_file,
            netCDF::NcVar const& topology_variable,
            std::map<std::string, std::vector<netCDF::NcVar>> const& attribute_variables,
            std::map<std::string, std::vector<std::string>> const& attribute_variable_names,
            std::map<UGridFileDimensions, netCDF::NcDim> const& dimensions);

        /// @brief Factory method producing a vector of instances of T class
        /// @tparam T The type that needs to be created
        /// @param nc_file [in] A pointer to NcFile, containing the id of an opened file
        /// @return A vector of T class instances
        template <typename T>
        [[nodiscard]] static std::vector<T> create(std::shared_ptr<netCDF::NcFile> const& nc_file)
        {
            // gets all variables in this file
            auto const file_variables = nc_file->getVars();
            auto const file_dimensions = nc_file->getDims();

            std::vector<T> result;
            for (auto const& variable : file_variables)
            {
                auto variable_attributes = variable.second.getAtts();

                if (!T::is_topology_variable(variable_attributes))
                {
                    continue;
                }

                if (!T::has_matching_dimensionality(variable_attributes, T::get_dimensionality()))
                {
                    continue;
                }

                auto const [entity_attribute_variables, entity_attribute_names, entity_dimensions] = get_ugrid_entity(variable.second, file_dimensions, file_variables);
                result.emplace_back(nc_file,
                                    variable.second,
                                    entity_attribute_variables,
                                    entity_attribute_names,
                                    entity_dimensions);
            }

            return result;
        }

    protected:
        /// @brief A function to determine if a variable is a topology variable (this function migh get overwritten in derived if necessary)
        /// @param attributes [in] The variable attributes
        /// @return True if the variable is a topology variable
        static bool is_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes);

        /// @brief A function to determine if a topology variable has a matching dimensionality
        /// @param attributes [in] The variable attributes
        /// @param entity_dimensionality [in] The sought dimensionality
        /// @return If The topology variable has a matching functionality
        static bool has_matching_dimensionality(std::map<std::string, netCDF::NcVarAtt> const& attributes, int entity_dimensionality);

        /// @brief Defines variables for storing the coordinates of specific entity locations
        /// @param location [in] The entity location (e.g. nodes, edges, faces)
        /// @param dimension [in] The dimension of the entity location
        /// @param long_name_pattern [in] The string pattern to use for producing the long name string
        /// @param name_pattern [in] The string pattern to use for producing the name string
        void define_topological_variable_with_coordinates(UGridEntityLocations const& location,
                                                          UGridFileDimensions const& dimension,
                                                          std::string const& long_name_pattern,
                                                          std::string const& name_pattern = "%s%s");

        /// @brief Defines a new double variable on a specific entity location (e.g. nodes, edges, faces)
        /// @param variable_name [in] The variable name
        /// @param dimension [in] The dimension of the entity location
        /// @param standard_name [in] The variable standard name
        /// @param long_name [in] The variable long name
        /// @param units [in] The variable units
        /// @param double_fill_value [in] The double fill value
        /// @return The resulting variable
        [[nodiscard]] netCDF::NcVar define_variable_on_location(std::string const& variable_name,
                                                                UGridFileDimensions const& dimension,
                                                                std::string const& standard_name,
                                                                std::string const& long_name,
                                                                std::string const& units,
                                                                double const& double_fill_value = double_invalid_value);

        /// @brief Defines a new integer variable on a specific entity location (e.g. nodes, edges, faces)
        /// @param variable_name [in] The variable name
        /// @param dimension [in] The dimension of the entity location
        /// @param standard_name [in] The variable standard name
        /// @param long_name [in] The variable long name
        /// @param units [in] The variable units
        /// @param int_fill_value [in] The integer fill value
        /// @return The resulting variable
        [[nodiscard]] netCDF::NcVar define_variable_on_location(std::string const& variable_name,
                                                                UGridFileDimensions const& dimension,
                                                                std::string const& standard_name,
                                                                std::string const& long_name,
                                                                std::string const& units,
                                                                int const& int_fill_value = int_invalid_value);

        /// @brief Find the names aliases (e.g. previous naming convention used plurals)
        /// @param variable_name [in] The variable name
        /// @return An iterator to \ref m_topology_attribute_variables
        [[nodiscard]] std::map<std::string, std::vector<netCDF::NcVar>>::const_iterator find_variable_with_aliases(std::string const& variable_name) const;

        /// @brief Add topology attributes
        /// @param nc_var_attribute [in] The topology attributes
        void add_topology_attribute(netCDF::NcVarAtt const& nc_var_attribute)
        {
            m_topology_attributes.insert({nc_var_attribute.getName(), nc_var_attribute});
        }

        /// @brief Add topology attribute variables
        /// @param nc_var_attribute [in] The topology attribute
        /// @param nc_var [in] The topology attribute variable
        void add_topology_attribute_variable(netCDF::NcVarAtt const& nc_var_attribute, netCDF::NcVar const& nc_var);

        /// @brief Add variables related to a specific topology
        /// @param nc_var [in] The related variable to add
        void add_topology_related_variables(netCDF::NcVar const& nc_var)
        {
            m_related_variables.insert({nc_var.getName(), {nc_var}});
        }

        /// @brief Method collecting common operations for defining a UGrid entity to file
        /// @param entity_name [in] The entity name
        /// @param start_index [in] The start_index of indices arrays
        /// @param long_name [in] The entity long name
        /// @param topology_dimension [in] The dimension of the topology
        /// @param is_spherical [in] 1 if coordinates are in a spherical system, 0 otherwise
        void define(char* entity_name, int start_index, std::string const& long_name, int topology_dimension, int is_spherical);

        std::shared_ptr<netCDF::NcFile> m_nc_file;                                        ///< A pointer to the opened file
        netCDF::NcVar m_topology_variable;                                                ///< The topology variable
        std::map<std::string, std::vector<netCDF::NcVar>> m_topology_attribute_variables; ///< For each topology attribute, the corresponding variables
        std::map<std::string, std::vector<std::string>> m_topology_attributes_names;      ///< For each attribute, the corresponding attribute names (can be more than one separated by white spaces)
        std::map<UGridFileDimensions, netCDF::NcDim> m_dimensions;                        ///< All entity dimensions

        std::map<std::string, netCDF::NcVarAtt> m_topology_attributes; ///< The attributes of the topology variable
        std::map<std::string, netCDF::NcVar> m_related_variables;      ///< Additional variables related to the entity (foe example defined on nodes, edges or faces)
        std::string m_entity_name = "";                                ///< The name of the entity

        bool m_spherical_coordinates = false;           ///< If it is a spherical entity
        int m_start_index = 0;                          ///< The start index
        int m_int_fill_value = int_missing_value;       ///< The fill value for arrays of int
        int m_double_fill_value = double_missing_value; ///< The fill value for arrays of double
        int m_espg_code = 0;                            ///< The espg code

    private:
        /// @brief
        /// @param attribute_name
        /// @param attribute_variable
        /// @param ugrid_entity_dimension
        /// @param coordinate
        /// @param long_name_pattern
        void define_variable_with_coordinate(std::string const& attribute_name,
                                             std::string const& attribute_variable,
                                             UGridFileDimensions const& ugrid_entity_dimension,
                                             UGridEntityCoordinates const& coordinate,
                                             std::string const& long_name_pattern);

        /// @brief
        /// @param variable_name
        /// @param ugrid_entity_dimension
        /// @param nc_type
        /// @param standard_name
        /// @param long_name
        /// @param units
        /// @param int_fill_value
        /// @param double_fill_value
        /// @return
        netCDF::NcVar define_variable_on_location(std::string const& variable_name,
                                                  UGridFileDimensions const& ugrid_entity_dimension,
                                                  netCDF::NcType const& nc_type,
                                                  std::string const& standard_name,
                                                  std::string const& long_name,
                                                  std::string const& units,
                                                  int const& int_fill_value = int_invalid_value,
                                                  double const& double_fill_value = double_invalid_value);

        /// @brief
        /// @param variable
        /// @param standard_name
        /// @param long_name
        /// @param units
        /// @param int_fill_value
        /// @param double_fill_value
        void define_additional_attributes(netCDF::NcVar& variable,
                                          std::string const& standard_name,
                                          std::string const& long_name,
                                          std::string const& units,
                                          int const& int_fill_value = int_invalid_value,
                                          double const& double_fill_value = double_invalid_value);
    };
} // namespace ugrid
