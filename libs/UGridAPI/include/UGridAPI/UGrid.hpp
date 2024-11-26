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

#ifndef UGRID_API
#ifdef _WIN32
#define UGRID_API __declspec(dllexport)
#else
#define UGRID_API __attribute__((visibility("default")))
#endif
#endif

#include <UGridAPI/Contacts.hpp>
#include <UGridAPI/Mesh1D.hpp>
#include <UGridAPI/Mesh2D.hpp>
#include <UGridAPI/MeshLocations.hpp>
#include <UGridAPI/Network1D.hpp>

/// \namespace ugridapi
/// @brief Contains all structs and functions exposed at the API level
namespace ugridapi
{

#ifdef __cplusplus
    extern "C"
    {
#endif

        static const size_t error_message_buffer_size = 512;

        static const int name_length = ugrid::name_length;

        static const int name_long_length = ugrid::name_long_length;

        /// @brief Enumeration for the topology type
        enum TopologyType
        {
            Network1dTopology = 0,
            Mesh1dTopology = 1,
            Mesh2dTopology = 2,
            ContactsTopology = 3
        };

        /// @brief Enumeration for the error types
        enum UGridioApiErrors
        {
            Success = 0,
            Exception = 1,
        };

        /// @brief Gets pointer to the exception message.
        /// @param[out] error_message The pointer to the latest error message
        /// @returns Error code
        UGRID_API int ug_error_get(char* error_message);

        /// @brief Gets the length of a name
        /// @param[out] length The length of names
        /// @return The length of a name
        /// @return Error code
        UGRID_API int ug_name_get_length(int& length);

        /// @brief Gets the length of a long name
        /// @return[out] length The length of long names
        /// @return Error code
        UGRID_API int ug_name_get_long_length(int& length);

        /// @brief Gets the number of topologies of a specific type
        /// @param[in] file_id The file id
        /// @param[in] topology_type The topology type
        /// @param[in] topology_count The number of topologies of the specific type
        /// @return Error code
        UGRID_API int ug_topology_get_count(int file_id, TopologyType topology_type, int& topology_count);

        /// @brief Get the number of data variables for a specific topology on a specific location
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[in] topology_type The topology type
        /// @param[in] location The location (node, edge, face)
        /// @param[out] data_variable_count The number of data variables on the topology
        /// @return Error code
        UGRID_API int ug_topology_count_data_variables(int file_id,
                                                       TopologyType topology_type,
                                                       int topology_id,
                                                       MeshLocations location,
                                                       int& data_variable_count);

        /// @brief Get the names of data variables for a specific topology on a specific location
        /// @param[in] file_id  The file id
        /// @param[in] topology_id The topology id
        /// @param[in] topology_type The topology type
        /// @param[in] location The location on the topology (e.g. node, edge or face)
        /// @param[out] data_variables_names_result The names of the data variables
        /// @return Error code
        UGRID_API int ug_topology_get_data_variables_names(int file_id,
                                                           TopologyType topology_type,
                                                           int topology_id,
                                                           MeshLocations location,
                                                           char* data_variables_names_result);

        /// @brief Get the names of data variables for a specific topology on a specific location
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[in] topology_type The topology type
        /// @param[in] location The location on the topology (e.g. node, edge or face)
        /// @param[in] variable_name The variable name
        /// @param[in] dimension_name The name of the dimension not related to a topology (e.g "numTimeSteps")
        /// @param[in] dimension_value The dimension value
        /// @return Error code
        UGRID_API int ug_topology_define_double_variable_on_location(int file_id,
                                                                     TopologyType topology_type,
                                                                     int topology_id,
                                                                     MeshLocations location,
                                                                     const char* variable_name,
                                                                     const char* dimension_name,
                                                                     const int dimension_value);

        /// @brief Get the number of attributes of a specific variable
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[out] attributes_count The number of attributes
        /// @return Error code
        UGRID_API int ug_variable_count_attributes(int file_id, const char* variable_name, int& attributes_count);

        /// @brief Get the attributes values of a specific variable
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[out] values The attribute values
        /// @return Error code
        UGRID_API int ug_variable_get_attributes_values(int file_id, const char* variable_name, char* values);

        /// @brief Get the attributes names of a specific variable
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[out] names The attribute names
        /// @return Error code
        UGRID_API int ug_variable_get_attributes_names(int file_id, const char* variable_name, char* names);

        /// @brief Get the number of dimensions of a specific variable
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[in] dimensions_count The number of dimensions
        /// @return Error code
        UGRID_API int ug_variable_count_dimensions(int file_id, const char* variable_name, int& dimensions_count);

        /// @brief Get the dimension values of a specific variable
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[in] dimension_vec The dimension values associated with the variable name
        /// @return Error code
        UGRID_API int ug_variable_get_data_dimensions(int file_id, const char* variable_name, int* dimension_vec);

        /// @brief Get the variable data as a flat array of doubles. This might be large, because the arrays can have a large dimensionality
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[out] data The variable data
        /// @return Error code
        UGRID_API int ug_variable_get_data_double(int file_id, const char* variable_name, double* data);

        /// @brief Get the variable data as a flat array of int. This might be large, because the arrays can have a large dimensionality
        /// @param[in] file_id  The file id
        /// @param[in] variable_name The variable name
        /// @param[out] data The variable data
        /// @return Error code
        UGRID_API int ug_variable_get_data_int(int file_id, const char* variable_name, int* data);

        /// @brief Get the variable data as a flat array of char. This might be large, because the arrays can have a large dimensionality
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[out] data The variable data
        /// @return Error code
        UGRID_API int ug_variable_get_data_char(int file_id, const char* variable_name, char* data);

        /// @brief Gets the integer identifying the file read mode
        /// @param[out] mode the integer identifying the file read mode
        /// @return Error code
        UGRID_API int ug_file_read_mode(int& mode);

        /// @brief Gets the integer identifying the file write mode
        /// @param[out] mode The integer identifying the file write mode
        /// @return Error code
        UGRID_API int ug_file_write_mode(int& mode);

        /// @brief Gets the integer identifying the file replace mode
        /// @param[out] mode The integer identifying the file replace mode
        /// @return Error code
        UGRID_API int ug_file_replace_mode(int& mode) noexcept;

        /// @brief Opens a file and fills the library state
        /// @param[in] file_path  The path of the file
        /// @param[in] mode The opening mode
        /// @param[out] file_id The file id
        /// @return Error code
        UGRID_API int ug_file_open(const char* file_path, int mode, int& file_id);

        /// @brief Closes a file
        /// @param[in] file_id The file id
        /// @return Error code
        UGRID_API int ug_file_close(int file_id);

        /// @brief Defines a new network1d topology
        /// @param[in] file_id The file id
        /// @param[in] network1d_api The structure containing the network data
        /// @param[out] topology_id The id of the newly created network topology
        /// @return Error code
        UGRID_API int ug_network1d_def(int file_id, Network1D const& network1d_api, int& topology_id);

        /// @brief Writes the network1d geometrical data to file
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[in] network1d_api The structure containing the network data
        /// @return Error code
        UGRID_API int ug_network1d_put(int file_id, int topology_id, Network1D const& network1d_api);

        /// @brief Inquires the network1d dimensions and names
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[out] network1d_api The structure containing the network data
        /// @return Error code
        UGRID_API int ug_network1d_inq(int file_id, int topology_id, Network1D& network1d_api);

        /// @brief Gets the network1d geometrical data
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[out] network1d_api The structure containing the network data
        /// @return Error code
        UGRID_API int ug_network1d_get(int file_id, int topology_id, Network1D& network1d_api);

        /// @brief Defines a new mesh1d topology
        /// @param[in] file_id The file id
        /// @param[in] mesh1d_api The structure containing the mesh1d data
        /// @param[out] topology_id The id of the newly created mesh1d topology
        /// @return Error code
        UGRID_API int ug_mesh1d_def(int file_id, Mesh1D const& mesh1d_api, int& topology_id);

        /// @brief Writes mesh1d geometrical data to file
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[in] mesh1d_api The structure containing the mesh1d data
        /// @return Error code
        UGRID_API int ug_mesh1d_put(int file_id, int topology_id, Mesh1D const& mesh1d_api);

        /// @brief Inquires mesh1d dimensions and names
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[out] mesh1d_api The structure containing the mesh1d data
        /// @return Error code
        UGRID_API int ug_mesh1d_inq(int file_id, int topology_id, Mesh1D& mesh1d_api);

        /// @brief Gets mesh1d geometrical data
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[out] mesh1d_api The structure containing the mesh1d data
        /// @return Error code
        UGRID_API int ug_mesh1d_get(int file_id, int topology_id, Mesh1D& mesh1d_api);

        /// @brief Defines a new mesh2d topology
        /// @param[in] file_id The file id
        /// @param[in] mesh2d_api The structure containing the mesh2d data
        /// @param[out] topology_id The id of the newly created mesh2d topology
        /// @return Error code
        UGRID_API int ug_mesh2d_def(int file_id, Mesh2D const& mesh2d_api, int& topology_id);

        /// @brief Writes mesh2d geometrical data to file
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[in] mesh2d_api The structure containing the mesh2d data
        /// @return Error code
        UGRID_API int ug_mesh2d_put(int file_id, int topology_id, Mesh2D const& mesh2d_api);

        /// @brief Inquires mesh2d dimensions and names
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[out] mesh2d_api The structure containing the mesh2d data
        /// @return Error code
        UGRID_API int ug_mesh2d_inq(int file_id, int topology_id, Mesh2D& mesh2d_api);

        /// @brief Gets mesh2d geometrical data
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[out] mesh2d_api The structure containing the mesh2d data
        /// @return Error code
        UGRID_API int ug_mesh2d_get(int file_id, int topology_id, Mesh2D& mesh2d_api);

        /// @brief Defines a new contact topology
        /// @param[in] file_id The file id
        /// @param[in] contacts_api The structure containing the contact data
        /// @param[out] topology_id The id of the newly created contact topology
        /// @return Error code
        UGRID_API int ug_contacts_def(int file_id, Contacts const& contacts_api, int& topology_id);

        /// @brief Writes contact geometrical data to file
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[in] contacts_api The structure containing the contact data
        /// @return Error code
        UGRID_API int ug_contacts_put(int file_id, int topology_id, Contacts const& contacts_api);

        /// @brief Inquires contact dimensions and names
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[out] contacts_api The structure containing the contact data
        /// @return Error code
        UGRID_API int ug_contacts_inq(int file_id, int topology_id, Contacts& contacts_api);

        /// @brief Gets contact geometrical data
        /// @param[in] file_id The file id
        /// @param[in] topology_id The topology id
        /// @param[out] contacts_api The structure containing the contact data
        /// @return Error code
        UGRID_API int ug_contacts_get(int file_id, int topology_id, Contacts& contacts_api);

        /// @brief Defines a new integer variable
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @return Error code
        UGRID_API int ug_variable_int_define(int file_id, const char* variable_name);

        /// @brief Add integer attributes to an existing variable
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[in] attribute_name The attribute name
        /// @param[in] attribute_values The attribute values
        /// @param[in] num_values The number of attribute values
        /// @return Error code
        UGRID_API int ug_attribute_int_define(int file_id,
                                              const char* variable_name,
                                              const char* attribute_name,
                                              int const* attribute_values,
                                              int num_values);

        /// @brief Add a char attributes to an existing variable
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[in] attribute_name The attribute name
        /// @param[in] attribute_values The attribute values
        /// @param[in] num_values The number of attribute values
        /// @return Error code
        UGRID_API int ug_attribute_char_define(int file_id,
                                               const char* variable_name,
                                               const char* attribute_name,
                                               const char* attribute_values,
                                               int num_values);

        /// @brief Add a double attributes to an existing variable
        /// @param[in] file_id The file id
        /// @param[in] variable_name The variable name
        /// @param[in] attribute_name The attribute name
        /// @param[in] attribute_values The attribute values
        /// @param[in] num_values The number of attribute values
        /// @return Error code
        UGRID_API int ug_attribute_double_define(int file_id,
                                                 const char* variable_name,
                                                 const char* attribute_name,
                                                 double const* attribute_values,
                                                 int num_values);

        /// @brief Add a char attributes as global and not related to a variable
        /// @param[in] file_id The file id
        /// @param[in] attribute_name The attribute name
        /// @param[in] attribute_values The attribute values
        /// @param[in] num_values The number of values
        /// @return Error code
        UGRID_API int ug_attribute_global_char_define(int file_id,
                                                      const char* attribute_name,
                                                      const char* attribute_values,
                                                      int num_values);

        /// @brief Gets the int fill value
        /// @param[out] fillValue The int indicating the fill value
        /// @returns Error code
        UGRID_API int ug_get_int_fill_value(int& fillValue);

        /// @brief Gets the double fill value
        /// @param[out] fillValue The double indicating the fill value
        /// @returns Error code
        UGRID_API int ug_get_double_fill_value(double& fillValue);

#ifdef __cplusplus
    }
#endif
} // namespace ugridapi
