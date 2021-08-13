#pragma once

#include <UGrid/Contacts.hpp>
#include <UGrid/Mesh1D.hpp>
#include <UGrid/Mesh2D.hpp>
#include <UGrid/Network1D.hpp>

namespace ugridapi
{
    /// @brief The class holding the state of the UGridIO
    struct UGridState
    {
        explicit UGridState(const std::shared_ptr<netCDF::NcFile> m_file)
            : m_file(m_file)
        {
        }

        UGridState() = default;


        ~UGridState()
        {
            if (m_file.use_count() <= 1)
            {
                m_file->close();
            }
        }

        std::shared_ptr<netCDF::NcFile> m_file;                ///< a pointer to the opened file

        std::vector<ugrid::Mesh1D> m_mesh1d;                   ///< Mesh1D instance
        std::vector<ugrid::Network1D> m_network1d;             ///< Network1D instance
        std::vector<ugrid::Mesh2D> m_mesh2d;                   ///< Mesh2D instance
        std::vector<ugrid::Contacts> m_contacts;               ///< Contacts instance
    };
}
