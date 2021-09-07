#include <exception>
#include <memory>
#include <string.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TestUtils/Definitions.hpp>
#include <UGridApi/UGrid.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

TEST(ApiTest, InquireAndGet_AFileWithOneMesh2d_ShouldReadMesh2d)
{
    // Prepare
    // Open a file
    std::string const file_path = TEST_FOLDER + "/OneMesh2D.nc";
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_read_mode();
    auto error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Get the number of topologies
    auto topology_type = ugridapi::ug_topology_get_mesh2d_enum();
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
    mesh2d.edge_node = edge_nodes.get();
    std::unique_ptr<double> const face_x(new double[mesh2d.num_faces]);
    mesh2d.face_x = face_x.get();
    std::unique_ptr<double> const face_y(new double[mesh2d.num_faces]);
    mesh2d.face_y = face_y.get();
    std::unique_ptr<int> const face_nodes(new int[mesh2d.num_faces * mesh2d.num_face_nodes_max]);
    mesh2d.face_node = face_nodes.get();

    // Execute
    error_code = ug_mesh2d_get(file_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Assert
    std::string mesh_name(mesh2d.name);
    ASSERT_EQ(mesh_name, "mesh2d");
    std::vector<double> node_x_vector(node_x.get(), node_x.get() + mesh2d.num_nodes);
    std::vector<double> node_x_expected{ 0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
    ASSERT_THAT(node_x_vector, ::testing::ContainerEq(node_x_expected));

    std::vector<double> node_y_vector(node_y.get(), node_y.get() + mesh2d.num_nodes);
    std::vector<double> node_y_vector_expected{ 0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3 };
    ASSERT_THAT(node_y_vector, ::testing::ContainerEq(node_y_vector_expected));

    std::vector<int> edge_nodes_vector(edge_nodes.get(), edge_nodes.get() + mesh2d.num_edges * 2);
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

    std::vector<double> face_x_vector(face_x.get(), face_x.get() + mesh2d.num_faces);
    std::vector<double> face_x_vector_expected{ 0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 2.5, 2.5, 2.5 };
    ASSERT_THAT(face_x_vector, ::testing::ContainerEq(face_x_vector_expected));

    std::vector<double> face_y_vector(face_y.get(), face_y.get() + mesh2d.num_faces);
    std::vector<double> face_y_vector_expected{ 0.5, 1.5, 2.5, 0.5, 1.5, 2.5, 0.5, 1.5, 2.5 };
    ASSERT_THAT(face_y_vector, ::testing::ContainerEq(face_y_vector_expected));


    std::vector<int> face_nodes_vector(face_nodes.get(), face_nodes.get() + mesh2d.num_faces * mesh2d.num_face_nodes_max);
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
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, DefineAndPut_OneMesh2D_ShouldWriteData)
{
    // Prepare
    // Open a file
    std::string const file_path = TEST_WRITE_FOLDER + "/OneMesh2DWrite.nc";
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_replace_mode();
    auto error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Fill data 
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
    mesh2d.edge_node = edge_nodes.get();
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
    mesh2d.face_node = face_nodes.get();
    mesh2d.num_face_nodes_max = 4;

    // Execute
    int topology_id = -1;
    error_code = ug_mesh2d_def(file_id, mesh2d, topology_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);

    error_code = ug_mesh2d_put(file_id, topology_id, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}


TEST(ApiTest, InquireAndGet_AFileWithOneNetwork1D_ShouldReadNetwork1D)
{
    // Prepare 
    // Open a file
    int file_id = 0;
    std::string const file_path = TEST_FOLDER + "/AllUGridEntities.nc";
    auto const file_mode = ugridapi::ug_file_read_mode();
    auto error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    auto topology_type = ugridapi::ug_topology_get_network1d_enum();
    auto const num_topologies = ugridapi::ug_topology_get_count(file_id, topology_type);
    ASSERT_EQ(num_topologies, 1);

    // get the dimensions 
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
    network1d.branch_node = edge_nodes.get();

    std::unique_ptr<double> const geometry_nodes_x(new double[network1d.num_geometry_nodes]);
    network1d.geometry_nodes_x = geometry_nodes_x.get();

    std::unique_ptr<double> const geometry_nodes_y(new double[network1d.num_geometry_nodes]);
    network1d.geometry_nodes_y = geometry_nodes_y.get();

    std::unique_ptr<char> const node_id(new char[name_length * network1d.num_nodes]);
    network1d.node_name_id = node_id.get();

    std::unique_ptr<char> const node_long_name(new char[long_names_length * network1d.num_nodes]);
    network1d.node_name_long = node_long_name.get();

    std::unique_ptr<char> const branch_id(new char[name_length * network1d.num_nodes]);
    network1d.branch_name_id = branch_id.get();

    std::unique_ptr<char> const branch_long_name(new char[long_names_length * network1d.num_nodes]);
    network1d.branch_name_long = branch_long_name.get();

    std::unique_ptr<double> const branch_lengths(new double[network1d.num_edges]);
    network1d.branch_length = branch_lengths.get();

    // get the data
    error_code = ug_network1d_get(file_id, 0, network1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    //// Assert
    std::string node_ids_string(node_id.get(), node_id.get() + name_length * network1d.num_nodes);
    std::string node_long_names_string(node_long_name.get(), node_long_name.get() + long_names_length * network1d.num_nodes);
    for (auto i = 0; i < network1d.num_nodes; ++i)
    {
        std::string node_id_string = node_ids_string.substr(i * name_length, name_length);
        std::string node_long_name_string = node_long_names_string.substr(i * long_names_length, long_names_length);
        ASSERT_EQ("nodesids                                ", node_id_string);
        ASSERT_EQ("nodeslongNames                                                                  ", node_long_name_string);

    }

    std::string branch_ids_string(branch_id.get(), branch_id.get() + name_length * network1d.num_edges);
    std::string branch_long_names_string(branch_long_name.get(), branch_long_name.get() + long_names_length * network1d.num_edges);
    for (auto i = 0; i < network1d.num_edges; ++i)
    {
        std::string branch_id_string = branch_ids_string.substr(i * name_length, name_length);
        std::string branch_long_name_string = branch_long_names_string.substr(i * long_names_length, long_names_length);
        ASSERT_EQ("branchids                               ", branch_id_string);
        ASSERT_EQ("branchlongNames                                                                 ", branch_long_name_string);
    }

    std::vector<double> node_x_vector(node_x.get(), node_x.get() + network1d.num_nodes);
    std::vector<double> node_x_expected{ 293.78, 538.89 };
    ASSERT_THAT(node_x_vector, ::testing::ContainerEq(node_x_expected));

    std::vector<double> node_y_vector(node_y.get(), node_y.get() + network1d.num_nodes);
    std::vector<double> node_y_vector_expected{ 27.48, 956.75 };
    ASSERT_THAT(node_y_vector, ::testing::ContainerEq(node_y_vector_expected));

    std::vector<int> edge_nodes_vector(edge_nodes.get(), edge_nodes.get() + network1d.num_edges * 2);
    std::vector<int> edge_nodes_vector_expected{ 0,1 };
    ASSERT_THAT(edge_nodes_vector, ::testing::ContainerEq(edge_nodes_vector_expected));

    std::vector<double> geometry_nodes_x_vector(geometry_nodes_x.get(), geometry_nodes_x.get() + network1d.num_geometry_nodes);
    std::vector<double> geometry_nodes_x_expected_vector{ 293.78, 278.97, 265.31, 254.17, 247.44, 248.3, 259.58,
    282.24, 314.61, 354.44, 398.94, 445, 490.6, 532.84, 566.64, 589.08,
    600.72, 603.53, 599.27, 590.05, 577.56, 562.97, 547.12, 530.67, 538.89 };

    ASSERT_THAT(geometry_nodes_x_vector, ::testing::ContainerEq(geometry_nodes_x_expected_vector));
    std::vector<double> geometry_nodes_y_vector(geometry_nodes_y.get(), geometry_nodes_y.get() + network1d.num_geometry_nodes);
    std::vector<double> geometry_nodes_y_expected_vector{ 27.48, 74.87, 122.59, 170.96, 220.12, 269.67, 317.89,
        361.93, 399.39, 428.84, 450.76, 469.28, 488.89, 514.78, 550.83, 594.93,
        643.09, 692.6, 742.02, 790.79, 838.83, 886.28, 933.33, 980.17, 956.75 };
    ASSERT_THAT(geometry_nodes_y_vector, ::testing::ContainerEq(geometry_nodes_y_expected_vector));

    std::vector<double> branch_lengths_vector(branch_lengths.get(), branch_lengths.get() + network1d.num_edges);
    std::vector<double> branch_lengths_expected_vector{ 1165.29 };
    ASSERT_THAT(branch_lengths_vector, ::testing::ContainerEq(branch_lengths_expected_vector));

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

}

TEST(ApiTest, DefineAndPut_OneNetwork1D_ShouldWriteData)
{
    std::string const file_path = TEST_WRITE_FOLDER + "/OneNetwork1DWrite.nc";

    // Open a file
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_replace_mode();
    auto error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Fill data 
    ugridapi::Network1d network1d;
    std::unique_ptr<char> const name(new char[] {"network1d"});
    network1d.name = name.get();
    std::unique_ptr<double> const node_x(new double[] { 293.78, 538.89 });
    network1d.node_x = node_x.get();
    std::unique_ptr<double> const node_y(new double[] { 27.48, 956.75 });
    network1d.node_y = node_y.get();
    network1d.num_nodes = 2;
    std::unique_ptr<int> const edge_nodes(new int[] { 0, 1});
    network1d.branch_node = edge_nodes.get();
    network1d.num_edges = 1;
    std::unique_ptr<double> const geometry_nodes_x(new double[] { 293.78, 278.97, 265.31, 254.17, 247.44, 248.3, 259.58,
        282.24, 314.61, 354.44, 398.94, 445, 490.6, 532.84, 566.64, 589.08,
        600.72, 603.53, 599.27, 590.05, 577.56, 562.97, 547.12, 530.67, 538.89 });
    network1d.geometry_nodes_x = geometry_nodes_x.get();
    std::unique_ptr<double> const geometry_nodes_y(new double[] { 27.48, 74.87, 122.59, 170.96, 220.12, 269.67, 317.89,
        361.93, 399.39, 428.84, 450.76, 469.28, 488.89, 514.78, 550.83, 594.93,
        643.09, 692.6, 742.02, 790.79, 838.83, 886.28, 933.33, 980.17, 956.75 });
    network1d.geometry_nodes_y = geometry_nodes_y.get();
    network1d.num_geometry_nodes = 25;
    std::unique_ptr<char> const node_id(new char[] {"nodesids                                nodesids                                "});
    network1d.node_name_id = node_id.get();
    std::unique_ptr<char> const node_long_name(new char[] {"nodeslongNames                                                                  nodeslongNames                                                                  "});
    network1d.node_name_long = node_long_name.get();
    std::unique_ptr<char> const branch_id(new char[] {"branchids                               "});
    network1d.branch_name_id = branch_id.get();
    std::unique_ptr<char> const branch_long_name(new char[] {"branchlongNames                                                                 "});
    network1d.branch_name_long = branch_long_name.get();
    std::unique_ptr<double> const branch_lengths(new double[] {1165.29});
    network1d.branch_length = branch_lengths.get();

    int topology_id = -1;
    error_code = ug_network1d_def(file_id, network1d, topology_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);

    error_code = ug_network1d_put(file_id, topology_id, network1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}
TEST(ApiTest, InquireAndGet_AFileWithOneMesh1D_ShouldReadMesh1D)
{
    std::string const file_path = TEST_FOLDER + "/AllUGridEntities.nc";

    // Open a file
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_read_mode();
    auto error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    auto topology_type = ugridapi::ug_topology_get_mesh1d_enum();
    auto const num_topologies = ugridapi::ug_topology_get_count(file_id, topology_type);
    ASSERT_EQ(num_topologies, 1);

    // get the dimensions 
    ugridapi::Mesh1D mesh1d;
    error_code = ug_mesh1d_inq(file_id, 0, mesh1d);
    ASSERT_EQ(25, mesh1d.num_nodes);
    ASSERT_EQ(24, mesh1d.num_edges);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    auto const name_length = ugridapi::ug_name_get_length();
    auto const long_names_length = ugridapi::ug_name_get_long_length();

    std::unique_ptr<char> const name(new char[name_length]);
    mesh1d.name = name.get();

    std::unique_ptr<char> const network_name(new char[name_length]);
    mesh1d.network_name = network_name.get();

    std::unique_ptr<int> const branch_id(new int[mesh1d.num_nodes]);
    mesh1d.node_edge_id = branch_id.get();

    std::unique_ptr<double> const branch_offset(new double[mesh1d.num_nodes]);
    mesh1d.node_edge_offset = branch_offset.get();

    std::unique_ptr<int> const edge_nodes(new int[mesh1d.num_edges * 2]);
    mesh1d.edge_node = edge_nodes.get();

    std::unique_ptr<char> const node_id(new char[name_length * mesh1d.num_nodes]);
    mesh1d.node_name_id = node_id.get();

    std::unique_ptr<char> const node_long_name(new char[long_names_length * mesh1d.num_nodes]);
    mesh1d.node_name_long = node_long_name.get();

    // get the data
    error_code = ug_mesh1d_get(file_id, 0, mesh1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    //// Assert
    std::vector<int> branch_id_vector(branch_id.get(), branch_id.get() + mesh1d.num_nodes);
    std::vector<double> branch_offset_vector(branch_offset.get(), branch_offset.get() + mesh1d.num_nodes);
    std::vector<int> edge_nodes_vector(edge_nodes.get(), edge_nodes.get() + mesh1d.num_edges * 2);

    std::string node_ids_string(node_id.get(), node_id.get() + name_length * mesh1d.num_nodes);
    std::string node_long_names_string(node_long_name.get(), node_long_name.get() + long_names_length * mesh1d.num_nodes);
    for (auto i = 0; i < mesh1d.num_nodes; ++i)
    {
        std::string node_id_string = node_ids_string.substr(i * name_length, name_length);
        std::string node_long_name_string = node_long_names_string.substr(i * long_names_length, long_names_length);
        ASSERT_EQ("meshnodeids                             ", node_id_string);
        ASSERT_EQ("meshnodelongnames                                                               ", node_long_name_string);
    }

    std::string network_name_string(mesh1d.network_name);
    ASSERT_EQ("network", network_name_string);

    std::string mesh1d_name_string(mesh1d.name);
    ASSERT_EQ("1dmesh", mesh1d_name_string);
}

TEST(ApiTest, DefineAndPut_OneMesh1D_ShouldWriteData)
{
    std::string const file_path = TEST_WRITE_FOLDER + "/OneMesh1DWrite.nc";

    // Open a file
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_replace_mode();
    auto error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Fill data 
    ugridapi::Mesh1D mesh1d;
    std::unique_ptr<char> const name(new char[] {"mesh1d"});
    mesh1d.name = name.get();

    std::unique_ptr<char> const network_name(new char[] {"network1d"});
    mesh1d.network_name = network_name.get();

    std::unique_ptr<int> const node_branch_id(new int[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
    mesh1d.node_edge_id = node_branch_id.get();

    std::unique_ptr<double> const node_branch_offset(new double[] {
        0, 49.65, 99.29, 148.92, 198.54, 248.09,
            297.62, 347.15, 396.66, 446.19, 495.8, 545.44, 595.08, 644.63, 694.04,
            743.52, 793.07, 842.65, 892.26, 941.89, 991.53, 1041.17, 1090.82,
            1140.46, 1165.29 });
    mesh1d.node_edge_offset = node_branch_offset.get();

    mesh1d.num_nodes = 25;
    mesh1d.num_edges = 24;
    std::unique_ptr<int> const edges_nodes(new int[] { 0, 1,
        1, 2,
        2, 3,
        3, 4,
        4, 5,
        5, 6,
        6, 7,
        7, 8,
        8, 9,
        9, 10,
        10, 11,
        11, 12,
        12, 13,
        13, 14,
        14, 15,
        15, 16,
        16, 17,
        17, 18,
        18, 19,
        19, 20,
        20, 21,
        21, 22,
        22, 23,
        23, 24});
    mesh1d.edge_node = edges_nodes.get();

    std::stringstream meshnodesids;
    std::stringstream meshnodelongnames;
    for (auto i = 0; i < mesh1d.num_nodes; ++i)
    {
        meshnodesids << "meshnodeids                             ";
        meshnodelongnames << "meshnodelongnames                                                               ";
    }

    std::unique_ptr<char> const node_name_id(strdup(meshnodesids.str().c_str()));
    mesh1d.node_name_id = node_name_id.get();

    std::unique_ptr<char> const node_name_long(strdup(meshnodelongnames.str().c_str()));
    mesh1d.node_name_long = node_name_long.get();

    int topology_id = -1;
    error_code = ug_mesh1d_def(file_id, mesh1d, topology_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);

    error_code = ug_mesh1d_put(file_id, topology_id, mesh1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, InquireAndGet_AFileWithOneContact_ShouldReadContact)
{
    std::string const file_path = TEST_FOLDER + "/AllUGridEntities.nc";

    // Open a file
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_read_mode();
    auto error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    auto topology_type = ugridapi::ug_topology_get_contacts_enum();
    auto const num_topologies = ugridapi::ug_topology_get_count(file_id, topology_type);
    ASSERT_EQ(num_topologies, 1);

    // get the dimensions 
    ugridapi::Contacts contacts;
    error_code = ug_contacts_inq(file_id, 0, contacts);
    ASSERT_EQ(23, contacts.num_contacts);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    auto const name_length = ugridapi::ug_name_get_length();
    auto const long_names_length = ugridapi::ug_name_get_long_length();

    std::unique_ptr<char> const name(new char[name_length]);
    contacts.name = name.get();

    std::unique_ptr<char> const mesh_from_name(new char[name_length]);
    contacts.mesh_from_name = mesh_from_name.get();

    std::unique_ptr<char> const mesh_to_name(new char[name_length]);
    contacts.mesh_to_name = mesh_to_name.get();

    std::unique_ptr<char> const contact_name_id(new char[name_length * contacts.num_contacts]);
    contacts.contact_name_id = contact_name_id.get();

    std::unique_ptr<char> const contact_name_long(new char[long_names_length * contacts.num_contacts]);
    contacts.contact_name_long = contact_name_long.get();

    std::unique_ptr<int> const contact_type(new int[contacts.num_contacts]);
    contacts.contact_type = contact_type.get();

    std::unique_ptr<int> const edges(new int[contacts.num_contacts * 2]);
    contacts.edges = edges.get();

    // get the data
    error_code = ug_contacts_get(file_id, 0, contacts);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Asserts
    std::string mesh_from_name_string(contacts.mesh_from_name);
    ASSERT_EQ("mesh2d", mesh_from_name_string);

    std::string mesh_to_name_string(contacts.mesh_to_name);
    ASSERT_EQ("1dmesh", mesh_to_name_string);

    std::string contacts_ids_string(contact_name_id.get(), contact_name_id.get() + name_length * contacts.num_contacts);
    std::string contacts_long_names_string(contact_name_long.get(), contact_name_long.get() + long_names_length * contacts.num_contacts);
    for (auto i = 0; i < contacts.num_contacts; ++i)
    {
        std::string contact_id_string = contacts_ids_string.substr(i * name_length, name_length);
        std::string contact_long_name_string = contacts_long_names_string.substr(i * long_names_length, long_names_length);
        ASSERT_EQ("linkid                                  ", contact_id_string);
        ASSERT_EQ("linklongname                                                                    ", contact_long_name_string);
    }

    std::vector<int> edge_vector(edges.get(), edges.get() + contacts.num_contacts * 2);
    std::vector<int> edge_vector_expected{ 13, 1,
        13, 2,
        13, 3,
        13, 4,
        70, 5,
        76, 6,
        91, 7,
        13, 8,
        13, 9,
        13, 10,
        13, 11,
        13, 12,
        178, 13,
        200, 14,
        228, 15,
        255, 16,
        277, 17,
        293, 18,
        304, 19,
        315, 20,
        326, 21,
        337, 22,
        353, 23 };
    ASSERT_THAT(edge_vector, ::testing::ContainerEq(edge_vector_expected));

    std::vector<int> contact_type_vector(contact_type.get(), contact_type.get() + contacts.num_contacts);
    std::vector<int> contact_type_vector_expected{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
    ASSERT_THAT(contact_type_vector, ::testing::ContainerEq(contact_type_vector_expected));

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, DefineAndPut_AFileWithOneContact_ShouldWriteAContact)
{
    std::string const file_path = TEST_WRITE_FOLDER + "/OneMesh1DWrite.nc";

    // Open a file
    int file_id = 0;
    auto const file_mode = ugridapi::ug_file_replace_mode();
    auto error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Fill data 
    ugridapi::Contacts contacts;
    std::unique_ptr<char> const name(new char[] {"2d1dlinks"});
    contacts.name = name.get();

    std::unique_ptr<char> const mesh_from_name(new char[] {"mesh2d"});
    contacts.mesh_from_name = mesh_from_name.get();

    std::unique_ptr<char> const mesh_to_name(new char[] {"1dmesh"});
    contacts.mesh_to_name = mesh_to_name.get();

    contacts.mesh_from_location = ugridapi::ug_entity_get_face_location_enum();

    contacts.mesh_to_location = ugridapi::ug_entity_get_face_location_enum();

    std::unique_ptr<int> const contact_type(new int[] {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 });
    contacts.contact_type = contact_type.get();

    std::unique_ptr<int> const edges(new int[] { 13, 1,
        13, 2,
        13, 3,
        13, 4,
        70, 5,
        76, 6,
        91, 7,
        13, 8,
        13, 9,
        13, 10,
        13, 11,
        13, 12,
        178, 13,
        200, 14,
        228, 15,
        255, 16,
        277, 17,
        293, 18,
        304, 19,
        315, 20,
        326, 21,
        337, 22,
        353, 23 });
    contacts.edges = edges.get();

    contacts.num_contacts = 23;

    std::stringstream ids;
    std::stringstream long_names;
    for (auto i = 0; i < contacts.num_contacts; ++i)
    {
        ids << "linkid                                  ";
        long_names << "linklongname                                                                    ";
    }

    std::unique_ptr<char> const contact_name_id(strdup(ids.str().c_str()));
    contacts.contact_name_id = contact_name_id.get();

    std::unique_ptr<char> const contact_name_long(strdup(long_names.str().c_str()));
    contacts.contact_name_long = contact_name_long.get();

    int topology_id = -1;
    error_code = ug_contacts_def(file_id, contacts, topology_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);

    error_code = ug_contacts_put(file_id, topology_id, contacts);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}