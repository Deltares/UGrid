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

void Mesh2D::Inquire(ugridapi::Mesh2D& mesh2d) const
{
    if (m_ncFile == nullptr || m_ncFile->isNull())
    {
        return;
    }

    mesh2d.num_nodes = m_attribute_to_variables.at("node_coordinates").at(0).getDim(0).getSize();
    mesh2d.num_nodes = m_attribute_to_variables.at("edge_coordinates").at(0).getDim(0).getSize();
    mesh2d.num_nodes = m_attribute_to_variables.at("face_coordinates").at(0).getDim(0).getSize();

    // Optional numerical values
    if (m_attribute_to_variables.find("face_node_connectivity") != m_attribute_to_variables.end())
    {
        mesh2d.num_face_nodes_max = m_attribute_to_variables.at("face_node_connectivity").at(0).getDim(1).getSize();
    }
}

void Mesh2D::Get(ugridapi::Mesh2D& mesh2d) const
{
    // Inquire in case dimensions are not there
    Inquire(mesh2d);

    //m_attributes




}

std::vector<Mesh2D> Mesh2D::Create(std::shared_ptr<netCDF::NcFile> const& ncFile)
{
    // Get all vars in this file
    const auto variables = ncFile->getVars();
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
            const auto attributes_to_variables = FillMappedVariables(attributes, variables);
            result.emplace_back(ncFile, attributes, attributes_to_variables);
        }
    }

    return result;
};
