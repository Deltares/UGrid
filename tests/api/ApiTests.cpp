#include <exception>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TestUtils/Definitions.hpp>
#include <UGridApi/UGrid.hpp>

TEST(ApiTest, Open_AFileWithOneMesh2d_ShouldCreateANewStateWithOneMesh2D)
{

    std::string filePath = TEST_FOLDER + "/data/OneMesh2D.nc";
    int ugrid_id = 0;

    // Open a file
    auto error_code = ugridapi::ug_open(filePath.c_str(), 0, ugrid_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    auto const num_mesh2d_topologies = ugridapi::ug_get_topology_num(ugrid_id, 2);
    ASSERT_EQ(num_mesh2d_topologies, 1);

    // Get the dimensions 
    ugridapi::Mesh2D mesh2d;
    error_code = ug_mesh2d_inq(ugrid_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    std::unique_ptr<double> const node_x(new double[mesh2d.num_nodes]);
    mesh2d.node_x = node_x.get();
    std::unique_ptr<double> const node_y(new double[mesh2d.num_nodes]);
    mesh2d.node_y = node_y.get();
    error_code = ug_mesh2d_get(ugrid_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<double> validMeshNodesX(node_x.get(), node_x.get() + mesh2d.num_nodes);

    // close the file
    error_code = ugridapi::ug_close(ugrid_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

}