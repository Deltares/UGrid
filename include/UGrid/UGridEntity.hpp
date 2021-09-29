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
        /// @brief Method collecting common operations for defining a UGrid entity to file
        /// @param entity_name [in] The entity name
        /// @param start_index [in] The start_index of indices arrays
        /// @param long_name [in] The entity long name
        /// @param topology_dimension [in] The dimension of the topology
        /// @param is_spherical [in] 1 if coordinates are in a spherical system, 0 otherwise
        void define(char* entity_name, int start_index, std::string const& long_name, int topology_dimension, int is_spherical);

        /// @brief A function to determine if a variable is a topology variable (this function migh get overwritten in derived if necessary)
        /// @param attributes [in] The variable attributes
        /// @return True if the variable is a topology variable
        static bool is_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes);

        /// @brief A function to determine if a topology variable has a matching dimensionality
        /// @param attributes [in] The variable attributes
        /// @param entity_dimensionality [in] The sought dimensionality
        /// @return If The topology variable has a matching functionality
        static bool has_matching_dimensionality(std::map<std::string, netCDF::NcVarAtt> const& attributes, int entity_dimensionality);

        /// @brief Find the names aliases (e.g. previous naming convention used plurals)
        /// @param variable_name [in] The variable name
        /// @return An iterator to \ref m_topology_attribute_variables
        [[nodiscard]] std::map<std::string, std::vector<netCDF::NcVar>>::const_iterator find_variable_name_with_aliases(std::string const& variable_name) const;

        /// @brief Defines a new topological attribute
        /// @param attribute_name [in] The attribute name
        /// @param attribute_value [in] The attribute value. If none is provided, it is constructed internally using the attribute name
        void define_topological_attribute(std::string const& attribute_name, std::string const& attribute_value = "");

        /// @brief Defines a new topology variable
        /// @param variable_suffix [in] The variable name suffix, to append to the entity name to form a new variable name.
        /// @param nc_type [in] The variable type.
        /// @param ugridfile_dimensions [in] The variable dimensions (multidimensional variable are expressed as vectors).
        /// @param attributes [in] The variable attributes.
        void define_topological_variable(std::string const& variable_suffix,
                                         netCDF::NcType nc_type,
                                         std::vector<UGridFileDimensions> const& ugridfile_dimensions,
                                         std::vector<std::pair<std::string, std::string>> const& attributes = {},
                                         bool add_fill_value = false);

        /// @brief Defines a new topology-related variable.
        /// @param variable [in] The variable name suffix, to append to the entity name to form a new variable name.
        /// @param nc_type [in] The variable type.
        /// @param ugridfile_dimensions [in] The variable dimensions (multidimensional variable are expressed as vectors).
        /// @param attributes [in] The variable attributes.
        void define_topology_related_variables(std::string const& variable,
                                               netCDF::NcType nc_type,
                                               std::vector<UGridFileDimensions> const& ugridfile_dimensions,
                                               std::vector<std::pair<std::string, std::string>> const& attributes = {});

        /// @brief Defines coordinate variables based on \ref m_spherical_coordinates
        /// @param dimension [in] The dimension for which the coordinate will be added (either projected or not projected)
        /// @param long_name_pattern [in] The string pattern to use for the long names
        /// @param name_pattern [in] The string pattern to use for the names
        void define_topology_coordinates(UGridFileDimensions dimension,
                                         std::string const& long_name_pattern,
                                         std::string const& name_pattern = "%s%s");

        /// @brief Get the location attribute variable based on \ref m_spherical_coordinates value
        /// @param location [in] The location (node, edge, face)
        /// @return The location string appended with "x"/"y" or "lat"/"lon"
        std::string get_location_attribute_value(std::string const& location);

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
        /// @brief Produces the attribute variables related to coordinate locations
        /// @param location [in] The entity location (e.g. nodes, edges, faces)
        /// @param long_name_pattern [in] The string pattern to use for producing the long name string
        /// @param name_pattern [in] The string pattern to use for producing the name string
        std::tuple<std::string,
                   std::string,
                   std::string,
                   std::string,
                   std::string,
                   std::string,
                   std::string,
                   std::string>
        get_location_variable_names(std::string const& location,
                                    std::string const& long_name_pattern,
                                    std::string const& name_pattern = "%s%s");
    };
} // namespace ugrid
