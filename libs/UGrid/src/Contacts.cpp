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

#include <UGrid/Contacts.hpp>
#include <UGrid/Operations.hpp>
#include <UGrid/UGridVarAttributeStringBuilder.hpp>
#include <UGridAPI/Contacts.hpp>

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
    m_entity_from_name = m_topology_attributes_variables_values.at("contact").at(0);
    if (m_entity_from_name.back() == ':')
    {
        m_entity_from_name.pop_back();
    }
    m_entity_to_name = m_topology_attributes_variables_values.at("contact").at(2);

    if (m_entity_to_name.back() == ':')
    {
        m_entity_to_name.pop_back();
    }

    m_mesh_from_location = from_location_string_to_location(m_topology_attributes_variables_values.at("contact").at(1));
    m_mesh_to_location = from_location_string_to_location(m_topology_attributes_variables_values.at("contact").at(3));
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

void Contacts::define(ugridapi::Contacts const& contacts)
{
    if (contacts.name == nullptr)
    {
        throw std::invalid_argument("Contacts::define contact name field is empty");
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
        throw std::invalid_argument("Contacts::define no contact available");
    }
    if (contacts.num_contacts < 0)
    {
        throw std::invalid_argument("Mesh2D::define no contact present");
    }

    // Define additional dimensions, maybe required
    m_dimensions.insert({UGridFileDimensions::id, m_nc_file->addDim(name_length_dimension, name_length)});
    m_dimensions.insert({UGridFileDimensions::long_name, m_nc_file->addDim(name_long_length_dimension, name_long_length)});
    m_dimensions.insert({UGridFileDimensions::Two, m_nc_file->addDim(two_string, 2)});

    // Set the entity name
    m_entity_name = std::string(contacts.name);
    right_trim_string(m_entity_name);
    auto string_builder = UGridVarAttributeStringBuilder(m_entity_name);

    // Define contact dimension
    string_builder.clear();
    string_builder << "_nContacts";
    m_dimensions.insert({UGridFileDimensions::node, m_nc_file->addDim(string_builder.str(), contacts.num_contacts)});

    // Define topology variable
    m_topology_variable = m_nc_file->addVar(m_entity_name, netCDF::NcType::nc_INT, {m_dimensions[UGridFileDimensions::node], m_dimensions[UGridFileDimensions::Two]});

    // Define topology attribute
    define_topological_attribute("cf_role", "mesh_topology_contact");

    auto const mesh_from_location_string = from_location_integer_to_location_string(static_cast<int>(contacts.mesh_from_location));
    auto const mesh_to_location_string = from_location_integer_to_location_string(static_cast<int>(contacts.mesh_to_location));

    // Define topology attribute from to mesh
    std::stringstream os;
    auto mesh_from_name = std::string(contacts.mesh_from_name);
    auto mesh_to_name = std::string(contacts.mesh_to_name);
    right_trim_string(mesh_from_name);
    right_trim_string(mesh_to_name);
    os << mesh_from_name << ": " << mesh_from_location_string << " " << mesh_to_name << ": " << mesh_to_location_string;
    define_topological_attribute("contact", os.str());

    // Define topology attribute contact type and variable
    define_topological_attribute("contact_type");
    define_topological_variable("contact_type",
                                "contact_type",
                                netCDF::NcType::nc_INT,
                                {UGridFileDimensions::node});

    m_topology_attribute_variables["contact_type"].back().setFill(true, -1);
    std::vector<int> valid_range{3, 4};
    m_topology_attribute_variables["contact_type"].back().putAtt("valid_range", netCDF::NcType::nc_INT, 2, &valid_range[0]);
    std::vector<int> flag_values{3, 4};
    m_topology_attribute_variables["contact_type"].back().putAtt("flag_values", netCDF::NcType::nc_INT, 2, &flag_values[0]);
    m_topology_attribute_variables["contact_type"].back().putAtt("flag_meanings", "lateral_1d2d_link longitudinal_1d2d_link");

    // Define topology attribute contact_id and variable
    define_topological_attribute("contact_id");
    define_topological_variable("contact_id",
                                "contact_id",
                                netCDF::NcType::nc_CHAR,
                                {UGridFileDimensions::node, UGridFileDimensions::id},
                                {{"long_name", "id of the contact"}});

    // Define topology attribute contact_long_name and variable
    define_topological_attribute("contact_long_name");
    define_topological_variable("contact_long_name",
                                "contact_long_name",
                                netCDF::NcType::nc_CHAR,
                                {UGridFileDimensions::node, UGridFileDimensions::long_name},
                                {{"long_name", "long names of the contact"}});

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
    if (auto const it = find_attribute_variable_name_with_aliases("contact_id"); contacts.contact_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).putVar(contacts.contact_name_id);
    }
    if (auto const it = find_attribute_variable_name_with_aliases("contact_long_name"); contacts.contact_name_long != nullptr && it != m_topology_attribute_variables.end())
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
        contacts.num_contacts = static_cast<int>(it->second.at(0).getDim(0).getSize());
    }
}

void Contacts::get(ugridapi::Contacts& contacts) const
{
    string_to_char_array(m_entity_name, name_long_length, contacts.name);

    string_to_char_array(m_entity_from_name, name_long_length, contacts.mesh_from_name);

    string_to_char_array(m_entity_to_name, name_long_length, contacts.mesh_to_name);

    if (contacts.edges != nullptr)
    {
        m_topology_variable.getVar(contacts.edges);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("contact_id"); contacts.contact_name_id != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(contacts.contact_name_id);
    }

    if (auto const it = find_attribute_variable_name_with_aliases("contact_long_name"); contacts.contact_name_long != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(contacts.contact_name_long);
    }

    if (auto const it = m_topology_attribute_variables.find("contact_type"); contacts.contact_type != nullptr && it != m_topology_attribute_variables.end())
    {
        it->second.at(0).getVar(contacts.contact_type);
    }
}
