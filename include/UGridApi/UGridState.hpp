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
        UGridState() = default;

        /// @brief Constructor
        /// @param nc_file [in] A pointer to a NcFile handle
        explicit UGridState(const std::shared_ptr<netCDF::NcFile>& nc_file)
            : m_ncFile(nc_file)
        {
        }

        std::shared_ptr<netCDF::NcFile> m_ncFile;  ///< A pointer to a NcFile handle
        std::vector<ugrid::Mesh1D> m_mesh1d;       ///< A vector containing all Mesh1D instances
        std::vector<ugrid::Network1D> m_network1d; ///< A vector containing all Network1D instances
        std::vector<ugrid::Mesh2D> m_mesh2d;       ///< A vector containing all Mesh2D instances
        std::vector<ugrid::Contacts> m_contacts;   ///< A vector containing all Contacts instances
    };
} // namespace ugridapi
