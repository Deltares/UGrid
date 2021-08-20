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
#include <UGrid/Operations.hpp>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    enum class UGridEntityLocations
    {
        nodes,
        edges,
        faces,
        layer,
        layer_interface,
        vertical
    };

    /// @brief A class containing the ids of UGrid netcdf file
    struct UGridEntity
    {

        UGridEntity() = default;

        explicit UGridEntity(const std::shared_ptr<netCDF::NcFile>& m_nc_file)
            : m_nc_file(m_nc_file)
        {
        }

        UGridEntity(
            std::shared_ptr<netCDF::NcFile> const& nc_file,
            std::string const& entity_name,
            std::map<std::string, std::vector<netCDF::NcVar>>& const attribute_variables,
            std::map<std::string, std::vector<std::string>> const& attribute_variable_names)
            :
            m_nc_file(nc_file),
            m_entity_name(entity_name),
            m_topology_attribute_variables(attribute_variables),
            m_topology_attributes_names(attribute_variable_names)
        {
        }


        //ug_addcoordvars
        void define_locations(UGridEntityLocations const& entity_location, int dimension, bool add_coordinate_variables)
        {
            UGridVarAttributeStringBuilder string_builder(m_entity_name);

            std::string location;
            std::string locationCapitalized;
            if (entity_location == UGridEntityLocations::nodes)
            {
                location = "node";
                locationCapitalized = "Node";
                string_builder.clear(); string_builder << "_" << location << "_lon " << m_entity_name << "_" << location << "_lat";
                m_topology_attributes.insert({ location + "_coordinates", m_topology_variable.putAtt(location + "_coordinates", string_builder.str()) });
            }
            else if (entity_location == UGridEntityLocations::edges)
            {
                location = "edge";
                locationCapitalized = "Edge";
                string_builder.clear(); string_builder << "_" << "_edge_nodes";
                m_topology_attributes.insert({ location + "_node_connectivity", m_topology_variable.putAtt(location + "_node_connectivity", string_builder.str()) });
            }
            else if (entity_location == UGridEntityLocations::faces)
            {
                location = "face";
                locationCapitalized = "Face";
                string_builder.clear(); string_builder << "_" << "_face_nodes";
                m_topology_attributes.insert({ location + "_node_connectivity", m_topology_variable.putAtt(location + "_node_connectivity", string_builder.str()) });
            }
            else if (entity_location == UGridEntityLocations::layer)
            {
                location = "layer";
                locationCapitalized = "Layers";
            }
            else if (entity_location == UGridEntityLocations::vertical)
            {
                location = "layer";
                locationCapitalized = "Layers";
            }
            else if (entity_location == UGridEntityLocations::layer_interface)
            {
                location = "interfaces";
                locationCapitalized = "nLayers: " + m_entity_name + "_nInterfaces (padding: none)";
            }
            else
            {
                throw std::invalid_argument("UGridEntity::define_locations invalid entity_location");
            }

            string_builder.clear(); string_builder << "_n" << locationCapitalized;
            m_dimensions.insert({ entity_location, m_nc_file->addDim(string_builder.str(),dimension) });
            m_topology_attributes.insert({ location + "_dimension", m_topology_variable.putAtt(location + "_dimension", string_builder.str()) });

            if (!add_coordinate_variables)
            {
                return;
            }
            std::string const coordinate_name(location + "_coordinates");
            if (!m_spherical_coordinates)
            {
                string_builder.clear(); string_builder << "_" << location << "_x " << m_entity_name << "_" << location << "_y";
                m_topology_attributes.insert({ location + "_coordinates",m_topology_variable.putAtt(location + "_coordinates", string_builder.str()) });

                string_builder.clear();  string_builder << "_" << location << "_x";
                m_topology_attribute_variables[coordinate_name].emplace_back(m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, m_dimensions[entity_location]));
                m_topology_attribute_variables[coordinate_name].back().putAtt("mesh", m_entity_name);

                m_topology_attribute_variables[coordinate_name].back().putAtt("units", "m");
                m_topology_attribute_variables[coordinate_name].back().putAtt("standard_name", "projection_x_coordinate");
                m_topology_attribute_variables[coordinate_name].back().putAtt("long_name", "x-coordinate of mesh " + location);

                string_builder.clear();  string_builder << "_" << location << "_y";
                m_topology_attribute_variables[coordinate_name].emplace_back(m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, m_dimensions[entity_location]));
                m_topology_attribute_variables[coordinate_name].back().putAtt("units", "m");
                m_topology_attribute_variables[coordinate_name].back().putAtt("standard_name", "projection_y_coordinate");
                m_topology_attribute_variables[coordinate_name].back().putAtt("long_name", "y-coordinate of mesh " + location);
            }

            if (m_spherical_coordinates)
            {
                string_builder.clear(); string_builder << "_" << location << "_lon " << m_entity_name << "_" << location << "_lat";
                m_topology_attributes.insert({ location + "_coordinates", m_topology_variable.putAtt(location + "_coordinates", string_builder.str()) });

                string_builder.clear();  string_builder << "_" << location << "_lon";
                m_topology_attribute_variables[coordinate_name].emplace_back(m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, m_dimensions[entity_location]));
                m_topology_attribute_variables[coordinate_name].back().putAtt("mesh", m_entity_name);

                m_topology_attribute_variables[coordinate_name].back().putAtt("units", "degrees_east");
                m_topology_attribute_variables[coordinate_name].back().putAtt("standard_name", "longitude");
                m_topology_attribute_variables[coordinate_name].back().putAtt("long_name", "longitude coordinate of mesh " + location);

                string_builder.clear();  string_builder << "_" << location << "_lat";
                m_topology_attribute_variables[coordinate_name].emplace_back(m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, m_dimensions[entity_location]));
                m_topology_attribute_variables[coordinate_name].back().putAtt("units", "degrees_north");
                m_topology_attribute_variables[coordinate_name].back().putAtt("standard_name", "latitude");
                m_topology_attribute_variables[coordinate_name].back().putAtt("long_name", "latitude coordinate of mesh " + location);
            }
        }

        std::shared_ptr<netCDF::NcFile>                    m_nc_file;                           /// A pointer to the opened file
        std::string                                        m_entity_name;                       /// The name of the entity
        netCDF::NcVar                                      m_topology_variable;                 /// The topology variable
        std::map<std::string, netCDF::NcVarAtt>            m_topology_attributes;
        std::map<std::string, std::vector<std::string>>    m_topology_attributes_names;          /// For each UGridEntity attribute, the corresponding names
        std::map<std::string, std::vector<netCDF::NcVar>>  m_topology_attribute_variables;               /// For each UGridEntity attribute, the corresponding attributes
        std::map<std::string, netCDF::NcVar>               m_related_variables;                 /// Variables defined on the entity (foe example on nodes, edges or faces)
        std::map<UGridEntityLocations, netCDF::NcDim>     m_dimensions;                          /// The entity dimensions
        bool m_spherical_coordinates = false;                                                   /// If it is a spherical entity
        bool m_add_node_spatial_coordinates = true;                                             /// Adds the node spatial coordinates
        bool m_add_edge_spatial_coordinates = true;                                             /// Adds the edge spatial coordinates
        bool m_add_face_spatial_coordinates = true;                                             /// Adds the face spatial coordinates
        int m_epsg;                                                                             /// The epsg code
    };
} // namespace ugrid
