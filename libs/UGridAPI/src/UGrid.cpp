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

#if defined(_WIN32)
#if !defined(UGRID_API)
#define UGRID_API __declspec(dllexport)
#endif
#else
#define UGRID_API __attribute__((visibility("default")))
#endif

#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <ncFile.h>

#include <UGrid/Constants.hpp>
#include <UGrid/Mesh2D.hpp>
#include <UGrid/Operations.hpp>
#include <UGrid/UGridEntity.hpp>
#include <UGridAPI/UGrid.hpp>
#include <UGridAPI/UGridState.hpp>

/// \namespace ugridapi
/// @brief Contains all structs and functions exposed at the API level
namespace ugridapi
{
    static std::map<int, UGridState> ugrid_states;
    static size_t constexpr max_chars_to_copy = error_message_buffer_size - 1; // make sure destination string is null-terminated when strncpy is used
    static char exceptionMessage[error_message_buffer_size] = "";

    /// @brief Hash table mapping locations to location names
    static const std::unordered_map<MeshLocations, std::string> locations_attribute_names{
        {MeshLocations::Faces, "face"},
        {MeshLocations::Nodes, "node"},
        {MeshLocations::Edges, "edge"},

    };

    /// @brief Hash table mapping locations to ugrid dimensions
    static const std::unordered_map<MeshLocations, ugrid::UGridFileDimensions> locations_ugrid_dimensions{
        {MeshLocations::Faces, ugrid::UGridFileDimensions::face},
        {MeshLocations::Nodes, ugrid::UGridFileDimensions::node},
        {MeshLocations::Edges, ugrid::UGridFileDimensions::edge}};

    static UGridioApiErrors HandleExceptions(const std::exception_ptr exceptionPtr)
    {
        try
        {
            std::rethrow_exception(exceptionPtr);
        }
        catch (const std::exception& e)
        {
            std::strncpy(exceptionMessage, e.what(), max_chars_to_copy);
            return Exception;
        }
    }

    static std::unique_ptr<ugrid::UGridEntity> get_topology(int file_id,
                                                            TopologyType topology_type,
                                                            int topology_id)
    {
        switch (topology_type)
        {
        case Network1dTopology:
            return std::make_unique<ugrid::UGridEntity>(ugrid_states[file_id].m_network1d[topology_id]);
        case Mesh1dTopology:
            return std::make_unique<ugrid::UGridEntity>(ugrid_states[file_id].m_mesh1d[topology_id]);
        case Mesh2dTopology:
            return std::make_unique<ugrid::UGridEntity>(ugrid_states[file_id].m_mesh2d[topology_id]);
        case ContactsTopology:
            return std::make_unique<ugrid::UGridEntity>(ugrid_states[file_id].m_contacts[topology_id]);
        default:
            throw std::runtime_error("Invalid topology.");
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
            netCDF::NcType const type = value.getType();
            if (type == netCDF::NcType::nc_INT)
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
            else if (type == netCDF::NcType::nc_DOUBLE)
            {
                auto const attribute_length = value.getAttLength();
                std::vector<double> items(attribute_length);
                value.getValues(&items[0]);
                std::stringstream os;
                for (auto const& item : items)
                {
                    os << item << " ";
                }
                result.emplace_back(os.str());
            }
            else if (type == netCDF::NcType::nc_CHAR)
            {
                std::string item;
                value.getValues(item);
                result.emplace_back(item);
            }
            else
            {
                throw std::invalid_argument("get_attributes_values_as_strings: Invalid attribute value type.");
            }
            std::cout << "done with " << name << std::endl;
        }
        return result;
    }

    /// @brief Gets all values of a data variable
    /// @tparam T The value type
    /// @param file_id The file id
    /// @param data_variable_name The name of the data variable
    /// @param data The retrieved data
    template <typename T>
    static void get_data_array(int file_id, const char* data_variable_name, T& data)
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

    static netCDF::NcVar get_variable(int file_id, std::string const& name)
    {
        // Get all variables
        const auto vars = ugrid_states[file_id].m_ncFile->getVars();

        // Find the variable name
        const auto it = vars.find(name);
        if (it == vars.end())
        {
            throw std::invalid_argument("get_variable: The variable name is not present in the netcdf file.");
        }

        return it->second;
    }

    static std::string get_coordinate_variable_string(int file_id,
                                                      TopologyType topology_type,
                                                      int topology_id,
                                                      std::string const& var_name)
    {
        auto topology = get_topology(file_id, topology_type, topology_id);

        auto coordinates_vector_variables = topology->get_topology_attribute_variable(var_name);
        std::string result;
        for (size_t i = 0; i < coordinates_vector_variables.size() - 1; ++i)
        {
            result += coordinates_vector_variables[i].getName() + " ";
        }
        result += coordinates_vector_variables.back().getName();
        return result;
    }

    UGRID_API int ug_error_get(char* error_message)
    {
        int exit_code = Success;
        std::memcpy(error_message, exceptionMessage, sizeof exceptionMessage);
        return exit_code;
    }

