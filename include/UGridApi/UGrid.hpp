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

#include <UGridApi/Contacts.hpp>
#include <UGridApi/Mesh1D.hpp>
#include <UGridApi/Mesh2D.hpp>
#include <UGridApi/Network1D.hpp>

/// \namespace ugridapi
/// @brief Contains all structs and functions exposed at the API level
namespace ugridapi
{

#ifdef __cplusplus
    extern "C"
    {
#endif
        /// @brief Enumeration for the topology type
        enum UGridTopologyType
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
        UGRID_API int ug_error_get(const char*& error_message);

        /// @brief Gets the length of a name
        /// @param length [out] The length of names
        /// @return The length of a name
        /// @return Error code
        UGRID_API int ug_name_get_length(int& length);

        /// @brief Gets the length of a long name
        /// @return length [out] The length of long names
        /// @return Error code
        UGRID_API int ug_name_get_long_length(int& length);

        /// @brief Gets the integer identifying a node location
        /// @param location [out] The integer identifying a node location
        /// @return Error code
        UGRID_API int ug_entity_get_node_location_enum(int& location);

        /// @brief Gets the integer identifying an edge location
        /// @param location [out] The integer identifying an edge location
        /// @return Error code
        UGRID_API int ug_entity_get_edge_location_enum(int& location);

        /// @brief Gets the integer identifying an face location
        /// @param location [out] The integer identifying a face location
        /// @return Error code
        UGRID_API int ug_entity_get_face_location_enum(int& location);

        /// @brief Gets the integer identifying the network topology type
        /// @param topology_enum [out] The integer identifying the network topology type
        /// @return Error code
        UGRID_API int ug_topology_get_network1d_enum(int& topology_enum);

        /// @brief Gets the integer identifying the mesh1d topology type
        /// @param topology_enum [out] The integer identifying the mesh1d topology type
        /// @return Error code
        UGRID_API int ug_topology_get_mesh1d_enum(int& topology_enum);

        /// @brief Gets the integer identifying the mesh2d topology type
        /// @param topology_enum [out] The integer identifying the mesh2d topology type
        /// @return Error code
        UGRID_API int ug_topology_get_mesh2d_enum(int& topology_enum);

        /// @brief Gets the integer identifying the contact topology type
        /// @param topology_enum [out] The integer identifying the contact topology type
        /// @return Error code
        UGRID_API int ug_topology_get_contacts_enum(int& topology_enum);

        /// @brief Gets the number of topologies of a specific type
        /// @param file_id [in] The file id
        /// @param topology_type [in] The topology type
        /// @param topology_count [out] The number of topologies of the specific type
        /// @return Error code
        UGRID_API int ug_topology_get_count(int file_id, int topology_type, int& topology_count);

        /// @brief For a specific topology, counts how many attributes are present
        /// @param file_id [in] The file id
        /// @param topology_type [in] The topology type
        /// @param topology_id [in] The topology id
        /// @param attributes_count [out] The number of attributes
        /// @return Error code
        UGRID_API int ug_variable_count_attributes(int file_id, int topology_type, int topology_id, int& attributes_count);

        /// @brief For a specific topology, retrieve its attribute names
        /// @param file_id [in] The file id
        /// @param topology_type [in] The topology type
        /// @param topology_id [in] The topology id
        /// @param names [out] The variable names
        /// @return Error code
        UGRID_API int ug_variable_get_attributes_names(int file_id, int topology_type, int topology_id, char* names);

        /// @brief For a specific topology, retrieve its attribute values
        /// @param file_id [in] The file id
        /// @param topology_type [in] The topology type
        /// @param topology_id [in] The topology id
        /// @param values [out] The variable values
        /// @return Error code
        UGRID_API int ug_variable_get_attributes_values(int file_id, int topology_type, int topology_id, char* values);

        /// @brief For a specific topology and location, count how many data variables are present
        /// @param file_id [in] The file id
        /// @param topology_type [in] The topology type
        /// @param topology_id [in] The topology id
        /// @param location [in] The location on the topology (e.g. node, edge or face)
        /// @param data_variable_count [out]
        /// @return Error code
        UGRID_API int ug_topology_count_data_variables(int file_id, int topology_type, int topology_id, int location, int& data_variable_count);

        /// @brief For a specific topology and location, get all names of data variables
        /// @param file_id [in] The file id
        /// @param topology_type [in] The topology type
        /// @param topology_id [in] The topology id
        /// @param location [in] The location on the topology (e.g. node, edge or face)
        /// @param data_variables_names_result [out]
        /// @return Error code
        UGRID_API int ug_topology_get_data_variables_names(int file_id, int topology_type, int topology_id, int location, char* data_variables_names_result);

        /// @brief For a specific data variables, count the number of dimensions
        /// @param file_id [in] The file id
        /// @param data_variable_name [in] The variable name
        /// @param dimensions_count [in] The number of dimensions associated with the variable name
        /// @return Error code
        UGRID_API int ug_variable_count_dimensions(int file_id, char const* data_variable_name, int& dimensions_count);

        /// @brief For a specific data variables, gets the dimension values.
        /// @param file_id [in] The file id
        /// @param data_variable_name [in] The variable name
        /// @param dimension_vec [in] The dimension values associated with the variable name
        /// @return Error code
        UGRID_API int ug_variable_get_data_dimensions(int file_id, char const* data_variable_name, int* dimension_vec);

        /// @brief For a specific data variables, gets the data as an array of double. This might be large, because the arrays contains all time steps.
        /// @param file_id [in] The file id
        /// @param data_variable_name [in] The variable name
        /// @param data [out] The variable data
        /// @return Error code
        UGRID_API int ug_variable_get_data_double(int file_id, char const* data_variable_name, double* data);

        /// @brief For a specific data variables, gets the data as an array of int. This might be large, because the arrays contains all time steps.
        /// @param file_id [in] The file id
        /// @param data_variable_name [in] The variable name
        /// @param data [out] The variable data
        /// @return Error code
        UGRID_API int ug_variable_get_data_int(int file_id, char const* data_variable_name, int* data);

        /// @brief For a specific data variables, gets the data as an array of chars. This might be large, because the arrays contains all time steps.
        /// @param file_id [in] The file id
        /// @param data_variable_name [in] The variable name
        /// @param data [out] The variable data
        /// @return Error code
        UGRID_API int ug_variable_get_data_char(int file_id, char const* data_variable_name, char* data);

        /// @brief Gets the integer identifying the file read mode
        /// @param mode [out] the integer identifying the file read mode
        /// @return Error code
        UGRID_API int ug_file_read_mode(int& mode);

        /// @brief Gets the integer identifying the file write mode
        /// @param mode [out] The integer identifying the file write mode
        /// @return Error code
        UGRID_API int ug_file_write_mode(int& mode);

        /// @brief Gets the integer identifying the file replace mode
        /// @param mode [out] The integer identifying the file replace mode
        /// @return Error code
        UGRID_API int ug_file_replace_mode(int& mode);

        /// @brief Writes the metadata to file for identify a specific coordinate projection system
        /// @param file_id [in] The file id
        /// @param espg [in] The espg code for identify a specific coordinate projection system
        /// @return Error code
        UGRID_API int ug_file_add_coordinate_mapping(int file_id, int espg);

        /// @brief Opens a file and fills the library state
        /// @param file_path [in] The path of the file
        /// @param mode [in] The opening mode
        /// @param file_id [out] The file id
        /// @return Error code
        UGRID_API int ug_file_open(char const* file_path, int mode, int& file_id);

        /// @brief Closes a file
        /// @param file_id [in] The file id
        /// @return Error code
        UGRID_API int ug_file_close(int file_id);

        /// @brief Defines a new network1d topology
        /// @param file_id [in] The file id
        /// @param network1d_api [in] The structure containing the network data
        /// @param topology_id [out] The id of the newly created network topology
        /// @return Error code
        UGRID_API int ug_network1d_def(int file_id, Network1D const& network1d_api, int& topology_id);

        /// @brief Writes the network1d geometrical data to file
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param network1d_api [in] The structure containing the network data
        /// @return Error code
        UGRID_API int ug_network1d_put(int file_id, int topology_id, Network1D const& network1d_api);

        /// @brief Inquires the network1d dimensions and names
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param network1d_api [out] The structure containing the network data
        /// @return Error code
        UGRID_API int ug_network1d_inq(int file_id, int topology_id, Network1D& network1d_api);

        /// @brief Gets the network1d geometrical data
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param network1d_api [out] The structure containing the network data
        /// @return Error code
        UGRID_API int ug_network1d_get(int file_id, int topology_id, Network1D& network1d_api);

        /// @brief Defines a new mesh1d topology
        /// @param file_id [in] The file id
        /// @param mesh1d_api [in] The structure containing the mesh1d data
        /// @param topology_id [out] The id of the newly created mesh1d topology
        /// @return Error code
        UGRID_API int ug_mesh1d_def(int file_id, Mesh1D const& mesh1d_api, int& topology_id);

        /// @brief Writes mesh1d geometrical data to file
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param mesh1d_api [in] The structure containing the mesh1d data
        /// @return Error code
        UGRID_API int ug_mesh1d_put(int file_id, int topology_id, Mesh1D const& mesh1d_api);

        /// @brief Inquires mesh1d dimensions and names
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param mesh1d_api [out] The structure containing the mesh1d data
        /// @return Error code
        UGRID_API int ug_mesh1d_inq(int file_id, int topology_id, Mesh1D& mesh1d_api);

        /// @brief Gets mesh1d geometrical data
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param mesh1d_api [out] The structure containing the mesh1d data
        /// @return Error code
        UGRID_API int ug_mesh1d_get(int file_id, int topology_id, Mesh1D& mesh1d_api);

        /// @brief Defines a new mesh2d topology
        /// @param file_id [in] The file id
        /// @param mesh2d_api [in] The structure containing the mesh2d data
        /// @param topology_id [out] The id of the newly created mesh2d topology
        /// @return Error code
        UGRID_API int ug_mesh2d_def(int file_id, Mesh2D const& mesh2d_api, int& topology_id);

        /// @brief Writes mesh2d geometrical data to file
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param mesh2d_api [in] The structure containing the mesh2d data
        /// @return Error code
        UGRID_API int ug_mesh2d_put(int file_id, int topology_id, Mesh2D const& mesh2d_api);

        /// @brief Inquires mesh2d dimensions and names
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param mesh2d_api [out] The structure containing the mesh2d data
        /// @return Error code
        UGRID_API int ug_mesh2d_inq(int file_id, int topology_id, Mesh2D& mesh2d_api);

        /// @brief Gets mesh2d geometrical data
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param mesh2d_api [out] The structure containing the mesh2d data
        /// @return Error code
        UGRID_API int ug_mesh2d_get(int file_id, int topology_id, Mesh2D& mesh2d_api);

        /// @brief Defines a new contact topology
        /// @param file_id [in] The file id
        /// @param contacts_api [in] The structure containing the contact data
        /// @param topology_id [out] The id of the newly created contact topology
        /// @return Error code
        UGRID_API int ug_contacts_def(int file_id, Contacts const& contacts_api, int& topology_id);

        /// @brief Writes contact geometrical data to file
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param contacts_api [in] The structure containing the contact data
        /// @return Error code
        UGRID_API int ug_contacts_put(int file_id, int topology_id, Contacts const& contacts_api);

        /// @brief Inquires contact dimensions and names
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param contacts_api [out] The structure containing the contact data
        /// @return Error code
        UGRID_API int ug_contacts_inq(int file_id, int topology_id, Contacts& contacts_api);

        /// @brief Gets contact geometrical data
        /// @param file_id [in] The file id
        /// @param topology_id [in] The topology id
        /// @param contacts_api [out] The structure containing the contact data
        /// @return Error code
        UGRID_API int ug_contacts_get(int file_id, int topology_id, Contacts& contacts_api);

#ifdef __cplusplus
    }
#endif
} // namespace ugridapi
