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
#include <UGrid/UGridVarAttributeStringBuilder.hpp>
#include <UGridApi/Contacts.hpp>

using ugrid::Contacts;

Contacts::Contacts(
    std::shared_ptr<netCDF::NcFile> nc_file,
    netCDF::NcVar const& topology_variable,
    std::map<std::string, std::vector<netCDF::NcVar>> const& entity_attributes,
    std::map<std::string, std::vector<std::string>> const& entity_attribute_names,
    std::map<UGridFileDimensions, netCDF::NcDim> const& entity_dimensions)
    : UGridEntity(nc_file, topology_variable, entity_attributes, entity_attribute_names, entity_dimensions)
{
    // Get the name from the tokens, remove colon at the end
    m_mesh_from_name = m_topology_attributes_names.at("contact").at(0);
    if (m_mesh_from_name.back() == ':')
    {
        m_mesh_from_name.pop_back();
    }
    m_mesh_to_name = m_topology_attributes_names.at("contact").at(2);

    if (m_mesh_to_name.back() == ':')
    {
        m_mesh_to_name.pop_back();
    }

    m_mesh_from_location = from_location_string_to_location(m_topology_attributes_names.at("contact").at(1));
    m_mesh_to_location = from_location_string_to_location(m_topology_attributes_names.at("contact").at(3));
}

bool Contacts::is_topology_variable(std::map<std::string, netCDF::NcVarAtt> const& attributes)
{
    if (attributes.find("cf_role") == attributes.end())
    {
        return false;
    }

    std::string attribute_name;
    attributes.at("cf_role").getValues(attribute_name);

    if (attribute_name != "mesh_topology_contact")
    {
        return false;
    }

    return true;
}

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
        throw std::invalid_argument("Contacts::define from mesh name is empty");
    }
    if (contacts.mesh_to_name == nullptr)
    {
        throw std::invalid_argument("Contacts::define to mesh to name is empty");
    }
    if (contacts.name == nullptr)
    {
        throw std::invalid_argument("Mesh2D::define mesh name field is empty");
    }
    if (contacts.num_contacts == 0)
    {
        throw std::invalid_argument("Contacts::define no contacts available");
    }
    if (contacts.num_contacts < 0)
    {
        throw std::invalid_argument("Mesh2D::define no contacts present");
    }

    m_entity_name = std::string(contacts.name);
    rtrim(m_entity_name);

    // Add additional dimensions, maybe required
    m_dimensions.insert({UGridFileDimensions::ids, m_nc_file->addDim("strLengthIds", name_lengths)});
    m_dimensions.insert({UGridFileDimensions::long_names, m_nc_file->addDim("strLengthLongNames", name_long_lengths)});
    m_dimensions.insert({UGridFileDimensions::Two, m_nc_file->addDim(two_string, 2)});

    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);

    string_builder.clear();
    string_builder << "_nContacts";
    m_dimensions.insert({UGridFileDimensions::nodes, m_nc_file->addDim(string_builder.str(), contacts.num_contacts)});

    m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_INT, {m_dimensions[UGridFileDimensions::nodes], m_dimensions[UGridFileDimensions::Two]});
    auto topology_attribute = m_topology_variable.putAtt("cf_role", "mesh_topology_contact");
    add_topology_attribute(topology_attribute);

    auto const mesh_from_location_string = from_location_integer_to_location_string(contacts.mesh_from_location);
    auto const mesh_to_location_string = from_location_integer_to_location_string(contacts.mesh_to_location);

    std::stringstream os;
    auto mesh_from_name = std::string(contacts.mesh_from_name);
    auto mesh_to_name = std::string(contacts.mesh_to_name);
    rtrim(mesh_from_name);
    rtrim(mesh_to_name);
    os << mesh_from_name << ": " << mesh_from_location_string << " " << mesh_to_name << ": " << mesh_to_location_string;
    topology_attribute = m_topology_variable.putAtt("contact", os.str());
    add_topology_attribute(topology_attribute);

    // contact type
    string_builder.clear();
    string_builder << "_contact_type";
    topology_attribute = m_topology_variable.putAtt("contact_type", string_builder.str());
    add_topology_attribute(topology_attribute);

    auto topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_INT, {m_dimensions[UGridFileDimensions::nodes]});
    topology_attribute_variable.setFill(true, -1);

    std::vector<int> valid_range{3, 4};
    auto topology_attribute_variable_attribute = topology_attribute_variable.putAtt("valid_range", netCDF::NcType::nc_INT, 2, &valid_range[0]);
    std::vector<int> flag_values{3, 4};
    topology_attribute_variable_attribute = topology_attribute_variable.putAtt("flag_values", netCDF::NcType::nc_INT, 2, &flag_values[0]);
    topology_attribute_variable_attribute = topology_attribute_variable.putAtt("flag_meanings", "lateral_1d2d_link longitudinal_1d2d_link");
    add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

    // contact id
    string_builder.clear();
    string_builder << "_id";
    topology_attribute = m_topology_variable.putAtt("contact_id", string_builder.str());
    add_topology_attribute(topology_attribute);

    topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, {m_dimensions[UGridFileDimensions::nodes], m_dimensions[UGridFileDimensions::ids]});
    topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "ids of the contacts");
    add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

    // contact long names
    string_builder.clear();
    string_builder << "_long_name";
    topology_attribute = m_topology_variable.putAtt("contact_long_name", string_builder.str());
    add_topology_attribute(topology_attribute);

    topology_attribute_variable = m_nc_file->addVar(string_builder.str(), netCDF::NcType::nc_CHAR, {m_dimensions[UGridFileDimensions::nodes], m_dimensions[UGridFileDimensions::long_names]});
    topology_attribute_variable_attribute = topology_attribute_variable.putAtt("long_name", "long names of the contacts");
    add_topology_attribute_variable(topology_attribute, topology_attribute_variable);

    m_nc_file->enddef();
}

