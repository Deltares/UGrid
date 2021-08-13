#include <exception>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TestUtils/Definitions.hpp>
#include <UGridApi/UGrid.hpp>

TEST(ApiTest, Open_AFileWithOneMesh2d_ShouldCreateANewStateWithOneMesh2D)
{
    // Prepare
    std::string filePath = TEST_FOLDER + "/data/OneMesh2D.nc";
    int ugrid_id = 0;
    // Execute
    auto error_code = ugridapi::ug_open(filePath.c_str(), 0, ugrid_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    auto const num_mesh2d_topologies = ugridapi::ug_get_topology_num(ugrid_id, 2);
    ASSERT_EQ(num_mesh2d_topologies, 1);

    ugridapi::Mesh2D mesh2d;
    error_code = ug_mesh2d_inq(ugrid_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);


}