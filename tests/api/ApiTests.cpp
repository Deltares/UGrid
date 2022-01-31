#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TestUtils/Definitions.hpp>
#include <TestUtils/Utils.hpp>
#include <UGridApi/UGrid.hpp>

std::vector<std::string> tokenize(const std::string&, std::size_t);

    static void define_variable_attributes(int file_id, std::string const& variable_name, std::string const& attribute_name, std::vector<int> const& attribute_values)
{
    // Write the char variable
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::unique_ptr<char> const variable_name_ptr(new char[name_long_length]);
    string_to_char_array(variable_name, name_long_length, variable_name_ptr.get());

    std::unique_ptr<char> const attribute_name_ptr(new char[name_long_length]);
    string_to_char_array(attribute_name, name_long_length, attribute_name_ptr.get());

    error_code = ugridapi::ug_attribute_int_define(file_id,
                                                   variable_name_ptr.get(),
                                                   attribute_name_ptr.get(),
                                                   &attribute_values[0],
                                                   attribute_values.size());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

static void define_variable_attributes(int file_id, std::string const& variable_name, std::string const& attribute_name, std::vector<double> const& attribute_values)
{
    // Write the char variable
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::unique_ptr<char> const variable_name_ptr(new char[name_long_length]);
    string_to_char_array(variable_name, name_long_length, variable_name_ptr.get());

    std::unique_ptr<char> const attribute_name_ptr(new char[name_long_length]);
    string_to_char_array(attribute_name, name_long_length, attribute_name_ptr.get());

    error_code = ugridapi::ug_attribute_double_define(file_id,
                                                      variable_name_ptr.get(),
                                                      attribute_name_ptr.get(),
                                                      &attribute_values[0],
                                                      attribute_values.size());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

static void define_variable_attributes(int file_id, std::string const& variable_name, std::string const& attribute_name, std::string const& attribute_value)
{
    // Write the char variable
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::unique_ptr<char> const variable_name_ptr(new char[name_long_length]);
    string_to_char_array(variable_name, name_long_length, variable_name_ptr.get());

    std::unique_ptr<char> const attribute_name_ptr(new char[name_long_length]);
    string_to_char_array(attribute_name, name_long_length, attribute_name_ptr.get());

    error_code = ugridapi::ug_attribute_char_define(file_id,
                                                    variable_name_ptr.get(),
                                                    attribute_name_ptr.get(),
                                                    attribute_value.c_str(),
                                                    attribute_value.length());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

static void define_global_attributes(int file_id, std::string const& attribute_name, std::string const& attribute_value)
{
    // Write the char variable
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::unique_ptr<char> const attribute_name_ptr(new char[name_long_length]);
    string_to_char_array(attribute_name, name_long_length, attribute_name_ptr.get());

    error_code = ugridapi::ug_attribute_global_char_define(file_id, attribute_name_ptr.get(), attribute_value.c_str(), attribute_value.length());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, InquireAndGet_OneMesh2D_ShouldReadMesh2d)
{
    // Prepare
    // Open a file
    std::string const file_path = TEST_FOLDER + "/OneMesh2D.nc";
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_read_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Get the number of topologies
    int topology_type;
    error_code = ugridapi::ug_topology_get_mesh2d_enum(topology_type);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int num_mesh2d_topologies;
    error_code = ugridapi::ug_topology_get_count(file_id, topology_type, num_mesh2d_topologies);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(num_mesh2d_topologies, 1);

    // Get the dimensions
    ugridapi::Mesh2D mesh2d;
    error_code = ug_mesh2d_inq(file_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    int name_long_length;
    error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    mesh2d.name.resize(name_long_length);
    mesh2d.node_x.resize(mesh2d.num_nodes);
    mesh2d.node_y.resize(mesh2d.num_nodes);
    mesh2d.edge_node.resize(mesh2d.num_edges * 2);
    mesh2d.face_x.resize(mesh2d.num_faces);
    mesh2d.face_y.resize(mesh2d.num_faces);
    mesh2d.face_node.resize(mesh2d.num_faces * mesh2d.num_face_nodes_max);
    // Execute
    error_code = ug_mesh2d_get(file_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Assert
    std::string mesh_name(mesh2d.name);
    right_trim_string(mesh_name);
    ASSERT_EQ(mesh_name, "mesh2d");
    std::vector<double> node_x_expected{0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3};
    ASSERT_THAT(mesh2d.node_x, ::testing::ContainerEq(node_x_expected));

    std::vector<double> node_y_vector_expected{0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3};
    ASSERT_THAT(mesh2d.node_y, ::testing::ContainerEq(node_y_vector_expected));

    std::vector<int> edge_nodes_vector_expected{
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
        15, 16};
    ASSERT_THAT(mesh2d.edge_node, ::testing::ContainerEq(edge_nodes_vector_expected));

    std::vector<double> face_x_vector_expected{0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 2.5, 2.5, 2.5};
    ASSERT_THAT(mesh2d.face_x, ::testing::ContainerEq(face_x_vector_expected));

    std::vector<double> face_y_vector_expected{0.5, 1.5, 2.5, 0.5, 1.5, 2.5, 0.5, 1.5, 2.5};
    ASSERT_THAT(mesh2d.face_y, ::testing::ContainerEq(face_y_vector_expected));

    std::vector<int> face_nodes_vector_expected{
        1, 2, 4, 3,
        3, 4, 6, 5,
        5, 6, 8, 7,
        2, 9, 10, 4,
        4, 10, 11, 6,
        6, 11, 12, 8,
        9, 13, 14, 10,
        10, 14, 15, 11,
        11, 15, 16, 12};
    ASSERT_THAT(mesh2d.face_node, ::testing::ContainerEq(face_nodes_vector_expected));

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, DefineAndPut_OneMesh2D_ShouldWriteData)
{
    // Prepare
    // Open a file
    std::string const file_path = TEST_WRITE_FOLDER + "/Mesh2DWrite.nc";
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_replace_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Fill data
    int name_long_length;
    error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    ugridapi::Mesh2D mesh2d;
    mesh2d.name = "mesh2d";
    mesh2d.node_x = {0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3};
    mesh2d.node_y = {0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3};

    mesh2d.num_nodes = 16;
    std::vector<int> edge_nodes{
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        2,
        9,
        4,
        10,
        6,
        11,
        8,
        12,
        9,
        13,
        10,
        14,
        11,
        15,
        12,
        16,
        1,
        3,
        3,
        5,
        5,
        7,
        2,
        4,
        4,
        6,
        6,
        8,
        9,
        10,
        10,
        11,
        11,
        12,
        13,
        14,
        14,
        15,
        15,
        16,
    };
    mesh2d.num_edges = 23;

    mesh2d.face_x = {0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 2.5, 2.5, 2.5};
    mesh2d.face_y = {0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3};
    mesh2d.num_faces = 9;
    std::vector<int> face_nodes{
        1, 2, 4, 3,
        3, 4, 6, 5,
        5, 6, 8, 7,
        2, 9, 10, 4,
        4, 10, 11, 6,
        6, 11, 12, 8,
        9, 13, 14, 10,
        10, 14, 15, 11,
        11, 15, 16, 12};
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

TEST(ApiTest, InquireAndGet_OneNetwork1D_ShouldReadNetwork1D)
{
    // Prepare
    // Open a file
    std::string const file_path = TEST_FOLDER + "/AllUGridEntities.nc";

    int file_mode = -1;
    auto error_code = ugridapi::ug_file_read_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    int file_id = -1;
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    int topology_type;
    error_code = ugridapi::ug_topology_get_network1d_enum(topology_type);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int num_topologies;
    error_code = ugridapi::ug_topology_get_count(file_id, topology_type, num_topologies);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(num_topologies, 1);

    // get the dimensions
    ugridapi::Network1D network1d;
    error_code = ug_network1d_inq(file_id, 0, network1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    int name_length;
    error_code = ugridapi::ug_name_get_length(name_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int long_names_length;
    error_code = ugridapi::ug_name_get_long_length(long_names_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    network1d.name.resize(long_names_length);
    network1d.node_x.resize(network1d.num_nodes);
    network1d.node_y.resize(network1d.num_nodes);
    network1d.edge_node.resize(network1d.num_edges * 2);
    network1d.geometry_nodes_x.resize(network1d.num_geometry_nodes);
    network1d.geometry_nodes_y.resize(network1d.num_geometry_nodes);
    network1d.num_edge_geometry_nodes.resize(network1d.num_edges);
    network1d.node_id.resize(name_length * network1d.num_nodes);
    network1d.node_long_name.resize(long_names_length * network1d.num_nodes);
    network1d.edge_id.resize(name_length * network1d.num_edges);
    network1d.edge_long_name.resize(long_names_length * network1d.num_edges);
    network1d.edge_length.resize(network1d.num_edges);

    // get the data
    error_code = ug_network1d_get(file_id, 0, network1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    std::vector<std::string> node_ids = tokenize(network1d.node_id, network1d.node_id.size() / network1d.num_nodes);
    std::vector<std::string> node_long_names = tokenize(network1d.node_long_name, network1d.node_long_name.size() / network1d.num_nodes);
    std::vector<std::string> edge_ids = tokenize(network1d.edge_id, network1d.edge_id.size() / network1d.num_edges);
    std::vector<std::string> edge_long_names = tokenize(network1d.edge_long_name, network1d.edge_long_name.size() / network1d.num_edges);
    //// Asserts
    ASSERT_EQ(network1d.num_edge_geometry_nodes[0], 25);
    for (auto i = 0; i < network1d.num_nodes; ++i)
    {
        right_trim_string(node_ids[i]);
        right_trim_string(node_long_names[i]);
        ASSERT_EQ("nodesids", node_ids[i]);
        ASSERT_EQ("nodeslongNames", node_long_names[i]);
    }

    for (auto i = 0; i < network1d.num_edges; ++i)
    {
        right_trim_string(edge_ids[i]);
        right_trim_string(edge_long_names[i]);
        ASSERT_EQ("branchids", edge_ids[i]);
        ASSERT_EQ("branchlongNames", edge_long_names[i]);
    }

    std::vector<double> node_x_expected{293.78, 538.89};
    ASSERT_THAT(network1d.node_x, ::testing::ContainerEq(node_x_expected));

    std::vector<double> node_y_vector_expected{27.48, 956.75};
    ASSERT_THAT(network1d.node_y, ::testing::ContainerEq(node_y_vector_expected));

    std::vector<int> edge_nodes_vector_expected{0, 1};
    ASSERT_THAT(network1d.edge_node, ::testing::ContainerEq(edge_nodes_vector_expected));

    std::vector<double> geometry_nodes_x_expected_vector{293.78, 278.97, 265.31, 254.17, 247.44, 248.3, 259.58,
                                                         282.24, 314.61, 354.44, 398.94, 445, 490.6, 532.84, 566.64, 589.08,
                                                         600.72, 603.53, 599.27, 590.05, 577.56, 562.97, 547.12, 530.67, 538.89};

    ASSERT_THAT(network1d.geometry_nodes_x, ::testing::ContainerEq(geometry_nodes_x_expected_vector));
    std::vector<double> geometry_nodes_y_expected_vector{27.48, 74.87, 122.59, 170.96, 220.12, 269.67, 317.89,
                                                         361.93, 399.39, 428.84, 450.76, 469.28, 488.89, 514.78, 550.83, 594.93,
                                                         643.09, 692.6, 742.02, 790.79, 838.83, 886.28, 933.33, 980.17, 956.75};
    ASSERT_THAT(network1d.geometry_nodes_y, ::testing::ContainerEq(geometry_nodes_y_expected_vector));

    std::vector<double> edge_lengths_expected_vector{1165.29};
    ASSERT_THAT(network1d.edge_length, ::testing::ContainerEq(edge_lengths_expected_vector));

    right_trim_string(network1d.name);
    ASSERT_EQ(network1d.name, "network");

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, DefineAndPut_OneNetwork1D_ShouldWriteData)
{
    std::string const file_path = TEST_WRITE_FOLDER + "/Network1DWrite.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_replace_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Fill data
    int name_long_length;
    error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    ugridapi::Network1D network1d;
    network1d.name = "network1d";
    network1d.node_x = {293.78, 538.89};
    network1d.node_y = {27.48, 956.75};
    network1d.num_nodes = 2;
    network1d.edge_node = {0, 1};
    network1d.num_edges = 1;
    network1d.geometry_nodes_x = {293.78, 278.97, 265.31, 254.17, 247.44, 248.3, 259.58,
                                  282.24, 314.61, 354.44, 398.94, 445, 490.6, 532.84, 566.64, 589.08,
                                  600.72, 603.53, 599.27, 590.05, 577.56, 562.97, 547.12, 530.67, 538.89};
    network1d.geometry_nodes_y = {27.48, 74.87, 122.59, 170.96, 220.12, 269.67, 317.89,
                                  361.93, 399.39, 428.84, 450.76, 469.28, 488.89, 514.78, 550.83, 594.93,
                                  643.09, 692.6, 742.02, 790.79, 838.83, 886.28, 933.33, 980.17, 956.75};
    network1d.num_edge_geometry_nodes = {25};

    network1d.num_geometry_nodes = 25;

    network1d.node_id = {"nodesids                                nodesids                                "};
    network1d.node_long_name = {"nodeslongNames                                                                  nodeslongNames                                                                  "};
    network1d.edge_id = {"branchids                               "};
    network1d.edge_long_name = {"branchlongNames                                                                 "};
    network1d.edge_length = {1165.29};

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
TEST(ApiTest, InquireAndGet_OneMesh1D_ShouldReadMesh1D)
{
    std::string const file_path = TEST_FOLDER + "/AllUGridEntities.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_read_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    int topology_type;
    error_code = ugridapi::ug_topology_get_mesh1d_enum(topology_type);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int num_topologies;
    error_code = ugridapi::ug_topology_get_count(file_id, topology_type, num_topologies);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(num_topologies, 1);

    // get the dimensions
    ugridapi::Mesh1D mesh1d;
    error_code = ug_mesh1d_inq(file_id, 0, mesh1d);
    ASSERT_EQ(25, mesh1d.num_nodes);
    ASSERT_EQ(24, mesh1d.num_edges);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    int name_length;
    error_code = ugridapi::ug_name_get_length(name_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int long_names_length;
    error_code = ugridapi::ug_name_get_long_length(long_names_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::unique_ptr<char> const name(new char[long_names_length]);
    mesh1d.name = name.get();

    std::unique_ptr<char> const network_name(new char[long_names_length]);
    mesh1d.network_name = network_name.get();

    mesh1d.node_edge_id.resize(mesh1d.num_nodes);
    mesh1d.node_edge_offset.resize(mesh1d.num_nodes);
    mesh1d.edge_node.resize(mesh1d.num_edges * 2);
    mesh1d.node_id.resize(name_length * mesh1d.num_nodes);
    mesh1d.node_long_name.resize(long_names_length * mesh1d.num_nodes);

    // get the data
    error_code = ug_mesh1d_get(file_id, 0, mesh1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    //// Assert
    std::vector<std::string> node_id = tokenize(mesh1d.node_id, mesh1d.node_id.size() / mesh1d.num_nodes);
    std::vector<std::string> node_long_name = tokenize(mesh1d.node_long_name, mesh1d.node_long_name.size() / mesh1d.num_nodes);

    for (auto i = 0; i < mesh1d.num_nodes; ++i)
    {
        right_trim_string(node_id[i]);
        right_trim_string(node_long_name[i]);
        ASSERT_EQ("meshnodeids", node_id[i]);
        ASSERT_EQ("meshnodelongnames", node_long_name[i]);
    }

    std::string network_name_string(mesh1d.network_name);
    right_trim_string(network_name_string);
    ASSERT_EQ("network", network_name_string);

    std::string mesh1d_name_string(mesh1d.name);
    right_trim_string(mesh1d_name_string);
    ASSERT_EQ("1dmesh", mesh1d_name_string);
}

TEST(ApiTest, DefineAndPut_OneMesh1D_ShouldWriteData)
{
    std::string const file_path = TEST_WRITE_FOLDER + "/Mesh1DWrite.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_replace_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Fill data
    int name_long_length;
    error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ugridapi::Mesh1D mesh1d;

    mesh1d.name = "mesh1d";
    mesh1d.network_name = "network1d";
    mesh1d.node_edge_id = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    mesh1d.node_edge_offset = {
        0, 49.65, 99.29, 148.92, 198.54, 248.09,
        297.62, 347.15, 396.66, 446.19, 495.8, 545.44, 595.08, 644.63, 694.04,
        743.52, 793.07, 842.65, 892.26, 941.89, 991.53, 1041.17, 1090.82,
        1140.46, 1165.29};

    mesh1d.num_nodes = 25;
    mesh1d.num_edges = 24;
    mesh1d.edge_node = { 0, 1,
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
                        23, 24};
    std::vector<std::string> ids;
    std::vector<std::string> long_names;
    for (auto i = 0; i < mesh1d.num_nodes; ++i)
    {
        ids.emplace_back("meshnodeids");
        long_names.emplace_back("meshnodelongnames");
    }

    int name_length;
    error_code = ugridapi::ug_name_get_long_length(name_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    mesh1d.node_id.resize(mesh1d.num_nodes * name_length);
    mesh1d.node_long_name.resize(mesh1d.num_nodes * name_long_length);

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

TEST(ApiTest, InquireAndGet_OneContact_ShouldReadContact)
{
    std::string const file_path = TEST_FOLDER + "/AllUGridEntities.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_read_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // get the number of topologies
    int topology_type;
    error_code = ugridapi::ug_topology_get_contacts_enum(topology_type);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int num_topologies;
    error_code = ugridapi::ug_topology_get_count(file_id, topology_type, num_topologies);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(num_topologies, 1);

    // get the dimensions
    ugridapi::Contacts contacts;
    error_code = ug_contacts_inq(file_id, 0, contacts);
    ASSERT_EQ(23, contacts.num_contacts);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    int name_length;
    error_code = ugridapi::ug_name_get_length(name_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int long_names_length;
    error_code = ugridapi::ug_name_get_long_length(long_names_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    contacts.name.resize(long_names_length);
    contacts.mesh_from_name.resize(long_names_length);
    contacts.mesh_to_name.resize(long_names_length);
    contacts.contact_name_id.resize(name_length * contacts.num_contacts);
    contacts.contact_name_long.resize(long_names_length * contacts.num_contacts);
    contacts.contact_type.resize(contacts.num_contacts);
    contacts.edges.resize(contacts.num_contacts * 2);

    // get the data
    error_code = ug_contacts_get(file_id, 0, contacts);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Asserts
    std::string mesh_from_name_string(contacts.mesh_from_name);
    right_trim_string(mesh_from_name_string);
    ASSERT_EQ("mesh2d", mesh_from_name_string);

    std::string mesh_to_name_string(contacts.mesh_to_name);
    right_trim_string(mesh_to_name_string);
    ASSERT_EQ("1dmesh", mesh_to_name_string);

    std::vector<std::string> contact_ids = tokenize(contacts.contact_name_id, contacts.contact_name_id.size() / contacts.num_contacts);
    std::vector<std::string> contacts_long_names = tokenize(contacts.contact_name_long, contacts.contact_name_long.size() / contacts.num_contacts);
    
    for (auto i = 0; i < contacts.num_contacts; ++i)
    {
        right_trim_string(contact_ids[i]);
        right_trim_string(contacts_long_names[i]);
        ASSERT_EQ("linkid", contact_ids[i]);
        ASSERT_EQ("linklongname", contacts_long_names[i]);
    }

    std::vector<int> edge_vector_expected{13, 1,
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
                                          353, 23};
    ASSERT_THAT(contacts.edges, ::testing::ContainerEq(edge_vector_expected));

    std::vector<int> contact_type_vector_expected{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    ASSERT_THAT(contacts.contact_type, ::testing::ContainerEq(contact_type_vector_expected));

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, DefineAndPut_OneContact_ShouldWriteAContact)
{
    std::string const file_path = TEST_WRITE_FOLDER + "/ContactsWrite.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_replace_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Fill data
    int name_long_length;
    error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ugridapi::Contacts contacts;
    std::unique_ptr<char> const name(new char[name_long_length]);
    string_to_char_array("2d1dlinks", name_long_length, name.get());
    contacts.name = name.get();
    std::unique_ptr<char> const mesh_from_name(new char[name_long_length]);
    string_to_char_array("mesh2d", name_long_length, mesh_from_name.get());
    contacts.mesh_from_name = mesh_from_name.get();
    std::unique_ptr<char> const mesh_to_name(new char[name_long_length]);
    string_to_char_array("mesh1d", name_long_length, mesh_to_name.get());
    contacts.mesh_to_name = mesh_to_name.get();

    int face_location_enum;
    error_code = ugridapi::ug_entity_get_face_location_enum(face_location_enum);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    contacts.mesh_from_location = face_location_enum;

    ugridapi::ug_entity_get_face_location_enum(face_location_enum);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    contacts.mesh_to_location = face_location_enum;
    contacts.contact_type = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    contacts.edges = {13, 1,
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
                      353, 23};
    contacts.num_contacts = 23;

    std::vector<std::string> ids;
    std::vector<std::string> long_names;
    for (auto i = 0; i < contacts.num_contacts; ++i)
    {
        ids.emplace_back("linkid");
        long_names.emplace_back("linklongname");
    }

    int name_length;
    error_code = ugridapi::ug_name_get_long_length(name_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    contacts.contact_name_id.resize(contacts.num_contacts * name_length);
    contacts.contact_name_long.resize(contacts.num_contacts * name_long_length);

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

TEST(ApiTest, GetTopologyAttributesNamesAndValues_OnResultFile_ShouldGetTopologyAttributesNamesAndValues)
{
    std::string const file_path = TEST_FOLDER + "/ResultFile.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_read_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Get mesh1d and its name
    int long_names_length;
    error_code = ugridapi::ug_name_get_long_length(long_names_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    ugridapi::Mesh1D mesh1d;
    mesh1d.name.resize(long_names_length);
    error_code = ug_mesh1d_get(file_id, 0, mesh1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    int attributes_count = 0;
    error_code = ugridapi::ug_variable_count_attributes(file_id, mesh1d.name, attributes_count);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<std::string> topology_attribute_names;
    error_code = ugridapi::ug_variable_get_attributes_names(file_id, mesh1d.name, topology_attribute_names);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<std::string> topology_attribute_values;
    error_code = ugridapi::ug_variable_get_attributes_values(file_id, mesh1d.name, topology_attribute_values);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Assert
    std::vector<std::string> expected_names{
        "cf_role",
        "edge_coordinates",
        "edge_dimension",
        "edge_node_connectivity",
        "long_name",
        "max_face_nodes_dimension",
        "node_coordinates",
        "node_dimension",
        "topology_dimension"};
    ASSERT_THAT(topology_attribute_names, ::testing::ContainerEq(expected_names));

    std::vector<std::string> expected_values{
        "mesh_topology",
        "mesh1d_edge_x mesh1d_edge_y",
        "nmesh1d_edge",
        "mesh1d_edge_nodes",
        "Topology data of 1D network",
        "max_nmesh1d_face_nodes",
        "mesh1d_node_x mesh1d_node_y",
        "nmesh1d_node",
        "1"};
   ASSERT_THAT(topology_attribute_values, ::testing::ContainerEq(expected_values));

    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, GetVariableAttributesNamesAndValues_OnResultFile_ShouldGetVariableAttributesNamesAndValues)
{
    std::string const file_path = TEST_FOLDER + "/ResultFile.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_read_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Set the mesh2d_edge_type variable name
    int long_names_length;
    error_code = ugridapi::ug_name_get_long_length(long_names_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::string var_name{"mesh2d_edge_type"};
    // Allocate arrays to get data names and variables
    int attributes_count = 0;
    error_code = ugridapi::ug_variable_count_attributes(file_id, var_name, attributes_count);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<std::string> attribute_names;
    error_code = ugridapi::ug_variable_get_attributes_names(file_id, var_name, attribute_names);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<std::string> attribute_values;
    error_code = ugridapi::ug_variable_get_attributes_values(file_id, var_name, attribute_values);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Assert
    std::vector<std::string> expected_names{
        "_FillValue",
        "cell_methods",
        "coordinates",
        "flag_meanings",
        "flag_values",
        "location",
        "long_name",
        "mesh",
        "standard_name",
        "units"};
    ASSERT_THAT(attribute_names, ::testing::ContainerEq(expected_names));

    std::vector<std::string> expected_values{
        "-999",
        "nmesh2d_edge: mean",
        "mesh2d_edge_x mesh2d_edge_y",
        "internal_closed internal boundary boundary_closed",
        "0 1 2 3",
        "edge",
        "edge type (relation between edge and flow geometry)",
        "mesh2d",
        "",
        ""};
    ASSERT_THAT(attribute_values, ::testing::ContainerEq(expected_values));

    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, GetDataVariables_OnResultFile_ShouldGetDataVariables)
{
    std::string const file_path = TEST_FOLDER + "/ResultFile.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_read_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Gets a data variable
    int dimensions_count = 0;
    int name_long_length;
    error_code = ugridapi::ug_name_get_long_length(name_long_length);

    std::string variable_name_to_retrive;
    variable_name_to_retrive.resize(name_long_length);
    variable_name_to_retrive = "mesh1d_s0";
    error_code = ugridapi::ug_variable_count_dimensions(file_id, variable_name_to_retrive, dimensions_count);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Get the dimensions of data variable
    std::vector<int> dimension_value;
    std::vector<std::string> dimension_name;
    error_code = ugridapi::ug_variable_get_data_dimensions(file_id, variable_name_to_retrive, dimension_name, dimension_value);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Compute the total dimension
    int total_dimension = 1;
    for (auto const& d : dimension_value)
    {
        total_dimension *= d; 
    }

    // Get the data
    std::vector<double> data_vector;
    data_vector.resize(total_dimension);
    ugridapi::ug_variable_get_data_double(file_id, variable_name_to_retrive, data_vector);

    std::vector<double> data;
    data.resize(5);
    for (int i = 0; i < 5; ++i)
    {
        data[i] = data_vector[i];
    }
    std::vector<double> data_expected_vector{
        -5.0,
        -5.0,
        -5.0,
        -5.0,
        -5.0,
    };
    ASSERT_THAT(data, ::testing::ContainerEq(data_expected_vector));

    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, DefineCoordinateReferenceSystem_OnExistingFile_ShouldDefineCoordinateReferenceSystem)
{
    std::string const file_path = TEST_WRITE_FOLDER + "/CoordinateReferenceSystem.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_replace_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Write a projected_coordinate_system variable
    int name_long_length;
    error_code = ugridapi::ug_name_get_long_length(name_long_length);
    std::unique_ptr<char> const variable_name(new char[name_long_length]);
    string_to_char_array("projected_coordinate_system", name_long_length, variable_name.get());

    error_code = ugridapi::ug_variable_int_define(file_id, variable_name.get());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    //// Add coordinate reference system
    define_variable_attributes(file_id, "projected_coordinate_system", "name", "Unknown projected");
    define_variable_attributes(file_id, "projected_coordinate_system", "epsg", std::vector<int>{0});
    define_variable_attributes(file_id, "projected_coordinate_system", "grid_mapping_name", "Unknown projected");
    define_variable_attributes(file_id, "projected_coordinate_system", "longitude_of_prime_meridian", std::vector<double>{0.0});
    define_variable_attributes(file_id, "projected_coordinate_system", "semi_major_axis", std::vector<double>{6378137.0});
    define_variable_attributes(file_id, "projected_coordinate_system", "semi_minor_axis", std::vector<double>{6356752.314245});
    define_variable_attributes(file_id, "projected_coordinate_system", "inverse_flattening", std::vector<double>{6356752.314245});
    define_variable_attributes(file_id, "projected_coordinate_system", "EPSG_code", "EPSG:0");
    define_variable_attributes(file_id, "projected_coordinate_system", "value", "value is equal to EPSG code");

    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, DefineGlobalAttributes_OnExistingFile_ShouldDefineGlobalAttributes)
{
    std::string const file_path = TEST_WRITE_FOLDER + "/Conventions.nc";

    // Open a file
    int file_id = -1;
    int file_mode = -1;
    auto error_code = ugridapi::ug_file_replace_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Write global attributes
    define_global_attributes(file_id, "institution", "Deltares");
    define_global_attributes(file_id, "references", "Unknown");
    define_global_attributes(file_id, "source", "Unknown Unknown. Model: Unknown");
    define_global_attributes(file_id, "history", "Created on 2017-11-27T18:05:09+0100, Unknown");
    define_global_attributes(file_id, "Conventions", "CF-1.6 UGRID-1.0/Deltares-0.8");

    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}
//------------------------------------------------------------------------------
std::vector<std::string> tokenize(const std::string& str, std::size_t count)
{
    std::vector<std::string> tokens;
    if (str.size() == 0)
    {
        return tokens;
    }
    size_t parts = str.size() / count;
    for (size_t i = 0, offset = 0; i < parts; ++i)
    {
        size_t size = count;
        if ((offset + size) < str.size())
        {
            std::string tmp_str = str.substr(offset, size);

            auto end = std::remove(tmp_str.begin(), tmp_str.end(), '\0');
            tmp_str.erase(end, tmp_str.end());

            tokens.push_back(tmp_str);
        }
        else
        {
            std::string tmp_str = str.substr(offset, str.size() - offset);

            auto end = std::remove(tmp_str.begin(), tmp_str.end(), '\0');
            tmp_str.erase(end, tmp_str.end());

            tokens.push_back(tmp_str);
        }
        offset += size;
    }
    return tokens;
}
