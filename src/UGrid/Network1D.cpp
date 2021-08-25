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

#include <ncFile.h>
#include <ncVar.h>
#include <ncDim.h>

#include <UGrid/Network1D.hpp>
#include <UGrid/Operations.hpp>

using ugrid::Network1D;

void Network1D::Define(ugridapi::Network1d const& network1d)
{
    if (network1d.name == nullptr)
    {
        throw std::invalid_argument("Network1D::Define mesh name field is empty");
    }

    m_entity_name = std::string(network1d.name);
    m_spherical_coordinates = network1d.is_spherical == 0 ? false : true;
    m_start_index = network1d.start_index;

    // add and define the topology variable
    m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_CHAR);
    m_topology_attributes.insert({ "cf_role", m_topology_variable.putAtt("cf_role", "mesh_topology") });
    m_topology_attributes.insert({ "long_name",m_topology_variable.putAtt("long_name", "Topology data of 2D mesh") });
    m_topology_attributes.insert({ "topology_dimension", m_topology_variable.putAtt("topology_dimension", netCDF::NcType::nc_INT, 2) });

    // add dimension Two 
    m_dimensions.insert({ UGridDimensions::Two, m_nc_file->addDim("Two", 2) });

    // string builder with the entity name as first part of the string
    UGridVarAttributeStringBuilder string_builder(m_entity_name);

    m_nc_file->enddef();
}

void Network1D::Put(ugridapi::Network1d const& network1d)
{
    if (network1d.name == nullptr)
    {
        throw std::invalid_argument("Network1D::Put invalid mesh name");
    }

}

void Network1D::Inquire(ugridapi::Network1d& network1d) const
{
}



void Network1D::Get(ugridapi::Network1d& network1d) const
{
    if (network1d.name != nullptr)
    {
        for (auto i = 0; i < m_entity_name.size(); ++i)
        {
            network1d.name[i] = m_entity_name[i];
        }
        network1d.name[m_entity_name.size()] = '\0';
    }
}

std::vector<Network1D> Network1D::Create(std::shared_ptr<netCDF::NcFile> const& nc_file)
{
    // Get all vars in this file
    const auto variables = nc_file->getVars();
    const auto dimensions = nc_file->getDims();

    std::vector<Network1D> result;
    for (auto const& variable : variables)
    {
        auto attributes = variable.second.getAtts();

        if (!is_mesh_topology_variable(attributes))
        {
            continue;
        }

        int dimensionality;
        attributes["topology_dimension"].getValues(&dimensionality);

        if (dimensionality == 1)
        {
            // entity_attribute_keys, entity_attribute_values, entity_dimensions
            auto const entity_name = variable.first;
            auto const [entity_attribute_keys, entity_attribute_values, entity_dimensions] = GetUGridEntity(dimensions, attributes, variables);
            result.emplace_back(nc_file, entity_name, entity_attribute_keys, entity_attribute_values, entity_dimensions);
        }
    }

    return result;
};
