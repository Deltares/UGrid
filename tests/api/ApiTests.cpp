#include <exception>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TestUtils/Definitions.hpp>
#include <UGridApi/Options.hpp>
#include <UGridApi/UGrid.hpp>

TEST(ApiTest, InquireAndGet_AFileWithOneMesh2d_ShouldReadMesh2d)
{
    std::string const filePath = TEST_FOLDER + "/data/OneMesh2D.nc";

    // Open a file
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_read_mode();
    auto error_code = ugridapi::ug_open(filePath.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    auto topology_type = ugridapi::ug_topology_get_mesh2d_type();
    auto const num_mesh2d_topologies = ugridapi::ug_topology_get_num(file_id, topology_type);
    ASSERT_EQ(num_mesh2d_topologies, 1);

    // Get the dimensions 
    ugridapi::Mesh2D mesh2d;
    error_code = ug_mesh2d_inq(file_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    int name_length = ugridapi::ug_name_get_length();
    std::unique_ptr<char> const name(new char[name_length]);
    mesh2d.name = name.get();
    std::unique_ptr<double> const node_x(new double[mesh2d.num_nodes]);
    mesh2d.node_x = node_x.get();
    std::unique_ptr<double> const node_y(new double[mesh2d.num_nodes]);
    mesh2d.node_y = node_y.get();
    std::unique_ptr<int> const edge_nodes(new int[mesh2d.num_edges * 2]);
    mesh2d.edge_nodes = edge_nodes.get();
    std::unique_ptr<double> const face_x(new double[mesh2d.num_faces]);
    mesh2d.face_x = face_x.get();
    std::unique_ptr<double> const face_y(new double[mesh2d.num_faces]);
    mesh2d.face_y = face_y.get();
    std::unique_ptr<int> const face_nodes(new int[mesh2d.num_faces * mesh2d.num_face_nodes_max]);
    mesh2d.face_nodes = face_nodes.get();

    // Get the data
    error_code = ug_mesh2d_get(file_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<double> node_x_vector(node_x.get(), node_x.get() + mesh2d.num_nodes);
    std::vector<double> node_y_vector(node_y.get(), node_y.get() + mesh2d.num_nodes);
    std::vector<int> edge_nodes_vector(edge_nodes.get(), edge_nodes.get() + mesh2d.num_edges * 2);
    std::vector<double> face_x_vector(face_x.get(), face_x.get() + mesh2d.num_faces);
    std::vector<double> face_y_vector(face_y.get(), face_y.get() + mesh2d.num_faces);
    std::vector<int> face_nodes_vector(face_nodes.get(), face_nodes.get() + mesh2d.num_faces * mesh2d.num_face_nodes_max);

    // Assert
    std::string mesh_name(mesh2d.name);
    ASSERT_EQ(mesh_name, "mesh2d");
    std::vector<double> node_x_expected{ 0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
    ASSERT_THAT(node_x_vector, ::testing::ContainerEq(node_x_expected));
    std::vector<double> node_y_vector_expected{ 0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3 };
    ASSERT_THAT(node_y_vector, ::testing::ContainerEq(node_y_vector_expected));
    std::vector<int> edge_nodes_vector_expected
    {
        1, 2,
        3, 4,
        5, 6,
        7, 8,
        2, 9,
        4, 10,
        6, 11,
        8, 12,
        9, 13,
        10, 14,
        11, 15,
        12, 16,
        1, 3,
        3, 5,
        5, 7,
        2, 4,
        4, 6,
        6, 8,
        9, 10,
        10, 11,
        11, 12,
        13, 14,
        14, 15,
        15, 16
    };
    ASSERT_THAT(edge_nodes_vector, ::testing::ContainerEq(edge_nodes_vector_expected));

    std::vector<double> face_x_vector_expected{ 0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 2.5, 2.5, 2.5 };
    ASSERT_THAT(face_x_vector, ::testing::ContainerEq(face_x_vector_expected));
    std::vector<double> face_y_vector_expected{ 0.5, 1.5, 2.5, 0.5, 1.5, 2.5, 0.5, 1.5, 2.5 };
    ASSERT_THAT(face_y_vector, ::testing::ContainerEq(face_y_vector_expected));

    std::vector<int> face_nodes_vector_expected
    {
        1, 2, 4, 3,
        3, 4, 6, 5,
        5, 6, 8, 7,
        2, 9, 10, 4,
        4, 10, 11, 6,
        6, 11, 12, 8,
        9, 13, 14, 10,
        10, 14, 15, 11,
        11, 15, 16, 12
    };
    ASSERT_THAT(face_nodes_vector, ::testing::ContainerEq(face_nodes_vector_expected));

    // Close the file
    error_code = ugridapi::ug_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, DefineAndPut_OneMesh2D_ShouldWriteData)
{
    std::string const filePath = TEST_FOLDER + "/data/OneMesh2DWrite.nc";

    // Open a file
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_write_mode();
    auto error_code = ugridapi::ug_open(filePath.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);


    int topology_id = -1;
    ugridapi::Mesh2D mesh2d;
    ugridapi::Mesh2DOptions mesh2d_options;
    error_code = ug_mesh2d_def(file_id, mesh2d, mesh2d_options, topology_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);

}