    UGRID_API int ug_name_get_length(int& length)
    {
        int exit_code = Success;
        length = static_cast<int>(ugrid::name_length);
        return exit_code;
    }

    UGRID_API int ug_name_get_long_length(int& length)
    {
        int exit_code = Success;
        length = static_cast<int>(ugrid::name_long_length);
        return exit_code;
    }

    UGRID_API int ug_topology_get_count(int file_id, TopologyType topology_type, int& topology_count)
    {
        int exit_code = Success;
        try
        {
            switch (topology_type)
            {
            case Network1dTopology:
                topology_count = static_cast<int>(ugrid_states[file_id].m_network1d.size());
                break;
            case Mesh1dTopology:
                topology_count = static_cast<int>(ugrid_states[file_id].m_mesh1d.size());
                break;
            case Mesh2dTopology:
                topology_count = static_cast<int>(ugrid_states[file_id].m_mesh2d.size());
                break;
            case ContactsTopology:
                topology_count = static_cast<int>(ugrid_states[file_id].m_contacts.size());
                break;
            default:
                throw std::runtime_error("Invalid topology");
            }
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }

        return exit_code;
    }

    UGRID_API int ug_topology_count_data_variables(int file_id,
                                                   TopologyType topology_type,
                                                   int topology_id,
                                                   MeshLocations location,
                                                   int& data_variable_count)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            auto const topology = get_topology(file_id, topology_type, topology_id);

            auto const location_string = ugrid::from_location_integer_to_location_string(static_cast<int>(location));

            auto const data_variables_names = topology->get_data_variables_names(location_string);

            // count data variables
            data_variable_count = static_cast<int>(data_variables_names.size());
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_topology_get_data_variables_names(int file_id,
                                                       TopologyType topology_type,
                                                       int topology_id,
                                                       MeshLocations location,
                                                       char* data_variables_names_result)
    {

        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            auto const topology = get_topology(file_id, topology_type, topology_id);

            auto const location_string = ugrid::from_location_integer_to_location_string(static_cast<int>(location));

            auto const data_variables_names = topology->get_data_variables_names(location_string);

            ugrid::vector_of_strings_to_char_array(data_variables_names, ugrid::name_long_length, data_variables_names_result);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_topology_define_double_variable_on_location(int file_id,
                                                                 TopologyType topology_type,
                                                                 int topology_id,
                                                                 MeshLocations location,
                                                                 const char* variable_name,
                                                                 const char* dimension_name,
                                                                 const int dimension_value)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            const auto local_variable_name = ugrid::char_array_to_string(variable_name, ugrid::name_long_length);

            const auto topology = get_topology(file_id, topology_type, topology_id);

            const auto mesh = topology->get_name();

            const auto location_str = locations_attribute_names.at(location);
            const auto coordinates = get_coordinate_variable_string(file_id, topology_type, topology_id, location_str + "_coordinates");

            // Set additional variable dimension on the file
            const auto local_dimension_name = ugrid::char_array_to_string(dimension_name, ugrid::name_long_length);
            ugrid_states[file_id].set_dimension(local_dimension_name, dimension_value);

            // First dimension is the additional dimension on the file, the second variable is a topological variable
            const auto variable_first_dimension = ugrid_states[file_id].get_dimension(local_dimension_name);
            const auto variable_second_dimension = topology->get_dimension(locations_ugrid_dimensions.at(location));

            auto variable = ugrid_states[file_id].m_ncFile->addVar(local_variable_name,
                                                                   netCDF::NcType::nc_DOUBLE,
                                                                   {variable_first_dimension, variable_second_dimension});
            variable.putAtt("mesh", netCDF::NcType::nc_CHAR, mesh.size(), mesh.c_str());
            variable.putAtt("location", netCDF::NcType::nc_CHAR, location_str.size(), location_str.c_str());
            variable.putAtt("coordinates", netCDF::NcType::nc_CHAR, coordinates.size(), coordinates.c_str());
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_count_attributes(int file_id, const char* variable_name, int& attributes_count)
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
                throw std::invalid_argument("ug_variable_count_attributes: The variable name is not present in the netcdf file.");
            }

            // Get the dimensions
            attributes_count = static_cast<int>(it->second.getAtts().size());
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_get_attributes_values(int file_id, const char* variable_name, char* values)
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

    UGRID_API int ug_variable_get_attributes_names(int file_id, const char* variable_name, char* names)
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
            std::vector<std::string> attribute_names;
            for (auto const& attribute : attributes)
            {
                attribute_names.emplace_back(attribute.second.getName());
            }
            ugrid::vector_of_strings_to_char_array(attribute_names, ugrid::name_long_length, names);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_count_dimensions(int file_id, const char* variable_name, int& dimensions_count)
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
            dimensions_count = static_cast<int>(it->second.getDims().size());
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_get_data_dimensions(int file_id, const char* variable_name, int* dimension_vec)
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
            for (size_t i = 0; i < dimensions.size(); ++i)
            {
                dimension_vec[i] = static_cast<int>(dimensions[i].getSize());
            }
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_variable_get_data_double(int file_id, const char* variable_name, double* data)
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