void Contacts::put(ugridapi::Contacts const& contacts)
{
    if (contacts.name == nullptr)
    {
        throw std::invalid_argument("Contacts::put invalid mesh name");
    }
    if (contacts.edges != nullptr)
    {
        m_topology_variable.putVar(contacts.edges);
    }
    if (auto const it = find_variable_with_aliases("contact_id"); contacts.contact_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(contacts.contact_name_id);
    }
    if (auto const it = find_variable_with_aliases("contact_long_name"); contacts.contact_name_long != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(contacts.contact_name_long);
    }
    if (auto const it = m_topology_attribute_variables.find("contact_type"); contacts.contact_type != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(contacts.contact_type);
    }
}

void Contacts::inquire(ugridapi::Contacts& contacts) const
{
    if (auto const it = m_topology_attribute_variables.find("contact_type"); it != m_topology_attribute_variables.end())
    {
        contacts.num_contacts = it->second.at(0).getDim(0).getSize();
    }
}

void Contacts::get(ugridapi::Contacts& contacts) const
{
    string_to_char_array(contacts.name, m_entity_name, name_lengths);

    string_to_char_array(contacts.mesh_from_name, m_mesh_from_name, name_lengths);

    string_to_char_array(contacts.mesh_to_name, m_mesh_to_name, name_lengths);

    if (contacts.edges != nullptr)
    {
        m_topology_variable.getVar(contacts.edges);
    }

    if (auto const it = find_variable_with_aliases("contact_id"); contacts.contact_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(contacts.contact_name_id);
    }

    if (auto const it = find_variable_with_aliases("contact_long_name"); contacts.contact_name_long != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(contacts.contact_name_long);
    }

    if (auto const it = m_topology_attribute_variables.find("contact_type"); contacts.contact_type != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(contacts.contact_type);
    }
}