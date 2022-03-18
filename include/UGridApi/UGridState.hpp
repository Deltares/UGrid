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

        /// @brief Set netcdf dimensions not related to topology
        /// @param dimension_name The dimension name
        /// @param dimension_value The dimension value
        void set_dimension(const std::string& dimension_name, const int dimension_value)
        {
            auto const it = m_dimensions.find(dimension_name);
            if (it != m_dimensions.end())
            {
                return;
            }
            auto const nc_dimension = m_ncFile->addDim(dimension_name, dimension_value);
            m_dimensions.emplace(dimension_name, nc_dimension);
        }

        /// @brief Get the nectdf dimension from its name
        /// @param dimension_name The dimension name
        /// @return The netcdf dimension
        netCDF::NcDim get_dimension(const std::string& dimension_name) const
        {
            return m_dimensions.at(dimension_name);
        }

    private:
        std::map<std::string, netCDF::NcDim> m_dimensions; ///< Dimensions not related to a topology
    };
} // namespace ugridapi
