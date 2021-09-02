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
    class UGridEntity
    {
    public:

        UGridEntity() = default;

        explicit UGridEntity(const std::shared_ptr<netCDF::NcFile>& m_nc_file);

        UGridEntity(
            std::shared_ptr<netCDF::NcFile> nc_file,
            netCDF::NcVar const& topology_variable,
            std::map<std::string, std::vector<netCDF::NcVar>> const& attribute_variables,
            std::map<std::string, std::vector<std::string>> const& attribute_variable_names,
            std::map<UGridDimensions, netCDF::NcDim> const& dimensions);

        /// @brief Factory method producing a vector of instances of the current class (as many mesh2d are found in the file)
        /// @return The vector of produced class instances
        template<typename T>
        static std::vector<T> create(std::shared_ptr<netCDF::NcFile> const& nc_file, int entity_dimensionality)
        {
            // get all vars in this file
            const auto file_variables = nc_file->getVars();
            const auto file_dimensions = nc_file->getDims();

            std::vector<T> result;
            for (auto const& variable : file_variables)
            {
                auto variable_attributes = variable.second.getAtts();

                if (!T::is_topology_variable(variable_attributes))
                {
                    continue;
                }

                if (!T::has_matching_dimensionality(variable_attributes, entity_dimensionality))
                {
                    continue;
                }

                // entity_attribute_keys, entity_attribute_values, entity_dimensions
                auto const [entity_attribute_variables, entity_attribute_names, entity_dimensions] = get_ugrid_entity(variable.second, file_dimensions, file_variables);
                result.emplace_back(nc_file,
                    variable.second,
                    entity_attribute_variables,
                    entity_attribute_names,
                    entity_dimensions);
            }

            return result;
        };

    protected:

        /// @brief Defines the topology attributes for each location variable (name, dimension and coordinate variable)
        /// @param location 
        /// @param dimension 
        /// @param long_name_pattern 
        /// @param name_pattern 
        void define_topological_variable_with_coordinates(UGridEntityLocations const& location,
            UGridDimensions const& dimension,
            std::string const& long_name_pattern,
            std::string const& name_pattern = "%s%s");

        void define_variable_with_coordinate(
            std::string const& attribute_name,
            std::string const& attribute_variable,
            UGridDimensions const& ugrid_entity_dimension,
            UGridCoordinates const& coordinate,
            std::string const& long_name_pattern);

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


        [[nodiscard]] auto find_variable_with_aliases(std::string const& variable_name) const
        {
            // define topology variable aliases
            static std::map<std::string, std::vector<std::string>> aliases
            {
                {"node_id",{"node_id", "node_ids"}},
                {"node_long_name",{"node_long_name", "node_long_names"}},
                {"branch_id",{"branch_id", "branch_ids"}},
                {"branch_long_name",{"branch_long_name", "branch_long_names"}},
                {"contact_id",{"contact_id", "contact_ids"}},
                {"contact_long_name",{"contact_long_name", "contact_long_names"}}
            };

            auto iterator = m_topology_attribute_variables.end();
            for (auto const& alias : aliases.at(variable_name))
            {
                iterator = m_topology_attribute_variables.find(alias);
                if (iterator != m_topology_attribute_variables.end())
                {
                    break;
                }
            }
            if (iterator == m_topology_attribute_variables.end())
            {
                throw std::invalid_argument("find_variable_with_aliases: No Matching found");
            }

            return iterator;
        }

        void add_topology_attribute(netCDF::NcVarAtt const& nc_var_attribute)
        {
            m_topology_attributes.insert({ nc_var_attribute.getName(), nc_var_attribute });
        }

        void add_topology_attribute_variable(netCDF::NcVarAtt const& nc_var_attribute, netCDF::NcVar const& nc_var)
        {
            // find if an attribute variable is there already
            auto it = m_topology_attribute_variables.find(nc_var_attribute.getName());
            if (it != m_topology_attribute_variables.end())
            {
                it->second.emplace_back(nc_var);
            }
            else
            {
                m_topology_attribute_variables.insert({ nc_var_attribute.getName(), {nc_var} });
            }
        }

        void add_topology_related_variables(netCDF::NcVar const& nc_var)
        {
            m_related_variables.insert({ nc_var.getName(), {nc_var} });
        }

        void define(char* entity_name, int start_index, std::string const& long_name, int dimensionality, int is_spherical)
        {
            m_start_index = start_index;
            m_entity_name = std::string(entity_name);
            m_spherical_coordinates = is_spherical == 0 ? false : true;

            // Topology with some standard attributes
            m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_CHAR);

            auto topology_attribute = m_topology_variable.putAtt("cf_role", "mesh_topology");
            add_topology_attribute(topology_attribute);

            topology_attribute = m_topology_variable.putAtt("long_name", long_name);
            add_topology_attribute(topology_attribute);

            topology_attribute = m_topology_variable.putAtt("topology_dimension", netCDF::NcType::nc_INT, dimensionality);
            add_topology_attribute(topology_attribute);

            // Add additional dimensions, maybe required
            m_dimensions.insert({ UGridDimensions::ids, m_nc_file->addDim("strLengthIds", name_lengths) });
            m_dimensions.insert({ UGridDimensions::long_names, m_nc_file->addDim("strLengthLongNames", name_long_lengths) });
            m_dimensions.insert({ UGridDimensions::Two, m_nc_file->addDim("Two", 2) });
        }

        static bool is_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes)
        {
            if (attributes.find("cf_role") == attributes.end())
            {
                return false;
            }

            if (attributes.find("topology_dimension") == attributes.end())
            {
                return false;
            }

            // Only network1d has edge_geometry
            if (attributes.find("edge_geometry") != attributes.end())
            {
                return false;
            }
            return true;
        };

        static bool has_matching_dimensionality(std::map<std::string, netCDF::NcVarAtt> const& attributes, int entity_dimensionality)
        {
            auto const it = attributes.find("topology_dimension");
            if (it == attributes.end())
            {
                return false;
            }
            int dimensionality;
            attributes.at("topology_dimension").getValues(&dimensionality);
            if (dimensionality == entity_dimensionality)
            {
                return true;
            }
            return false;
        }

        std::shared_ptr<netCDF::NcFile>                    m_nc_file;                           /// A pointer to the opened file
        netCDF::NcVar                                      m_topology_variable;                 /// The topology variable
        std::map<std::string, std::vector<netCDF::NcVar>>  m_topology_attribute_variables;      /// For each topology attribute, the corresponding variables
        std::map<std::string, std::vector<std::string>>    m_topology_attributes_names;         /// For each attribute, the corresponding attribute names (can be more than one separated by white spaces)
        std::map<UGridDimensions, netCDF::NcDim>           m_dimensions;                        /// All entity dimensions

        std::map<std::string, netCDF::NcVarAtt>            m_topology_attributes;               /// The attributes of the topology variable
        std::map<std::string, netCDF::NcVar>               m_related_variables;                 /// Additional variables related to the entity (foe example defined on nodes, edges or faces)
        std::string                                        m_entity_name = "";                   /// The name of the entity

        bool m_spherical_coordinates = false;                                                   /// If it is a spherical entity
        int m_start_index = 0;                                                                  /// The start index
        int m_int_fill_value = int_missing_value;                                               /// The fill value for arrays of int
        int m_double_fill_value = double_missing_value;                                         /// The fill value for arrays of double
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
