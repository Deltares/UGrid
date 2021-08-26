#include <exception>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TestUtils/Definitions.hpp>
#include <UGridApi/UGrid.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

TEST(ApiTest, InquireAndGet_AFileWithOneMesh2d_ShouldReadMesh2d)
{
    std::string const filePath = TEST_FOLDER + "/data/OneMesh2D.nc";

    // Open a file
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_read_mode();
    auto error_code = ugridapi::ug_open(filePath.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    auto topology_type = ugridapi::ug_topology_get_mesh2d_type_enum();
    auto const num_mesh2d_topologies = ugridapi::ug_topology_get_count(file_id, topology_type);
    ASSERT_EQ(num_mesh2d_topologies, 1);

    // Get the dimensions 
    ugridapi::Mesh2D mesh2d;
    error_code = ug_mesh2d_inq(file_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    auto const name_length = ugridapi::ug_name_get_length();
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
    auto const file_mode = ugridapi::ug_file_replace_mode();
    auto error_code = ugridapi::ug_open(filePath.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Fill all data 
    ugridapi::Mesh2D mesh2d;
    std::unique_ptr<char> const name(new char[] {"mesh2d"});
    mesh2d.name = name.get();
    std::unique_ptr<double> const node_x(new double[] { 0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3 });
    mesh2d.node_x = node_x.get();
    std::unique_ptr<double> const node_y(new double[] { 0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3 });
    mesh2d.node_y = node_y.get();
    mesh2d.num_nodes = 16;
    std::unique_ptr<int> const edge_nodes(new int[] { 1, 2,
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
        15, 16,
        });
    mesh2d.edge_nodes = edge_nodes.get();
    mesh2d.num_edges = 23;
    std::unique_ptr<double> const face_x(new double[] { 0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 2.5, 2.5, 2.5 });
    mesh2d.face_x = face_x.get();
    std::unique_ptr<double> const face_y(new double[] { 0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3 });
    mesh2d.face_y = face_y.get();
    mesh2d.num_faces = 9;
    std::unique_ptr<int> const face_nodes(new int[] {
        1, 2, 4, 3,
            3, 4, 6, 5,
            5, 6, 8, 7,
            2, 9, 10, 4,
            4, 10, 11, 6,
            6, 11, 12, 8,
            9, 13, 14, 10,
            10, 14, 15, 11,
            11, 15, 16, 12
        });
    mesh2d.face_nodes = face_nodes.get();
    mesh2d.num_face_nodes_max = 4;


    int topology_id = -1;
    error_code = ug_mesh2d_def(file_id, mesh2d, topology_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);

    error_code = ug_mesh2d_put(file_id, topology_id, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);

    // Close the file
    error_code = ugridapi::ug_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}


TEST(ApiTest, InquireAndGet_AFileWithOneNetwork1D_ShouldReadNetwork1D)
{
    std::string const filePath = TEST_FOLDER + "/data/AllEntities.nc";

    // Open a file
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_read_mode();
    auto error_code = ugridapi::ug_open(filePath.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    auto topology_type = ugridapi::ug_topology_get_network1d_type_enum();
    auto const num_topologies = ugridapi::ug_topology_get_count(file_id, topology_type);
    ASSERT_EQ(num_topologies, 1);

    // Get the dimensions 
    ugridapi::Network1d network1d;
    error_code = ug_network1d_inq(file_id, 0, network1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    auto const name_length = ugridapi::ug_name_get_length();
    auto const long_names_length = ugridapi::ug_name_get_long_length();

    std::unique_ptr<double> const node_x(new double[network1d.num_nodes]);
    network1d.node_x = node_x.get();

    std::unique_ptr<double> const node_y(new double[network1d.num_nodes]);
    network1d.node_y = node_y.get();

    std::unique_ptr<int> const edge_nodes(new int[network1d.num_edges * 2]);
    network1d.edge_nodes = edge_nodes.get();

    std::unique_ptr<double> const geometry_nodes_x(new double[network1d.num_geometry_nodes]);
    network1d.geometry_nodes_x = geometry_nodes_x.get();

    std::unique_ptr<double> const geometry_nodes_y(new double[network1d.num_geometry_nodes]);
    network1d.geometry_nodes_y = geometry_nodes_y.get();

    std::unique_ptr<char> const node_ids(new char[name_length * network1d.num_nodes]);
    network1d.node_ids = node_ids.get();

    std::unique_ptr<char> const node_long_names(new char[long_names_length * network1d.num_nodes]);
    network1d.node_long_names = node_long_names.get();

    std::unique_ptr<char> const branch_ids(new char[name_length * network1d.num_nodes]);
    network1d.branch_ids = branch_ids.get();

    std::unique_ptr<char> const branch_long_names(new char[long_names_length * network1d.num_nodes]);
    network1d.branch_long_names = branch_long_names.get();

    // Get the data
    error_code = ug_network1d_get(file_id, 0, network1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    //// Assert
    std::vector<double> node_x_vector(node_x.get(), node_x.get() + network1d.num_nodes);
    std::vector<double> node_y_vector(node_y.get(), node_y.get() + network1d.num_nodes);
    std::vector<int> edge_nodes_vector(edge_nodes.get(), edge_nodes.get() + network1d.num_edges * 2);
    std::vector<double> geometry_nodes_x_vector(geometry_nodes_x.get(), geometry_nodes_x.get() + network1d.num_geometry_nodes);
    std::vector<double> geometry_nodes_y_vector(geometry_nodes_y.get(), geometry_nodes_y.get() + network1d.num_geometry_nodes);
    std::string node_ids_string(node_ids.get(), node_ids.get() + name_length * network1d.num_nodes);
    std::string node_long_names_string(node_long_names.get(), node_long_names.get() + long_names_length * network1d.num_nodes);

    std::string branch_ids_string(branch_ids.get(), branch_ids.get() + name_length * network1d.num_edges);
    std::string branch_long_names_string(branch_long_names.get(), branch_long_names.get() + long_names_length * network1d.num_edges);
    for (auto i = 0; i < network1d.num_nodes; ++i)
    {
        std::string node_id = node_ids_string.substr(i * name_length, (i + 1) * name_length);
        std::string node_long_name = node_long_names_string.substr(i * long_names_length, (i + 1) * long_names_length);
        ASSERT_EQ("nodesids                                ", node_id);
        ASSERT_EQ("nodeslongNames                                                                  ", node_long_name);

    }
    for (auto i = 0; i < network1d.num_edges; ++i)
    {
        std::string branch_id = branch_ids_string.substr(i * name_length, (i + 1) * name_length);
        std::string branch_long_name = branch_long_names_string.substr(i * long_names_length, (i + 1) * long_names_length);
        ASSERT_EQ("branchids                               ", branch_id);
        ASSERT_EQ("branchlongNames                                                                 ", branch_long_name);
    }

    std::vector<double> node_x_expected{ 293.78, 538.89 };
    ASSERT_THAT(node_x_vector, ::testing::ContainerEq(node_x_expected));
    std::vector<double> node_y_vector_expected{ 27.48, 956.75 };
    ASSERT_THAT(node_y_vector, ::testing::ContainerEq(node_y_vector_expected));
    std::vector<int> edge_nodes_vector_expected{ 0,1 };
    ASSERT_THAT(edge_nodes_vector, ::testing::ContainerEq(edge_nodes_vector_expected));

    std::vector<double> geometry_nodes_x_expected_vector{ 293.78, 278.97, 265.31, 254.17, 247.44, 248.3, 259.58,
    282.24, 314.61, 354.44, 398.94, 445, 490.6, 532.84, 566.64, 589.08,
    600.72, 603.53, 599.27, 590.05, 577.56, 562.97, 547.12, 530.67, 538.89 };
    ASSERT_THAT(geometry_nodes_x_vector, ::testing::ContainerEq(geometry_nodes_x_expected_vector));
    std::vector<double> geometry_nodes_y_expected_vector{ 27.48, 74.87, 122.59, 170.96, 220.12, 269.67, 317.89,
    361.93, 399.39, 428.84, 450.76, 469.28, 488.89, 514.78, 550.83, 594.93,
    643.09, 692.6, 742.02, 790.79, 838.83, 886.28, 933.33, 980.17, 956.75 };
    ASSERT_THAT(geometry_nodes_y_vector, ::testing::ContainerEq(geometry_nodes_y_expected_vector));
}