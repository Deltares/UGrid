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

#include <sstream>
#include <string>

/// \namespace ugrid
/// @brief Contains the logic of the C++ static library
namespace ugrid
{
    /// @brief A class for building ugrid variable names, appending strings to the entity name
    class UGridVarAttributeStringBuilder
    {
    public:
        /// @brief Constructor setting the entity name
        /// @param name The entity name
        explicit UGridVarAttributeStringBuilder(std::string const& name) : m_name(name)
        {
            m_os << m_name;
        }

        /// @brief Clears the stream and adds the entity name
        void clear()
        {
            m_os.str(std::string());
            m_os << m_name;
        }

        /// @brief Get the stream content
        /// @return The stream content
        [[nodiscard]] std::string str() const
        {
            return m_os.str();
        }

        /// @brief Customized << operator for adding content into the internal stream
        /// @tparam T The content type
        /// @param val The content value
        /// @return The current UGridVarAttributeStringBuilder instance
        template <typename T>
        UGridVarAttributeStringBuilder& operator<<(T val)
        {
            m_os << val;
            return *this;
        }

    private:
        std::stringstream m_os; ///< The string stream, used for concatenating strings
        std::string m_name;     ///< The entity name
    };

} // namespace ugrid
