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

#if defined(_WIN32)
#if !defined(UGRID_API)
#define UGRID_API __declspec(dllexport)
#endif
#else
#define UGRID_API __attribute__((visibility("default")))
#endif

#include <map>
#include <sstream>
#include <vector>

#include <ncFile.h>

#include <UGrid/Constants.hpp>
#include <UGrid/Mesh2D.hpp>
#include <UGrid/Operations.hpp>
#include <UGrid/UGridEntity.hpp>
#include <UGridApi/UGrid.hpp>
#include <UGridApi/UGridState.hpp>

/// \namespace ugridapi
/// @brief Contains all structs and functions exposed at the API level
namespace ugridapi
{
    static std::map<int, UGridState> ugrid_states;
    static char exceptionMessage[512] = "";

    int HandleExceptions(const std::exception_ptr exceptionPtr)
    {
        try
        {
            std::rethrow_exception(exceptionPtr);
        }
        catch (const std::exception& e)
        {
            strcpy(exceptionMessage, e.what());
            return Exception;
        }
    }

    std::unique_ptr<ugrid::UGridEntity> get_topology(int file_id, int topology_id, int topology_type)
    {
        if (topology_type == Network1dTopology)
        {
            return std::make_unique<ugrid::UGridEntity>(ugrid_states[file_id].m_network1d[topology_id]);
        }
        if (topology_type == Mesh1dTopology)
        {
            return std::make_unique<ugrid::UGridEntity>(ugrid_states[file_id].m_mesh1d[topology_id]);
        }
        if (topology_type == Mesh2dTopology)
        {
            return std::make_unique<ugrid::UGridEntity>(ugrid_states[file_id].m_mesh2d[topology_id]);
        }
        if (topology_type == ContactsTopology)
        {
            return std::make_unique<ugrid::UGridEntity>(ugrid_states[file_id].m_contacts[topology_id]);
        }
    }

    /// @brief Gets all attributes values of a netCDF variable
    /// @param nc_var [in] The netCDF variable
    /// @return The attributes values
    static std::vector<std::string> get_attributes_values_as_strings(netCDF::NcVar const& nc_var)
    {
        std::vector<std::string> result;
        auto const attributes = nc_var.getAtts();
        for (auto const& [name, value] : attributes)
        {
            if (value.getType() == netCDF::NcType::nc_INT)
            {
                auto const attribute_length = value.getAttLength();
                std::vector<int> items(attribute_length);
                value.getValues(&items[0]);
                std::stringstream os;
                for (auto const& item : items)
                {
                    os << item << " ";
                }
                result.emplace_back(os.str());
            }
            else if (value.getType() == netCDF::NcType::nc_CHAR)
            {
                std::string item;
                value.getValues(item);
                result.emplace_back(item);
            }
            else
            {
                throw std::invalid_argument("get_attributes_values_as_strings: Invalid attribute value type.");
            }
        }
        return result;
    }

    /// @brief Gets all values of a data variable
    /// @tparam T The value type
    /// @param file_id The file id
    /// @param data_variable_name The name of the data variable
    /// @param data The retrieved data
    template <typename T>
    void get_data_array(int file_id, const char* data_variable_name, T& data)
    {
        // Gets the variable name
        const auto variable_name = ugrid::char_array_to_string(data_variable_name, ugrid::name_long_length);

        // Gets all variables
        const auto vars = ugrid_states[file_id].m_ncFile->getVars();

        // Finds the data variables using its name
        const auto it = vars.find(variable_name);
        if (it == vars.end())
        {
            throw std::invalid_argument("get_data_array: The variable name is not present in the netcdf file.");
        }

        // Gets the data for all time steps
        it->second.getVar(&data);
    }

    UGRID_API int ug_error_get(const char*& error_message)
    {
        error_message = exceptionMessage;
        return Success;
    }

    UGRID_API int ug_name_get_length(int& length)
    {
        length = static_cast<int>(ugrid::name_length);
        return Success;
    }

