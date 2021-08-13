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

#include <ncFile.h>
#include <ncVar.h>
#include <ncDim.h>

#include <UGrid/Mesh2D.hpp>
#include <UGrid/Operations.hpp>

using ugrid::Mesh2D;

int Mesh2D::Define(const ugridapi::Mesh2D& mesh2d)
{
    int netcdf_error_code = 0;

    return netcdf_error_code;
}

int Mesh2D::Put(const ugridapi::Mesh2D& mes2d)
{
    int netcdf_error_code = 0;
    return netcdf_error_code;
}

int Mesh2D::Inquire(ugridapi::Mesh2D& mesh2d) const
{
    mesh2d.num_nodes = m_dimensions.at("node_dimension").at(0).getSize();
    mesh2d.num_edges = m_dimensions.at("edge_dimension").at(0).getSize();
    mesh2d.num_faces = m_dimensions.at("face_dimension").at(0).getSize();
}

int Mesh2D::Get(ugridapi::Mesh2D& mesh2d) const
{
    int netcdf_error_code = 0;
    return netcdf_error_code;

}

std::vector<Mesh2D> Mesh2D::Create(std::multimap<std::string, netCDF::NcVar> const& variables)
{
    // Get all vars in this file
    std::vector<Mesh2D> result;
    for (auto const& variable : variables)
    {
        auto attributes = variable.second.getAtts();

        if (!is_mesh_topology_variable(attributes))
        {
            continue;
        }

        int dimensionality;
        attributes["topology_dimension"].getValues(&dimensionality);

        if (dimensionality == 2)
        {
            const auto [mapped_variables, mapped_variables_dimensions] = FillMappedVariables(attributes, variables);
            result.emplace_back(attributes, mapped_variables, mapped_variables_dimensions);
        }
    }

    return result;
};
