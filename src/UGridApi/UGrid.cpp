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
#include <vector>

#include <ncFile.h>

#include <UGrid/Mesh2D.hpp>
#include <UGridApi/UGrid.hpp>
#include <UGridApi/UGridState.hpp>

/// \namespace ugridapi
/// @brief Contains all structs and functions exposed at the API level
namespace ugridapi
{
    static std::map<int, UGridState> ugrid_states;
    static char exceptionMessage[512] = "";
    static int num_instances = 0;

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

    UGRID_API int ug_open(char const* filePath, int mode, int& ugrid_id)
    {
        int exitCode = Success;
        try
        {
            if (mode == netCDF::NcFile::read)
            {

                auto ncFile = std::make_shared< netCDF::NcFile>(filePath, netCDF::NcFile::read, netCDF::NcFile::classic);
                ugrid_states.insert({ num_instances, UGridState(ncFile) });

                auto meshes = ugrid::Mesh2D::Create(ncFile);
                ugrid_states[num_instances].m_mesh2d = meshes;
                ugrid_id = num_instances;
                num_instances++;
            }
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    };

    UGRID_API int ug_close(int ugrid_id)
    {
        int exitCode = Success;
        try
        {
            int err = nc_close(ugrid_id);
            if (err != NC_NOERR)
            {
                throw std::exception();
            }
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        ugrid_states.erase(ugrid_id);
        return exitCode;
    };

    UGRID_API int ug_mesh2d_def(int ugrid_id, Mesh2D mesh2dapi, int& topology_id)
    {
        int exitCode = Success;
        try
        {
            //ugrid::Mesh2D mesh2d;
            //mesh2d.Define(mesh2dapi);
            //ugrid_states[ugrid_id].m_mesh2d.push_back(mesh2d);
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    }

    UGRID_API int ug_mesh2d_put(int ugrid_id, int topology_id, Mesh2D mesh2d)
    {
        int exitCode = Success;
        try
        {
            ugrid_states[ugrid_id].m_mesh2d[topology_id].Put(mesh2d);
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    }

    UGRID_API int ug_mesh2d_inq(int ugrid_id, int topology_id, Mesh2D& mesh2d)
    {
        int exitCode = Success;
        try
        {
            ugrid_states[ugrid_id].m_mesh2d[topology_id].Inquire(mesh2d);
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    }

    UGRID_API int ug_mesh2d_get(int ugrid_id, int topology_id, Mesh2D& mesh2d)
    {
        int exitCode = Success;
        try
        {
            ugrid_states[ugrid_id].m_mesh2d[topology_id].Get(mesh2d);
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    }

    UGRID_API int ug_network1d_def(int ugrid_id, Network1D network, int& topology_id)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_network1d_put(int ugrid_id, int topology_id, Network1D network)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_network1d_inq(int ugrid_id, int topology_id, Network1D& network)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_network1d_get(int ugrid_id, int topology_id, Network1D& network)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_mesh1d_def(int ugrid_id, Mesh1D mesh1d, int& topology_id)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_mesh1d_put(int ugrid_id, int topology_id, Mesh1D mesh1d)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_mesh1d_inq(int ugrid_id, int topology_id, Mesh1D& mesh1d)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_mesh1d_get(int ugrid_id, int topology_id, Mesh1D& mesh1d)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_contacts_def(int ugrid_id, Contacts contacts, int& topology_id)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_contacts_put(int ugrid_id, int topology_id, Contacts contacts)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_contacts_inq(int ugrid_id, int topology_id, Contacts& contacts)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_contacts_get(int ugrid_id, int topology_id, Contacts& contacts)
    {
        int exitCode = Success;
        return exitCode;
    }


    UGRID_API int ug_get_topology_num(int ugrid_id, int topology_type)
    {

        if (topology_type == Network1dTopology)
        {
            return ugrid_states[ugrid_id].m_network1d.size();
        }
        if (topology_type == Mesh1dTopology)
        {
            return ugrid_states[ugrid_id].m_mesh1d.size();
        }
        if (topology_type == Mesh2dTopology)
        {
            return ugrid_states[ugrid_id].m_mesh2d.size();
        }
        if (topology_type == ContactsTopology)
        {
            return ugrid_states[ugrid_id].m_contacts.size();
        }

        return 0;
    }

} // namespace meshkernelapi