    UGRID_API int ug_name_get_long_length(int& length)
    {
        length = static_cast<int>(ugrid::name_long_length);
        return Success;
    }

    UGRID_API int ug_entity_get_node_location_enum(int& location)
    {
        location = static_cast<int>(ugrid::UGridEntityLocations::node);
        return Success;
    }

    UGRID_API int ug_entity_get_edge_location_enum(int& location)
    {
        location = static_cast<int>(ugrid::UGridEntityLocations::edge);
        return Success;
    }

    UGRID_API int ug_entity_get_face_location_enum(int& location)
    {
        location = static_cast<int>(ugrid::UGridEntityLocations::face);
        return Success;
    }

    UGRID_API int ug_topology_get_network1d_enum(int& topology_enum)
    {
        topology_enum = static_cast<int>(Network1dTopology);
        return Success;
    }

    UGRID_API int ug_topology_get_mesh1d_enum(int& topology_enum)
    {
        topology_enum = static_cast<int>(Mesh1dTopology);
        return Success;
    }

    UGRID_API int ug_topology_get_mesh2d_enum(int& topology_enum)
    {
        topology_enum = static_cast<int>(Mesh2dTopology);
        return Success;
    }

    UGRID_API int ug_topology_get_contacts_enum(int& topology_enum)
    {
        topology_enum = static_cast<int>(ContactsTopology);
        return Success;
    }

    UGRID_API int ug_topology_get_count(int file_id, int topology_type, int& topology_count)
    {
        if (topology_type == Network1dTopology)
        {
            topology_count = static_cast<int>(ugrid_states[file_id].m_network1d.size());
        }
        if (topology_type == Mesh1dTopology)
        {
            topology_count = static_cast<int>(ugrid_states[file_id].m_mesh1d.size());
        }
        if (topology_type == Mesh2dTopology)
        {
            topology_count = static_cast<int>(ugrid_states[file_id].m_mesh2d.size());
        }
        if (topology_type == ContactsTopology)
        {
            topology_count = static_cast<int>(ugrid_states[file_id].m_contacts.size());
        }
        return Success;
    }

    UGRID_API int ug_topology_count_data_variables(int file_id, int topology_type, int topology_id, int location, int& data_variable_count)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            auto const topology = get_topology(file_id, topology_id, topology_type);

            auto const location_string = ugrid::from_location_integer_to_location_string(location);

            auto const data_variables_names = topology->get_data_variables_names(location_string);

