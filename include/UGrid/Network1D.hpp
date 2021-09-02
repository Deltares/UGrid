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
        explicit Network1D(
            std::shared_ptr<netCDF::NcFile> nc_file) : UGridEntity(nc_file)
        {
        }

        /// @brief Constructor setting nc_file and all internal state
        /// @param nc_file The nc file pointer
        /// @param entity_name The network1d name
        /// @param entity_attributes The topological attributes (key value pair with key the topological attribute name and value the associated vector of variables)
        /// @param entity_attribute_names The topological attributes names (key value pair with key the topological attribute name and value the associated vector of variables names)
        /// @param entity_dimensions The dimensions associated with the mesh2d (key value pair with key the dimension enumeration and value the associated NetCDF dimension)
        explicit Network1D(
            std::shared_ptr<netCDF::NcFile> nc_file,
            netCDF::NcVar const& topology_variable,
            std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
            std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
            std::map<UGridDimensions, netCDF::NcDim> const& entity_dimensions
        )
            : UGridEntity(nc_file, topology_variable, entity_attributes, entity_attribute_names, entity_dimensions)

        {
            // file variables and file dimensions
            auto const file_variables = nc_file->getVars();
            auto const file_dimensions = nc_file->getDims();

            // find the network geometry
            auto const entity_attribute_strings_iterator = entity_attribute_names.find("edge_geometry");
            if (entity_attribute_strings_iterator == entity_attribute_names.end())
            {
                throw std::invalid_argument("Network1D::create " + entity_attribute_strings_iterator->first + " attribute in" + m_topology_variable.getName() + " can not be found");
            }
            auto const edge_geometry_variable_name = entity_attribute_strings_iterator->second.front();
            auto const edge_geometry_variable_iterator = file_variables.find(edge_geometry_variable_name);
            if (edge_geometry_variable_iterator == file_variables.end())
            {
                throw std::invalid_argument("Network1D::create " + edge_geometry_variable_name + " variable can not be found");
            }

            auto const network_geometry_variable = edge_geometry_variable_iterator->second;
            auto const [edge_geometry_attribute_variables, edge_geometry_attribute_names, edge_geometry_entity_dimensions] =
                get_ugrid_entity(network_geometry_variable, file_dimensions, file_variables);

            m_network_geometry_variable = network_geometry_variable;
            m_network_geometry_attribute_variables = edge_geometry_attribute_variables;
            m_network_geometry_attributes_names = edge_geometry_attribute_names;
            m_network_geometry_dimensions = edge_geometry_entity_dimensions;

        }

        /// @brief Defines the network1d header (ug_create_1d_network_v1)
        /// @param mesh2d The network1d api structure with the fields to write and all optional flags  
        void define(ugridapi::Network1d const& mesh2d);

        /// @brief Writes a network1d to file
        /// @param mesh2d network1d The mesh2d api structure with the fields to write and all optional flags  
        void put(ugridapi::Network1d const& mesh2d);

        /// @brief Inquires the network1d dimensions
        /// @param mesh2d The network1d api structure with the fields where to assign the dimensions
        void inquire(ugridapi::Network1d& mesh2d) const;

        /// @brief Inquires the network1d arrays
        /// @param mesh2d The network1d api structure with the fields where to assign the data
        void get(ugridapi::Network1d& mesh2d) const;

        static bool is_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes)
        {
            if (attributes.find("cf_role") == attributes.end())
            {
                return false;
            }

            if (attributes.find("edge_geometry") == attributes.end())
            {
                return false;
            }
            return true;
        };


    private:

        netCDF::NcVar                                      m_network_geometry_variable;                 /// The network topology variable
        std::map<std::string, std::vector<netCDF::NcVar>>  m_network_geometry_attribute_variables;      /// For each network attribute, the corresponding attributes
        std::map<std::string, std::vector<std::string>>    m_network_geometry_attributes_names;         /// For each network attribute, the corresponding names
        std::map<UGridDimensions, netCDF::NcDim>           m_network_geometry_dimensions;               /// The network entity dimensions

    };
} // namespace ugrid