    UGRID_API int ug_variable_get_data_int(int file_id, const char* variable_name, int* data)
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

    UGRID_API int ug_variable_get_data_char(int file_id, const char* variable_name, char* data)
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
        int exit_code = Success;
        mode = static_cast<int>(netCDF::NcFile::read);
        return exit_code;
    }

    UGRID_API int ug_file_write_mode(int& mode)
    {
        int exit_code = Success;
        mode = static_cast<int>(netCDF::NcFile::write);
        return exit_code;
    }

    UGRID_API int ug_file_replace_mode(int& mode) noexcept
    {
        int exit_code = Success;
        mode = static_cast<int>(netCDF::NcFile::replace);
        return exit_code;
    }

    UGRID_API int ug_file_open(const char* file_path, int mode, int& file_id)
    {
        int exit_code = Success;
        try
        {
            auto local_mode = static_cast<netCDF::NcFile::FileMode>(mode);
            auto const nc_file = std::make_shared<netCDF::NcFile>(file_path, local_mode, netCDF::NcFile::classic);
            file_id = nc_file->getId();
            ugrid_states.insert({nc_file->getId(), UGridState(nc_file)});

            if (mode == netCDF::NcFile::read || mode == netCDF::NcFile::write)
            {
                ugrid_states[file_id].m_mesh2d = ugrid::UGridEntity::create<ugrid::Mesh2D>(nc_file);
                ugrid_states[file_id].m_network1d = ugrid::Network1D::create<ugrid::Network1D>(nc_file);
                ugrid_states[file_id].m_mesh1d = ugrid::UGridEntity::create<ugrid::Mesh1D>(nc_file);
                ugrid_states[file_id].m_contacts = ugrid::UGridEntity::create<ugrid::Contacts>(nc_file);
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
            topology_id = static_cast<int>(ugrid_states[file_id].m_network1d.size()) - 1;
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
            topology_id = static_cast<int>(ugrid_states[file_id].m_mesh1d.size()) - 1;
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
            topology_id = static_cast<int>(ugrid_states[file_id].m_mesh2d.size()) - 1;
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
            topology_id = static_cast<int>(ugrid_states[file_id].m_contacts.size()) - 1;
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

    UGRID_API int ug_variable_int_define(int file_id, const char* variable_name)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            auto const var_name = ugrid::char_array_to_string(variable_name, ugrid::name_long_length);
            ugrid_states[file_id].m_ncFile->addVar(var_name, netCDF::NcType::nc_INT);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_attribute_int_define(int file_id,
                                          const char* variable_name,
                                          const char* att_name,
                                          int const* attribute_values,
                                          int num_values)
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

            // Get the variable
            auto const variable = get_variable(file_id, name);

            // Get the attribute name
            const auto attribute_name = ugrid::char_array_to_string(att_name, ugrid::name_long_length);

            // Put the attribute values
            variable.putAtt(attribute_name, netCDF::NcType::nc_INT, num_values, attribute_values);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_attribute_char_define(int file_id,
                                           const char* variable_name,
                                           const char* att_name,
                                           const char* attribute_values,
                                           int num_values)
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

            // Get the variable
            auto const variable = get_variable(file_id, name);

            // Get the attribute name
            const auto attribute_name = ugrid::char_array_to_string(att_name, ugrid::name_long_length);

            // Put the attribute values
            variable.putAtt(attribute_name, netCDF::NcType::nc_CHAR, num_values, attribute_values);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_attribute_double_define(int file_id,
                                             const char* variable_name,
                                             const char* att_name,
                                             double const* attribute_values,
                                             int num_values)
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

            // Get the variable
            auto const variable = get_variable(file_id, name);

            // Get the attribute name
            const auto attribute_name = ugrid::char_array_to_string(att_name, ugrid::name_long_length);

            // Put the attribute values
            variable.putAtt(attribute_name, netCDF::NcType::nc_DOUBLE, num_values, attribute_values);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_attribute_global_char_define(int file_id,
                                                  const char* attribute_name,
                                                  const char* attribute_values,
                                                  int num_values)
    {
        int exit_code = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            // Get the attribute name
            const auto attribute_name_str = ugrid::char_array_to_string(attribute_name, ugrid::name_long_length);

            // Put the attribute values
            ugrid_states[file_id].m_ncFile->putAtt(attribute_name_str, netCDF::NcType::nc_CHAR, num_values, attribute_values);
        }
        catch (...)
        {
            exit_code = HandleExceptions(std::current_exception());
        }
        return exit_code;
    }

    UGRID_API int ug_get_int_fill_value(int& fillValue)
    {
        int exit_code = Success;
        fillValue = ugrid::int_missing_value;
        return exit_code;
    }
    UGRID_API int ug_get_double_fill_value(double& fillValue)
    {
        int exit_code = Success;
        fillValue = ugrid::double_missing_value;
        return exit_code;
    }

} // namespace ugridapi
