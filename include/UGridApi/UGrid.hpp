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
        /// @brief Enumeration for api error types
        enum UGridioApiErrors
        {
            Success = 0,
            Exception = 1,
        };

        /// @brief Get the last library error message
        /// @param error_message 
        /// @return 
        UGRID_API int ug_get_error(const char*& error_message);

        /// @brief Opens a file and fills the state with the entities upon reading
        /// @param filePath The path of the file
        /// @param mode The netcdf opening mode
        /// @return An error if is not able to open it or it is not  
        UGRID_API int ug_open(char const* filePath, int mode);

        /// @brief Close a file
        /// @param ugrid_id 
        /// @return 
        UGRID_API int ug_close(int ugrid_id);

        UGRID_API int ug_network1d_def(int ugrid_id, Network1D network, int& topology_id);

        UGRID_API int ug_network1d_put(int ugrid_id, int topology_id, Network1D network);

        UGRID_API int ug_network1d_inq(int ugrid_id, int topology_id, Network1D& network);

        UGRID_API int ug_network1d_get(int ugrid_id, int topology_id, Network1D& network);

        UGRID_API int ug_mesh1d_def(int ugrid_id, Mesh1D mesh1d, int& topology_id);

        UGRID_API int ug_mesh1d_put(int ugrid_id, int topology_id, Mesh1D mesh1d);

        UGRID_API int ug_mesh1d_inq(int ugrid_id, int topology_id, Mesh1D& mesh1d);

        UGRID_API int ug_mesh1d_get(int ugrid_id, int topology_id, Mesh1D& mesh1d);

        UGRID_API int ug_mesh2d_def(int ugrid_id, Mesh2D mesh2d, int& topology_id);

        UGRID_API int ug_mesh2d_put(int ugrid_id, int topology_id, Mesh2D mesh2d);

        UGRID_API int ug_mesh2d_inq(int ugrid_id, int topology_id, Mesh2D& mesh2d);

        UGRID_API int ug_mesh2d_get(int ugrid_id, int topology_id, Mesh2D& mesh2d);

        UGRID_API int ug_contacts_def(int ugrid_id, Contacts contacts, int& topology_id);

        UGRID_API int ug_contacts_put(int ugrid_id, int topology_id, Contacts contacts);

        UGRID_API int ug_contacts_inq(int ugrid_id, int topology_id, Contacts& contacts);

        UGRID_API int ug_contacts_get(int ugrid_id, int topology_id, Contacts& contacts);

        /// @brief Get the number of topologies of a specific type
        /// @param ugrid_id The UGrid file id
        /// @param topology_type The topology type (0 network1d, 1 mesh1d, 2 mesh2d, 3 contacts)
        /// @return The number of topologies
        UGRID_API int ug_get_topology_num(int ugrid_id, int topology_type);

        /// @brief Get the number of topology attributes associated to a specific topology (Huite request)
        /// @param ugrid_id The UGrid file id
        /// @param topology_type The topology type (0 network1d, 1 mesh1d, 2 mesh2d, 3 contacts)
        /// @param topology_id The topology id (multiple instances are supported)
        /// @param attributes_num The number of topology attributes
        //// @return Error code
        UGRID_API int ug_get_topology_attributes_num(int ugrid_id, int topology_type, int topology_id, int& attributes_num);

        /// @brief Get the attribute names (UGRID standard names) associated to a specific topology (Huite request)
        /// @param ugrid_id The UGrid file id
        /// @param topology_type The topology type (0 network1d, 1 mesh1d, 2 mesh2d, 3 contacts)
        /// @param topology_id The topology id (multiple instances are supported)
        /// @param attributes_names The attributes names
        /// @return Error code
        UGRID_API int ug_get_topology_attributes_names(int ugrid_id, int topology_type, int topology_id, char* attributes_names);

        /// @brief Get the attribute vales associated to a specific topology (Huite request)
        /// @param ugrid_id The UGrid file id
        /// @param topology_type The topology type (0 network1d, 1 mesh1d, 2 mesh2d, 3 contacts)
        /// @param topology_id The topology id (multiple instances are supported)
        /// @param attributes_values The attributes values
        /// @return Error code
        UGRID_API int ug_get_topology_attributes_values(int ugrid_id, int topology_type, int topology_id, char* attributes_values);

        /// @brief Get the number of data variables associated to a specific topology (Huite request)
        /// @param ugrid_id The UGrid file id
        /// @param topology_type The topology type (0 network1d, 1 mesh1d, 2 mesh2d, 3 contacts) 
        /// @param topology_id The topology id (multiple instances are supported)
        /// @param var_num The number of data variable associated with the specific topology
        /// @return Error code
        UGRID_API int ug_get_data_var_attributes_num(int ugrid_id, int topology_type, int topology_id, int& var_num);

        /// @brief Get the data variables names associated to a specific topology (Huite request)
        /// @param ugrid_id The UGrid file id
        /// @param topology_type The topology type (0 network1d, 1 mesh1d, 2 mesh2d, 3 contacts)
        /// @param topology_id The topology id (multiple instances are supported)
        /// @param var_names The variable names associated with a specific topology
        /// @return Error code
        UGRID_API int ug_get_data_var_attributes_names(int ugrid_id, int topology_type, int topology_id, char* var_names);

        /// @brief Define a new data variable on a mesh topology
        /// @param ugrid_id  The UGrid file id
        /// @param topology_type The topology type (0 network1d, 1 mesh1d, 2 mesh2d, 3 contacts)
        /// @param topology_id The topology id (multiple instances are supported)
        /// @param data_variable_name The variable name
        /// @param type The variable type expressed in one of the netcdf types (e.g. nc_double).
        /// @param location Specifies at which unique mesh location data will be specified.
        /// @param standard_name Standard name (CF-compliant) for 'standard_name' attribute in this variable.
        /// @param long_name Long name for 'long_name' attribute in this variable (use empty string if not wanted).
        /// @param units Unit of this variable (CF-compliant) (use empty string for dimensionless quantities).
        /// @param cell_method Cell method for the spatial dimension (i.e., for edge/face/volume), value should be one of 'point', 'mean', etc. (See CF) (empty string if not relevant).
        /// @return Error code
        UGRID_API int ug_def_var(int ugrid_id,
            int topology_type,
            int topology_id,
            char* data_variable_name,
            int type,
            int location,
            char* standard_name,
            char* long_name,
            char* units,
            char* cell_method);

        /// @brief Inquires the if a certain variable is associated to a topology, and returns its id if true
        /// @param ugrid_id The UGrid file id
        /// @param topology_type The topology type (0 network1d, 1 mesh1d, 2 mesh2d, 3 contacts)
        /// @param topology_id The topology id (multiple instances are supported)
        /// @param var_name The variable name to inquire
        /// @param varid The resulting variable id
        /// @return Error code
        UGRID_API int ug_topology_varid_inq(int ugrid_id, int topology_type, int topology_id, char* var_name, int& varid);

        /// @brief From ugrid_id to netcdf_id
        /// @param ugrid_id The ugrid id
        /// @return Error code
        UGRID_API int ug_netcdf_id_get(int ugrid_id);

        // Put data variable
        UGRID_API int ug_put_double(int ugrid_id, int topology_type, int topology_id, char* data_variable_name, double* data);

        UGRID_API int ug_put_int(int ugrid_id, int topology_type, int topology_id, char* data_variable_name, int* data);

        UGRID_API int ug_put_char(int ugrid_id, int topology_type, int topology_id, char* data_variable_name, char* data);

        // Get a data variable (DFX, QGIS UGrid drivers)
        UGRID_API int ug_get_double(int ugrid_id, int topology_type, int topology_id, char* data_variable_name, double* data);

        UGRID_API int ug_get_int(int ugrid_id, int topology_type, int topology_id, char* data_variable_name, int* data);

        UGRID_API int ug_get_char(int ugrid_id, int topology_type, int topology_id, char* data_variable_name, char* data);


        /// to create
        /// ug_inq_varids
        /// ug_get_var_attset get all attributes associated with a variable

#ifdef __cplusplus
    }
#endif
} // namespace ugridapi
