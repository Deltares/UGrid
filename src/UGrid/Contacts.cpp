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

#include <UGrid/Contacts.hpp>
#include <UGrid/Operations.hpp>
#include <UGridApi/Contacts.hpp>

using ugrid::Contacts;

// ug_create_1d_mesh_v2
// ug_def_mesh_ids
void Contacts::define(ugridapi::Contacts const& contacts)
{
    if (contacts.name == nullptr)
    {
        throw std::invalid_argument("Contacts::define contacts name field is empty");
    }
    if (contacts.mesh_from_name == nullptr)
    {
        throw std::invalid_argument("Contacts::define contacts mesh from name is empty");
    }
    if (contacts.mesh_to_name == nullptr)
    {
        throw std::invalid_argument("Contacts::define contacts mesh to name is empty");
    }
    if (contacts.num_contacts == 0)
    {
        throw std::invalid_argument("Contacts::define no contacts available");
    }

    UGridEntity::define(contacts.name, contacts.start_index, "mesh_topology_contact", 1, contacts.is_spherical);
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);

    if (contacts.num_contacts > 0)
    {
        //string_builder.clear(); string_builder << "_nNodes";
        //m_dimensions.insert({ UGridDimensions::nodes, m_nc_file->addDim(string_builder.str(), contacts.num_nodes) });
        //topology_attribute = m_topology_variable.putAtt("node_dimension", string_builder.str());
        //add_topology_attribute(topology_attribute);

        //string_builder.clear(); string_builder << "_node_branch " << m_entity_name << "_node_offset";
        //topology_attribute = m_topology_variable.putAtt("node_coordinates", string_builder.str());
        //add_topology_attribute(topology_attribute);

        //string_builder.clear(); string_builder << "_node_branch";
        //auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, m_dimensions[UGridDimensions::nodes]);
        //auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Index of branch on which mesh nodes are located");
        //add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        //string_builder.clear(); string_builder << "_node_offset";
        //topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_DOUBLE, m_dimensions[UGridDimensions::nodes]);
        //topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "Offset along branch of mesh nodes");
        //add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

        //// add node x an y if present, TODO: consider lat/lon 
        //bool const add_coordinate_variables = contacts.node_x != nullptr && contacts.node_y != nullptr;
        //if (add_coordinate_variables)
        //{
        //    define_topological_variable_with_coordinates(UGridEntityLocations::nodes, UGridDimensions::nodes, "%s of mesh nodes");
        //}

        //string_builder.clear(); string_builder << "_node_id";
        //topology_attribute = m_topology_variable.putAtt("node_name_id", string_builder.str());
        //add_topology_attribute(topology_attribute);
        //topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, { m_dimensions[UGridDimensions::nodes], m_dimensions[UGridDimensions::ids] });
        //topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "ID of mesh nodes");
        //add_topology_attribute_variable(topology_attribute, topology_attribute_variable);


        //string_builder.clear(); string_builder << "_node_long_name";
        //topology_attribute = m_topology_variable.putAtt("node_name_long", string_builder.str());
        //add_topology_attribute(topology_attribute);
        //topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, { m_dimensions[UGridDimensions::nodes], m_dimensions[UGridDimensions::long_names] });
        //topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "'Long name of mesh nodes");
        //add_topology_attribute_variable(topology_attribute, topology_attribute_variable);
    }

    m_nc_file->enddef();
}

void Contacts::put(ugridapi::Contacts const& contacts)
{
    if (contacts.name == nullptr)
    {
        throw std::invalid_argument("Contacts::put invalid mesh name");
    }

    //if (contacts.node_branch_id != nullptr)
    //{
    //    m_topology_attribute_variables.at("node_coordinates").at(0).putVar(contacts.node_branch_id);
    //}
    //if (contacts.node_branch_offset != nullptr)
    //{
    //    m_topology_attribute_variables.at("node_coordinates").at(1).putVar(contacts.node_branch_offset);
    //}
    //if (contacts.node_name_id != nullptr)
    //{
    //    m_topology_attribute_variables.at("node_name_id").at(0).putVar(contacts.node_name_id);
    //}
    //if (contacts.node_name_long != nullptr)
    //{
    //    m_topology_attribute_variables.at("node_name_long").at(0).putVar(contacts.node_name_long);
    //}
    //if (contacts.edge_nodes != nullptr)
    //{
    //    m_topology_attribute_variables.at("edge_node_connectivity").at(0).putVar(contacts.edge_nodes);
    //}
}

void Contacts::inquire(ugridapi::Contacts& contacts) const
{
    if (m_topology_attribute_variables.find("contact_type") != m_topology_attribute_variables.end())
    {
        contacts.num_contacts = m_topology_attribute_variables.at("contact_type").at(0).getDim(0).getSize();
    }
}

void Contacts::get(ugridapi::Contacts& contacts) const
{

    fill_char_array_with_string_values(contacts.name, m_entity_name);

    fill_char_array_with_string_values(contacts.mesh_from_name, m_mesh_from_name);

    fill_char_array_with_string_values(contacts.mesh_to_name, m_mesh_to_name);

    //if (contacts.contact != nullptr)
    //{
    //    m_topology_attribute_variables.at("node_coordinates").at(0).getVar(contacts.node_branch_id);
    //}
    //if (contacts.node_branch_offset != nullptr)
    //{
    //    m_topology_attribute_variables.at("node_coordinates").at(1).getVar(contacts.node_branch_offset);
    //}
    //if (contacts.edge_nodes != nullptr)
    //{
    //    m_topology_attribute_variables.at("edge_node_connectivity").at(0).getVar(contacts.edge_nodes);
    //}
    //if (contacts.node_name_id != nullptr)
    //{
    //    auto const map_iterator = find_variable_with_aliases("node_id");
    //    map_iterator->second.at(0).getVar(contacts.node_name_id);
    //}
    //if (contacts.node_name_long != nullptr)
    //{
    //    auto const map_iterator = find_variable_with_aliases("node_long_name");
    //    map_iterator->second.at(0).getVar(contacts.node_name_long);
    //}
}