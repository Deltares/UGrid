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
    auto error_code = ugridapi::ug_open(filePath.c_str(), 0);

    // Assert
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

}