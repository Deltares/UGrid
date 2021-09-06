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


#include <UGridApi/Mesh1D.hpp>
#include <UGridApi/Mesh2D.hpp>
#include <UGridApi/Network1D.hpp>
#include <UGridApi/Contacts.hpp>

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
        /// @return The length of a name
        UGRID_API int ug_name_get_length();

        /// @brief Gets the length of a long name
        /// @return The length of a long name
        UGRID_API int ug_name_get_long_length();

        /// @brief Gets the integer identifying a node location
        /// @return The integer identifying a node location
        UGRID_API  int ug_entity_get_node_location_enum();

        /// @brief Gets the integer identifying an edge location
        /// @return The integer identifying an edge location
        UGRID_API  int ug_entity_get_edge_location_enum();

        /// @brief Gets the integer identifying an face location
        /// @return The integer identifying the face location
        UGRID_API  int ug_entity_get_face_location_enum();

        /// @brief Gets the integer identifying the network topology type
        /// @return The integer identifying the network topology type
        UGRID_API int ug_topology_get_network1d_enum();

        /// @brief Gets the integer identifying the mesh1d topology type
         /// @return The integer identifying the mesh1d topology type
        UGRID_API int ug_topology_get_mesh1d_enum();

        /// @brief Gets the integer identifying the mesh2d topology type
        /// @return The integer identifying the mesh2d topology type
        UGRID_API int ug_topology_get_mesh2d_enum();

        /// @brief Gets the integer identifying the contact topology type
        /// @return The integer identifying the contact topology type
        UGRID_API int ug_topology_get_contacts_enum();

        /// @brief Gets the number of topologies of a specific type
        /// @param file_id The file id
        /// @param topology_type The topology type
        /// @return The number of topologies of the specific type
        UGRID_API int ug_topology_get_count(int file_id, int topology_type);

        /// @brief Gets the integer identifying the file read mode
        /// @return the integer identifying the file read mode
        UGRID_API int ug_file_read_mode();

        /// @brief Gets the integer identifying the file write mode
        /// @return The integer identifying the file write mode
        UGRID_API int ug_file_write_mode();

        /// @brief Gets the integer identifying the file replace mode
        /// @return The integer identifying the file replace mode
        UGRID_API  int ug_file_replace_mode();

        /// @brief Writes the metadata to file for identify a specific coordinate projection system
        /// @param file_id [in] The file id
        /// @param espg [in] The espg code for identify a specific coordinate projection system
        /// @return Error code
        UGRID_API  int ug_file_add_coordinate_mapping(int file_id, int espg);

        /// @brief Opens a file and fills the library state
        /// @param file_path [in] The path of the file
        /// @param mode [in] The opening mode
        /// @param file_id [out] The file id
        /// @return Error code
        UGRID_API int ug_file_open(char const* file_path, int mode, int& file_id);

        /// @brief Closes a file
        /// @param file_id The file id
        /// @return Error code
        UGRID_API int ug_file_close(int file_id);

        /*----------------------------------------------------------------------------------------------------
         * Network 1d
         *---------------------------------------------------------------------------------------------------*/

        UGRID_API int ug_network1d_def(int file_id, Network1d const& network1dapi, int& topology_id);

        UGRID_API int ug_network1d_put(int file_id, int topology_id, Network1d const& network);

        UGRID_API int ug_network1d_inq(int file_id, int topology_id, Network1d& network);

        UGRID_API int ug_network1d_get(int file_id, int topology_id, Network1d& network);

        /*----------------------------------------------------------------------------------------------------
         * Mesh 1d
         *---------------------------------------------------------------------------------------------------*/

        UGRID_API int ug_mesh1d_def(int file_id, Mesh1D const& mesh1dapi, int& topology_id);

        UGRID_API int ug_mesh1d_put(int file_id, int topology_id, Mesh1D const& mesh1dapi);

        UGRID_API int ug_mesh1d_inq(int file_id, int topology_id, Mesh1D& mesh1dapi);

        UGRID_API int ug_mesh1d_get(int file_id, int topology_id, Mesh1D& mesh1dapi);

        /*----------------------------------------------------------------------------------------------------
         * Mesh 2d
         *---------------------------------------------------------------------------------------------------*/

         /// @brief Defines a new 2d mesh
         /// @param file_id 
         /// @param mesh2d 
         /// @param topology_id 
         /// @return 
        UGRID_API int ug_mesh2d_def(int file_id, Mesh2D const& mesh2d, int& topology_id);

        UGRID_API int ug_mesh2d_put(int file_id, int topology_id, Mesh2D const& mesh2d);

        UGRID_API int ug_mesh2d_inq(int file_id, int topology_id, Mesh2D& mesh2d);

        UGRID_API int ug_mesh2d_get(int file_id, int topology_id, Mesh2D& mesh2d);

        /*----------------------------------------------------------------------------------------------------
         * Contacts
        *---------------------------------------------------------------------------------------------------*/

        UGRID_API int ug_contacts_def(int file_id, Contacts  const& contacts, int& topology_id);

        UGRID_API int ug_contacts_put(int file_id, int topology_id, Contacts  const& contactsapi);

        UGRID_API int ug_contacts_inq(int file_id, int topology_id, Contacts& contactsapi);

        UGRID_API int ug_contacts_get(int file_id, int topology_id, Contacts& contactsapi);

        /*----------------------------------------------------------------------------------------------------
         * Topologies
         *---------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
    }
#endif
} // namespace ugridapi