            // count data variables
            data_variable_count = data_variables_names.size();
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_topology_get_data_variables_names(int file_id, int topology_type, int topology_id, int location, char* data_variables_names_result)
    {

        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            auto const topology = get_topology(file_id, topology_id, topology_type);

            auto const location_string = ugrid::from_location_integer_to_location_string(location);

            auto const data_variables_names = topology->get_data_variables_names(location_string);

            ugrid::vector_of_strings_to_char_array(data_variables_names, ugrid::name_long_length, data_variables_names_result);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_count_attributes(int file_id, char const* variable_name, int& attributes_count)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            // Get the variable name
            const auto name = ugrid::char_array_to_string(variable_name, ugrid::name_long_length);

            // Get all variables
            const auto vars = ugrid_states[file_id].m_ncFile->getVars();

            // Find the variable name
            const auto it = vars.find(name);
            if (it == vars.end())
            {
                throw std::invalid_argument("ug_variable_count_dimensions: The variable name is not present in the netcdf file.");
            }

            // Get the dimensions
            attributes_count = it->second.getAtts().size();
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_get_attributes_values(int file_id, char const* variable_name, char* values)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            // Get the variable name
            const auto name = ugrid::char_array_to_string(variable_name, ugrid::name_long_length);

            // Get all variables
            const auto vars = ugrid_states[file_id].m_ncFile->getVars();

            // Find the variable name
            const auto it = vars.find(name);
            if (it == vars.end())
            {
                throw std::invalid_argument("ug_variable_get_attributes_values: The variable name is not present in the netcdf file.");
            }

            // Get the attribute values
            auto const attribute_values = get_attributes_values_as_strings(it->second);
            ugrid::vector_of_strings_to_char_array(attribute_values, ugrid::name_long_length, values);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_get_attributes_names(int file_id, char const* variable_name, char* names)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            // Get the variable name
            const auto name = ugrid::char_array_to_string(variable_name, ugrid::name_long_length);

            // Get all variables
            const auto vars = ugrid_states[file_id].m_ncFile->getVars();

            // Find the variable name
            const auto it = vars.find(name);
            if (it == vars.end())
            {
                throw std::invalid_argument("ug_variable_get_attributes_names: The variable name is not present in the netcdf file.");
            }

            // Get the attribute names
            auto const attributes = it->second.getAtts();
            std::vector<std::string> atrribute_names;
            for (auto const& attribute : attributes)
            {
                atrribute_names.emplace_back(attribute.second.getName());
            }
            ugrid::vector_of_strings_to_char_array(atrribute_names, ugrid::name_long_length, names);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_count_dimensions(int file_id, char const* variable_name, int& dimensions_count)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            // Get the variable name
            const auto name = ugrid::char_array_to_string(variable_name, ugrid::name_long_length);

            // Get all variables
            const auto vars = ugrid_states[file_id].m_ncFile->getVars();

            // Find the variable name
            const auto it = vars.find(name);
            if (it == vars.end())
            {
                throw std::invalid_argument("ug_variable_count_dimensions: The variable name is not present in the netcdf file.");
            }

            // Get the dimensions
            dimensions_count = it->second.getDims().size();
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_get_data_dimensions(int file_id, char const* variable_name, int* dimension_vec)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            // Get the variable name
            const auto name = ugrid::char_array_to_string(variable_name, ugrid::name_long_length);

            // Get all variables
            const auto vars = ugrid_states[file_id].m_ncFile->getVars();

            // Find the variable name
            const auto it = vars.find(name);
            if (it == vars.end())
            {
                throw std::invalid_argument("ug_variable_get_data_dimensions: The variable name is not present in the netcdf file.");
            }

            // Get the dimensions
            auto const dimensions = it->second.getDims();
            for (auto i = 0; i < dimensions.size(); ++i)
            {
                dimension_vec[i] = dimensions[i].getSize();
            }
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_get_data_double(int file_id, char const* variable_name, double* data)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            get_data_array(file_id, variable_name, *data);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_get_data_int(int file_id, char const* variable_name, int* data)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            get_data_array(file_id, variable_name, *data);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_get_data_char(int file_id, char const* variable_name, char* data)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            get_data_array(file_id, variable_name, *data);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_file_read_mode(int& mode)
    {
        mode = static_cast<int>(netCDF::NcFile::read);
        return Success;
    }

    UGRID_API int ug_file_write_mode(int& mode)
    {
        mode = static_cast<int>(netCDF::NcFile::write);
        return Success;
    }

    UGRID_API int ug_file_replace_mode(int& mode)
    {
        mode = static_cast<int>(netCDF::NcFile::replace);
        return Success;
    }

    UGRID_API int ug_file_add_coordinate_mapping(int file_id, int espg)
    {
        int exit_code = Success;
        try
        {
            //TODO: add a method for adding coordinate mappings
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_file_open(char const* file_path, int mode, int& file_id)
    {
        int exit_code = Success;
        try
        {
            if (mode == netCDF::NcFile::read)
            {
                auto const nc_file = std::make_shared<netCDF::NcFile>(file_path, netCDF::NcFile::read, netCDF::NcFile::classic);
                file_id = nc_file->getId();
                ugrid_states.insert({nc_file->getId(), UGridState(nc_file)});

                ugrid_states[file_id].m_mesh2d = ugrid::UGridEntity::create<ugrid::Mesh2D>(nc_file);
                ugrid_states[file_id].m_network1d = ugrid::Network1D::create<ugrid::Network1D>(nc_file);
                ugrid_states[file_id].m_mesh1d = ugrid::UGridEntity::create<ugrid::Mesh1D>(nc_file);
                ugrid_states[file_id].m_contacts = ugrid::UGridEntity::create<ugrid::Contacts>(nc_file);
            }
            if (mode == netCDF::NcFile::replace)
            {
                auto const nc_file = std::make_shared<netCDF::NcFile>(file_path, netCDF::NcFile::replace, netCDF::NcFile::classic);
                file_id = nc_file->getId();
                ugrid_states.insert({nc_file->getId(), UGridState(nc_file)});
            }
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_file_close(int file_id)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }
            ugrid_states[file_id].m_ncFile->close();
            ugrid_states.erase(file_id);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_network1d_def(int file_id, Network1D const& network1d_api, int& topology_id)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid::Network1D network1d(ugrid_states[file_id].m_ncFile);
            network1d.define(network1d_api);
            ugrid_states[file_id].m_network1d.emplace_back(network1d);
            topology_id = ugrid_states[file_id].m_network1d.size() - 1;
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_network1d_put(int file_id, int topology_id, Network1D const& network1d_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_network1d[topology_id].put(network1d_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_network1d_inq(int file_id, int topology_id, Network1D& network1d_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_network1d[topology_id].inquire(network1d_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_network1d_get(int file_id, int topology_id, Network1D& network1d_api)
    {
        int exit_code = Success;
        try
        {
            ugrid_states[file_id].m_network1d[topology_id].get(network1d_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_mesh1d_def(int file_id, Mesh1D const& mesh1d_api, int& topology_id)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid::Mesh1D mesh1d(ugrid_states[file_id].m_ncFile);
            mesh1d.define(mesh1d_api);
            ugrid_states[file_id].m_mesh1d.emplace_back(mesh1d);
            topology_id = ugrid_states[file_id].m_mesh1d.size() - 1;
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_mesh1d_put(int file_id, int topology_id, Mesh1D const& mesh1d_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_mesh1d[topology_id].put(mesh1d_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_mesh1d_inq(int file_id, int topology_id, Mesh1D& mesh1d_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_mesh1d[topology_id].inquire(mesh1d_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_mesh1d_get(int file_id, int topology_id, Mesh1D& mesh1d_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_mesh1d[topology_id].get(mesh1d_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_mesh2d_def(int file_id, Mesh2D const& mesh2d_api, int& topology_id)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid::Mesh2D mesh2d(ugrid_states[file_id].m_ncFile);
            mesh2d.define(mesh2d_api);
            ugrid_states[file_id].m_mesh2d.emplace_back(mesh2d);
            topology_id = ugrid_states[file_id].m_mesh2d.size() - 1;
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_mesh2d_put(int file_id, int topology_id, Mesh2D const& mesh2d_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_mesh2d[topology_id].put(mesh2d_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_mesh2d_inq(int file_id, int topology_id, Mesh2D& mesh2d_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_mesh2d[topology_id].inquire(mesh2d_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_mesh2d_get(int file_id, int topology_id, Mesh2D& mesh2d_api)
    {
        int exit_code = Success;
        try
        {
            ugrid_states[file_id].m_mesh2d[topology_id].get(mesh2d_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_contacts_def(int file_id, Contacts const& contacts_api, int& topology_id)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid::Contacts contacts(ugrid_states[file_id].m_ncFile);
            contacts.define(contacts_api);
            ugrid_states[file_id].m_contacts.emplace_back(contacts);
            topology_id = ugrid_states[file_id].m_contacts.size() - 1;
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_contacts_put(int file_id, int topology_id, Contacts const& contacts_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_contacts[topology_id].put(contacts_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_contacts_inq(int file_id, int topology_id, Contacts& contacts_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_contacts[topology_id].inquire(contacts_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_contacts_get(int file_id, int topology_id, Contacts& contacts_api)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid_states[file_id].m_contacts[topology_id].get(contacts_api);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

} // namespace ugridapi
