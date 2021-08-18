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
#include <UGrid/Constants.hpp>
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

    UGRID_API int ug_name_get_length()
    {
        return ugrid::name_lengths;
    }

    UGRID_API int ug_name_get_long_length()
    {
        return ugrid::name_long_lengths;
    }
    UGRID_API int ug_file_read_mode()
    {
        return netCDF::NcFile::read;
    }

    UGRID_API int ug_file_write_mode()
    {
        return netCDF::NcFile::write;
    }

    UGRID_API int ug_file_replace_mode()
    {
        return netCDF::NcFile::replace;
    }

    UGRID_API int ug_topology()
    {
        return netCDF::NcFile::write;
    }


    UGRID_API int ug_open(char const* filePath, int mode, int& file_id)
    {
        int exitCode = Success;
        try
        {
            if (mode == netCDF::NcFile::read)
            {
                auto const ncFile = std::make_shared< netCDF::NcFile>(filePath, netCDF::NcFile::read, netCDF::NcFile::classic);
                file_id = ncFile->getId();
                ugrid_states.insert({ ncFile->getId(), UGridState(ncFile) });

                auto const meshes = ugrid::Mesh2D::Create(ncFile);
                ugrid_states[file_id].m_mesh2d = meshes;
            }
            if (mode == netCDF::NcFile::replace)
            {
                auto const ncFile = std::make_shared< netCDF::NcFile>(filePath, netCDF::NcFile::replace, netCDF::NcFile::classic);
                file_id = ncFile->getId();

                ugrid_states.insert({ ncFile->getId(), UGridState(ncFile) });
            }
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    };

    UGRID_API int ug_close(int file_id)
    {
        int exitCode = Success;
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
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    };

    UGRID_API int ug_mesh2d_def(int file_id, Mesh2D const& mesh2dapi, int& topology_id)
    {
        int exitCode = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }

            ugrid::Mesh2D mesh2d(ugrid_states[file_id].m_ncFile);
            mesh2d.Define(mesh2dapi);
            ugrid_states[file_id].m_mesh2d.emplace_back(mesh2d);
            topology_id = ugrid_states[file_id].m_mesh2d.size() - 1;
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    }

    UGRID_API int ug_mesh2d_put(int file_id, int topology_id, Mesh2D mesh2d)
    {
        int exitCode = Success;
        try
        {
            if (ugrid_states.count(file_id) == 0)
            {
                throw std::invalid_argument("UGrid: The selected file_id does not exist.");
            }
            if (topology_id >= ugrid_states[file_id].m_mesh2d.size())
            {
                throw std::invalid_argument("UGrid: The selected topology id is larger than the number of available 2d meshes");
            }

            ugrid_states[file_id].m_mesh2d[topology_id].Put(mesh2d);
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    }

    UGRID_API int ug_mesh2d_inq(int file_id, int topology_id, Mesh2D& mesh2d)
    {
        int exitCode = Success;
        try
        {
            ugrid_states[file_id].m_mesh2d[topology_id].Inquire(mesh2d);
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    }

    UGRID_API int ug_mesh2d_get(int file_id, int topology_id, Mesh2D& mesh2d)
    {
        int exitCode = Success;
        try
        {
            ugrid_states[file_id].m_mesh2d[topology_id].Get(mesh2d);
        }
        catch (...)
        {
            exitCode = HandleExceptions(std::current_exception());
        }
        return exitCode;
    }

    UGRID_API int ug_network1d_def(int file_id, Network1D const& network, int& topology_id)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_network1d_put(int file_id, int topology_id, Network1D const& network)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_network1d_inq(int file_id, int topology_id, Network1D& network)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_network1d_get(int file_id, int topology_id, Network1D& network)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_mesh1d_def(int file_id, Mesh1D const& mesh1d, int& topology_id)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_mesh1d_put(int file_id, int topology_id, Mesh1D const& mesh1d)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_mesh1d_inq(int file_id, int topology_id, Mesh1D& mesh1d)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_mesh1d_get(int file_id, int topology_id, Mesh1D& mesh1d)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_contacts_def(int file_id, Contacts const& contacts, int& topology_id)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_contacts_put(int file_id, int topology_id, Contacts  const& contacts)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_contacts_inq(int file_id, int topology_id, Contacts& contacts)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_contacts_get(int file_id, int topology_id, Contacts& contacts)
    {
        int exitCode = Success;
        return exitCode;
    }

    UGRID_API int ug_topology_get_network1d_type()
    {
        return Network1dTopology;
    }

    UGRID_API int ug_topology_get_mesh1d_type()
    {
        return Mesh1dTopology;
    }

    UGRID_API int ug_topology_get_mesh2d_type()
    {
        return Mesh2dTopology;
    }

    UGRID_API int ug_topology_get_contacts_type()
    {
        return ContactsTopology;
    }

    UGRID_API int ug_topology_get_num(int file_id, int topology_type)
    {

        if (topology_type == Network1dTopology)
        {
            return ugrid_states[file_id].m_network1d.size();
        }
        if (topology_type == Mesh1dTopology)
        {
            return ugrid_states[file_id].m_mesh1d.size();
        }
        if (topology_type == Mesh2dTopology)
        {
            return ugrid_states[file_id].m_mesh2d.size();
        }
        if (topology_type == ContactsTopology)
        {
            return ugrid_states[file_id].m_contacts.size();
        }

        return 0;
    }

} // namespace meshkernelapi
