#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TestUtils/Definitions.hpp>
#include <TestUtils/Utils.hpp>
#include <UGridAPI/UGrid.hpp>

static void define_variable_attributes(int file_id, std::string const& variable_name, std::string const& attribute_name, std::vector<int> const& attribute_values)
{
    // Write the char variable
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<char> variable_name_ptr(name_long_length);
    string_to_char_array(variable_name, name_long_length, variable_name_ptr.data());

    std::vector<char> attribute_name_ptr(name_long_length);
    string_to_char_array(attribute_name, name_long_length, attribute_name_ptr.data());

    error_code = ugridapi::ug_attribute_int_define(file_id,
                                                   variable_name_ptr.data(),
                                                   attribute_name_ptr.data(),
                                                   &attribute_values[0],
                                                   static_cast<int>(attribute_values.size()));
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

static void define_variable_attributes(int file_id, std::string const& variable_name, std::string const& attribute_name, std::vector<double> const& attribute_values)
{
    // Write the char variable
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<char> variable_name_ptr(name_long_length);
    string_to_char_array(variable_name, name_long_length, variable_name_ptr.data());

    std::vector<char> attribute_name_ptr(name_long_length);
    string_to_char_array(attribute_name, name_long_length, attribute_name_ptr.data());

    error_code = ugridapi::ug_attribute_double_define(file_id,
                                                      variable_name_ptr.data(),
                                                      attribute_name_ptr.data(),
                                                      &attribute_values[0],
                                                      static_cast<int>(attribute_values.size()));
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

static void define_variable_attributes(int file_id, std::string const& variable_name, std::string const& attribute_name, std::string const& attribute_value)
{
    // Write the char variable
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<char> variable_name_ptr(name_long_length);
    string_to_char_array(variable_name, name_long_length, variable_name_ptr.data());

    std::vector<char> attribute_name_ptr(name_long_length);
    string_to_char_array(attribute_name, name_long_length, attribute_name_ptr.data());

    error_code = ugridapi::ug_attribute_char_define(file_id,
                                                    variable_name_ptr.data(),
                                                    attribute_name_ptr.data(),
                                                    attribute_value.c_str(),
                                                    static_cast<int>(attribute_value.length()));
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

static void define_global_attributes(int file_id, std::string const& attribute_name, std::string const& attribute_value)
{
    // Write the char variable
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<char> attribute_name_ptr(name_long_length);
    string_to_char_array(attribute_name, name_long_length, attribute_name_ptr.data());

    error_code = ugridapi::ug_attribute_global_char_define(file_id,
                                                           attribute_name_ptr.data(),
                                                           attribute_value.c_str(),
                                                           static_cast<int>(attribute_value.length()));
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

static void create_ugrid_mesh(std::string mesh_name, int file_id)
{
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);

    ugridapi::Mesh2D mesh2d;
    std::vector<char> name(name_long_length);
    string_to_char_array(mesh_name, name_long_length, name.data());
    mesh2d.name = name.data();
    std::vector<double> node_x{0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3};

    mesh2d.node_x = node_x.data();
    std::vector<double> node_y{0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3};

    mesh2d.node_y = node_y.data();
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

    mesh2d.edge_nodes = edge_nodes.data();
    mesh2d.num_edges = 23;

    std::vector<double> face_x{0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 2.5, 2.5, 2.5};
    mesh2d.face_x = face_x.data();
    std::vector<double> face_y{0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3};
    mesh2d.face_y = face_y.data();
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
    mesh2d.face_nodes = face_nodes.data();
    mesh2d.num_face_nodes_max = 4;

    int topology_id = -1;
    error_code = ugridapi::ug_mesh2d_def(file_id, mesh2d, topology_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    error_code = ugridapi::ug_mesh2d_put(file_id, topology_id, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

std::string make_padded_string(const std::string& name, size_t num_spaces, size_t repetitions)
{
    std::string spaces(num_spaces, ' ');
    std::string result;

    for (size_t i = 0; i < repetitions; ++i)
    {
        result += name;
        result += spaces;
    }

    return result;
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
    int num_mesh2d_topologies;
    error_code = ugridapi::ug_topology_get_count(file_id, ugridapi::TopologyType::Mesh2dTopology, num_mesh2d_topologies);
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

    std::vector<char> name(name_long_length);
    mesh2d.name = name.data();
    std::vector<double> node_x(mesh2d.num_nodes);
    mesh2d.node_x = node_x.data();
    std::vector<double> node_y(mesh2d.num_nodes);
    mesh2d.node_y = node_y.data();
    std::vector<int> edge_nodes(mesh2d.num_edges * 2);
    mesh2d.edge_nodes = edge_nodes.data();
    std::vector<double> face_x(mesh2d.num_faces);
    mesh2d.face_x = face_x.data();
    std::vector<double> face_y(mesh2d.num_faces);
    mesh2d.face_y = face_y.data();
    std::vector<int> face_nodes(mesh2d.num_faces * mesh2d.num_face_nodes_max);
    mesh2d.face_nodes = face_nodes.data();
    mesh2d.start_index = 1;

    // Execute
    error_code = ug_mesh2d_get(file_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Assert
    std::string mesh_name(mesh2d.name);
    right_trim_string(mesh_name);
    ASSERT_EQ(mesh_name, "mesh2d");
    std::vector<double> node_x_vector(node_x.data(), node_x.data() + mesh2d.num_nodes);
    std::vector<double> node_x_expected{0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3};
    ASSERT_THAT(node_x_vector, ::testing::ContainerEq(node_x_expected));

    std::vector<double> node_y_vector(node_y.data(), node_y.data() + mesh2d.num_nodes);
    std::vector<double> node_y_vector_expected{0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3};
    ASSERT_THAT(node_y_vector, ::testing::ContainerEq(node_y_vector_expected));

    std::vector<int> edge_nodes_vector(edge_nodes.data(), edge_nodes.data() + mesh2d.num_edges * 2);
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
    ASSERT_THAT(edge_nodes_vector, ::testing::ContainerEq(edge_nodes_vector_expected));

    std::vector<double> face_x_vector(face_x.data(), face_x.data() + mesh2d.num_faces);
    std::vector<double> face_x_vector_expected{0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 2.5, 2.5, 2.5};
    ASSERT_THAT(face_x_vector, ::testing::ContainerEq(face_x_vector_expected));

    std::vector<double> face_y_vector(face_y.data(), face_y.data() + mesh2d.num_faces);
    std::vector<double> face_y_vector_expected{0.5, 1.5, 2.5, 0.5, 1.5, 2.5, 0.5, 1.5, 2.5};
    ASSERT_THAT(face_y_vector, ::testing::ContainerEq(face_y_vector_expected));

    std::vector<int> face_nodes_vector(face_nodes.data(), face_nodes.data() + mesh2d.num_faces * mesh2d.num_face_nodes_max);
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
    ASSERT_THAT(face_nodes_vector, ::testing::ContainerEq(face_nodes_vector_expected));

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
    int num_topologies;
    error_code = ugridapi::ug_topology_get_count(file_id, ugridapi::TopologyType::Network1dTopology, num_topologies);
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

    std::vector<char> name(long_names_length);
    network1d.name = name.data();

    std::vector<double> node_x(network1d.num_nodes);
    network1d.node_x = node_x.data();

    std::vector<double> node_y(network1d.num_nodes);
    network1d.node_y = node_y.data();

    std::vector<int> edge_node(network1d.num_edges * 2);
    network1d.edge_nodes = edge_node.data();

    std::vector<double> geometry_nodes_x(network1d.num_geometry_nodes);
    network1d.geometry_nodes_x = geometry_nodes_x.data();

    std::vector<double> geometry_nodes_y(network1d.num_geometry_nodes);
    network1d.geometry_nodes_y = geometry_nodes_y.data();

    std::vector<int> geometry_nodes_count(network1d.num_edges);
    network1d.num_edge_geometry_nodes = geometry_nodes_count.data();

    std::vector<char> node_id(name_length * network1d.num_nodes);
    network1d.node_id = node_id.data();

    std::vector<char> node_long_name(long_names_length * network1d.num_nodes);
    network1d.node_long_name = node_long_name.data();

    std::vector<char> edge_id(name_length * network1d.num_nodes);
    network1d.edge_id = edge_id.data();

    std::vector<char> edge_long_name(long_names_length * network1d.num_nodes);
    network1d.edge_long_name = edge_long_name.data();

    std::vector<double> edge_lengths(network1d.num_edges);
    network1d.edge_length = edge_lengths.data();

    // get the data
    error_code = ug_network1d_get(file_id, 0, network1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    //// Asserts
    ASSERT_EQ(network1d.num_edge_geometry_nodes[0], 25);
    std::string node_ids_string(node_id.data(), node_id.data() + name_length * network1d.num_nodes);
    std::string node_long_names_string(node_long_name.data(), node_long_name.data() + long_names_length * network1d.num_nodes);
    for (int i = 0; i < network1d.num_nodes; ++i)
    {
        std::string node_id_string = node_ids_string.substr(i * name_length, name_length);
        right_trim_string(node_id_string);
        std::string node_long_name_string = node_long_names_string.substr(i * long_names_length, long_names_length);
        right_trim_string(node_long_name_string);
        ASSERT_EQ("nodesids", node_id_string);
        ASSERT_EQ("nodeslongNames", node_long_name_string);
    }

    std::string edge_ids_string(edge_id.data(), edge_id.data() + name_length * network1d.num_edges);
    std::string edge_long_names_string(edge_long_name.data(), edge_long_name.data() + long_names_length * network1d.num_edges);
    for (int i = 0; i < network1d.num_edges; ++i)
    {
        std::string edge_id_string = edge_ids_string.substr(i * name_length, name_length);
        right_trim_string(edge_id_string);
        std::string edge_long_name_string = edge_long_names_string.substr(i * long_names_length, long_names_length);
        right_trim_string(edge_long_name_string);
        ASSERT_EQ("branchids", edge_id_string);
        ASSERT_EQ("branchlongNames", edge_long_name_string);
    }

    std::vector<double> node_x_vector(node_x.data(), node_x.data() + network1d.num_nodes);
    std::vector<double> node_x_expected{293.78, 538.89};
    ASSERT_THAT(node_x_vector, ::testing::ContainerEq(node_x_expected));

    std::vector<double> node_y_vector(node_y.data(), node_y.data() + network1d.num_nodes);
    std::vector<double> node_y_vector_expected{27.48, 956.75};
    ASSERT_THAT(node_y_vector, ::testing::ContainerEq(node_y_vector_expected));

    std::vector<int> edge_nodes_vector(edge_node.data(), edge_node.data() + network1d.num_edges * 2);
    std::vector<int> edge_nodes_vector_expected{0, 1};
    ASSERT_THAT(edge_nodes_vector, ::testing::ContainerEq(edge_nodes_vector_expected));

    std::vector<double> geometry_nodes_x_vector(geometry_nodes_x.data(), geometry_nodes_x.data() + network1d.num_geometry_nodes);
    std::vector<double> geometry_nodes_x_expected_vector{293.78, 278.97, 265.31, 254.17, 247.44, 248.3, 259.58,
                                                         282.24, 314.61, 354.44, 398.94, 445, 490.6, 532.84, 566.64, 589.08,
                                                         600.72, 603.53, 599.27, 590.05, 577.56, 562.97, 547.12, 530.67, 538.89};

    ASSERT_THAT(geometry_nodes_x_vector, ::testing::ContainerEq(geometry_nodes_x_expected_vector));
    std::vector<double> geometry_nodes_y_vector(geometry_nodes_y.data(), geometry_nodes_y.data() + network1d.num_geometry_nodes);
    std::vector<double> geometry_nodes_y_expected_vector{27.48, 74.87, 122.59, 170.96, 220.12, 269.67, 317.89,
                                                         361.93, 399.39, 428.84, 450.76, 469.28, 488.89, 514.78, 550.83, 594.93,
                                                         643.09, 692.6, 742.02, 790.79, 838.83, 886.28, 933.33, 980.17, 956.75};
    ASSERT_THAT(geometry_nodes_y_vector, ::testing::ContainerEq(geometry_nodes_y_expected_vector));

    std::vector<double> edge_lengths_vector(edge_lengths.data(), edge_lengths.data() + network1d.num_edges);
    std::vector<double> edge_lengths_expected_vector{1165.29};
    ASSERT_THAT(edge_lengths_vector, ::testing::ContainerEq(edge_lengths_expected_vector));

    std::string network1d_name(network1d.name);
    right_trim_string(network1d_name);
    ASSERT_EQ(network1d_name, "network");

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
    std::vector<char> name(name_long_length);
    string_to_char_array("network1d", name_long_length, name.data());
    network1d.name = name.data();
    std::vector<double> node_x{293.78, 538.89};
    network1d.node_x = node_x.data();
    std::vector<double> node_y{27.48, 956.75};
    network1d.node_y = node_y.data();
    network1d.num_nodes = 2;
    std::vector<int> edge_node{0, 1};
    network1d.edge_nodes = edge_node.data();
    network1d.num_edges = 1;

    std::vector<double> geometry_nodes_x{293.78, 278.97, 265.31, 254.17, 247.44, 248.3, 259.58,
                                         282.24, 314.61, 354.44, 398.94, 445, 490.6, 532.84, 566.64, 589.08,
                                         600.72, 603.53, 599.27, 590.05, 577.56, 562.97, 547.12, 530.67, 538.89};
    network1d.geometry_nodes_x = geometry_nodes_x.data();

    std::vector<double> geometry_nodes_y{27.48, 74.87, 122.59, 170.96, 220.12, 269.67, 317.89,
                                         361.93, 399.39, 428.84, 450.76, 469.28, 488.89, 514.78, 550.83, 594.93,
                                         643.09, 692.6, 742.02, 790.79, 838.83, 886.28, 933.33, 980.17, 956.75};
    network1d.geometry_nodes_y = geometry_nodes_y.data();

    std::vector<int> geometry_nodes_count{25};
    network1d.num_edge_geometry_nodes = geometry_nodes_count.data();

    network1d.num_geometry_nodes = 25;

    std::string node_id = make_padded_string("nodesids", ugrid::name_length, 2);
    network1d.node_id = node_id.data();
    std::string node_long_name = make_padded_string("nodeslongNames", ugrid::name_long_length, 2);
    network1d.node_long_name = node_long_name.data();
    std::string edge_id = make_padded_string("branchids", ugrid::name_length, 1);
    network1d.edge_id = edge_id.data();
    std::string edge_long_name = make_padded_string("branchlongNames", ugrid::name_long_length, 1);
    network1d.edge_long_name = edge_long_name.data();
    std::vector<double> edge_lengths{1165.29};
    network1d.edge_length = edge_lengths.data();

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
    int num_topologies;
    error_code = ugridapi::ug_topology_get_count(file_id, ugridapi::TopologyType::Mesh1dTopology, num_topologies);
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

    std::vector<char> name(long_names_length);
    mesh1d.name = name.data();

    std::vector<char> network_name(long_names_length);
    mesh1d.network_name = network_name.data();

    std::vector<int> edge_id(mesh1d.num_nodes);
    mesh1d.node_edge_id = edge_id.data();

    std::vector<double> edge_offset(mesh1d.num_nodes);
    mesh1d.node_edge_offset = edge_offset.data();

    std::vector<int> edge_nodes(mesh1d.num_edges * 2);
    mesh1d.edge_nodes = edge_nodes.data();

    std::vector<char> node_id(name_length * mesh1d.num_nodes);
    mesh1d.node_id = node_id.data();

    std::vector<char> node_long_name(long_names_length * mesh1d.num_nodes);
    mesh1d.node_long_name = node_long_name.data();

    // get the data
    error_code = ug_mesh1d_get(file_id, 0, mesh1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    //// Assert
    std::vector<int> edge_id_vector(edge_id.data(), edge_id.data() + mesh1d.num_nodes);
    std::vector<double> edge_offset_vector(edge_offset.data(), edge_offset.data() + mesh1d.num_nodes);
    std::vector<int> edge_nodes_vector(edge_nodes.data(), edge_nodes.data() + mesh1d.num_edges * 2);

    std::string node_ids_string(node_id.data(), node_id.data() + name_length * mesh1d.num_nodes);
    std::string node_long_names_string(node_long_name.data(), node_long_name.data() + long_names_length * mesh1d.num_nodes);
    for (int i = 0; i < mesh1d.num_nodes; ++i)
    {
        std::string node_id_string = node_ids_string.substr(i * name_length, name_length);
        right_trim_string(node_id_string);
        std::string node_long_name_string = node_long_names_string.substr(i * long_names_length, long_names_length);
        right_trim_string(node_long_name_string);
        ASSERT_EQ("meshnodeids", node_id_string);
        ASSERT_EQ("meshnodelongnames", node_long_name_string);
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
    std::vector<char> name(name_long_length);
    string_to_char_array("mesh1d", name_long_length, name.data());
    mesh1d.name = name.data();

    std::string network_name("network1d                               ");
    mesh1d.network_name = network_name.data();

    std::vector<int> node_edge_id{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    mesh1d.node_edge_id = node_edge_id.data();

    std::vector<double> node_edge_offset{
        0, 49.65, 99.29, 148.92, 198.54, 248.09,
        297.62, 347.15, 396.66, 446.19, 495.8, 545.44, 595.08, 644.63, 694.04,
        743.52, 793.07, 842.65, 892.26, 941.89, 991.53, 1041.17, 1090.82,
        1140.46, 1165.29};
    mesh1d.node_edge_offset = node_edge_offset.data();

    mesh1d.num_nodes = 25;
    mesh1d.num_edges = 24;
    std::vector<int> edges_nodes{0, 1,
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

    mesh1d.edge_nodes = edges_nodes.data();

    std::vector<std::string> ids;
    std::vector<std::string> long_names;
    for (int i = 0; i < mesh1d.num_nodes; ++i)
    {
        ids.emplace_back("meshnodeids");
        long_names.emplace_back("meshnodelongnames");
    }

    int name_length;
    error_code = ugridapi::ug_name_get_long_length(name_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<char> node_name_id(mesh1d.num_nodes * name_length);
    vector_of_strings_to_char_array(ids, name_length, node_name_id.data());
    mesh1d.node_id = node_name_id.data();

    std::vector<char> contact_name_long(mesh1d.num_nodes * name_long_length);
    vector_of_strings_to_char_array(long_names, name_long_length, contact_name_long.data());
    mesh1d.node_long_name = contact_name_long.data();

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
    int num_topologies;
    error_code = ugridapi::ug_topology_get_count(file_id, ugridapi::TopologyType::ContactsTopology, num_topologies);
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

    std::vector<char> name(long_names_length);
    contacts.name = name.data();

    std::vector<char> mesh_from_name(long_names_length);
    contacts.mesh_from_name = mesh_from_name.data();

    std::vector<char> mesh_to_name(long_names_length);
    contacts.mesh_to_name = mesh_to_name.data();

    std::vector<char> contact_name_id(name_length * contacts.num_contacts);
    contacts.contact_name_id = contact_name_id.data();

    std::vector<char> contact_name_long(long_names_length * contacts.num_contacts);
    contacts.contact_name_long = contact_name_long.data();

    std::vector<int> contact_type(contacts.num_contacts);
    contacts.contact_type = contact_type.data();

    std::vector<int> edges(contacts.num_contacts * 2);
    contacts.edges = edges.data();

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

    std::string contacts_ids_string(contact_name_id.data(), contact_name_id.data() + name_length * contacts.num_contacts);
    std::string contacts_long_names_string(contact_name_long.data(), contact_name_long.data() + long_names_length * contacts.num_contacts);
    for (int i = 0; i < contacts.num_contacts; ++i)
    {
        std::string contact_id_string = contacts_ids_string.substr(i * name_length, name_length);
        right_trim_string(contact_id_string);
        std::string contact_long_name_string = contacts_long_names_string.substr(i * long_names_length, long_names_length);
        right_trim_string(contact_long_name_string);
        ASSERT_EQ("linkid", contact_id_string);
        ASSERT_EQ("linklongname", contact_long_name_string);
    }

    std::vector<int> edge_vector(edges.data(), edges.data() + contacts.num_contacts * 2);
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
    ASSERT_THAT(edge_vector, ::testing::ContainerEq(edge_vector_expected));

    std::vector<int> contact_type_vector(contact_type.data(), contact_type.data() + contacts.num_contacts);
    std::vector<int> contact_type_vector_expected{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    ASSERT_THAT(contact_type_vector, ::testing::ContainerEq(contact_type_vector_expected));

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
    std::vector<char> name(name_long_length);
    string_to_char_array("2d1dlinks", name_long_length, name.data());
    contacts.name = name.data();
    std::vector<char> mesh_from_name(name_long_length);
    string_to_char_array("mesh2d", name_long_length, mesh_from_name.data());
    contacts.mesh_from_name = mesh_from_name.data();
    std::vector<char> mesh_to_name(name_long_length);
    string_to_char_array("mesh1d", name_long_length, mesh_to_name.data());
    contacts.mesh_to_name = mesh_to_name.data();
    contacts.mesh_from_location = ugridapi::MeshLocations::Faces;
    contacts.mesh_to_location = ugridapi::MeshLocations::Faces;

    std::vector<int> contact_type{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    contacts.contact_type = contact_type.data();

    std::vector<int> edges{13, 1,
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
    contacts.edges = edges.data();

    contacts.num_contacts = 23;

    std::vector<std::string> ids;
    std::vector<std::string> long_names;
    for (int i = 0; i < contacts.num_contacts; ++i)
    {
        ids.emplace_back("linkid");
        long_names.emplace_back("linklongname");
    }

    int name_length;
    error_code = ugridapi::ug_name_get_long_length(name_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<char> contact_name_id(contacts.num_contacts * name_length);
    vector_of_strings_to_char_array(ids, name_length, contact_name_id.data());
    contacts.contact_name_id = contact_name_id.data();

    std::vector<char> contact_name_long(contacts.num_contacts * name_long_length);
    vector_of_strings_to_char_array(long_names, name_long_length, contact_name_long.data());
    contacts.contact_name_long = contact_name_long.data();

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
    std::vector<char> name(long_names_length);
    mesh1d.name = name.data();
    error_code = ug_mesh1d_get(file_id, 0, mesh1d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    int attributes_count = 0;
    error_code = ugridapi::ug_variable_count_attributes(file_id, mesh1d.name, attributes_count);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<char> topology_attributes_names(attributes_count * long_names_length);
    error_code = ugridapi::ug_variable_get_attributes_names(file_id, mesh1d.name, topology_attributes_names.data());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::string topology_attributes_names_string(topology_attributes_names.data(), topology_attributes_names.data() + long_names_length * attributes_count);
    auto names = split_string(topology_attributes_names_string, attributes_count, long_names_length);
    right_trim_string_vector(names);

    std::vector<char> topology_attributes_values(attributes_count * long_names_length);
    error_code = ugridapi::ug_variable_get_attributes_values(file_id, mesh1d.name, topology_attributes_values.data());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::string topology_attributes_values_string(topology_attributes_values.data(), topology_attributes_values.data() + long_names_length * attributes_count);
    auto values = split_string(topology_attributes_values_string, attributes_count, long_names_length);
    right_trim_string_vector(values);

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
    ASSERT_THAT(names, ::testing::ContainerEq(expected_names));

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
    ASSERT_THAT(values, ::testing::ContainerEq(expected_values));

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
    std::vector<char> variable_name(long_names_length);
    string_to_char_array("mesh2d_edge_type", long_names_length, variable_name.data());

    // Allocate arrays to get data names and variables
    int attributes_count = 0;
    error_code = ugridapi::ug_variable_count_attributes(file_id, variable_name.data(), attributes_count);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<char> attributes_names(attributes_count * long_names_length);
    error_code = ugridapi::ug_variable_get_attributes_names(file_id, variable_name.data(), attributes_names.data());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::string topology_attributes_names_string(attributes_names.data(), attributes_names.data() + long_names_length * attributes_count);
    auto names = split_string(topology_attributes_names_string, attributes_count, long_names_length);
    right_trim_string_vector(names);

    std::vector<char> attributes_values(attributes_count * long_names_length);
    error_code = ugridapi::ug_variable_get_attributes_values(file_id, variable_name.data(), attributes_values.data());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::string topology_attributes_values_string(attributes_values.data(), attributes_values.data() + long_names_length * attributes_count);
    auto values = split_string(topology_attributes_values_string, attributes_count, long_names_length);
    right_trim_string_vector(values);

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
    ASSERT_THAT(names, ::testing::ContainerEq(expected_names));

    std::vector<std::string> expected_values{
        "-999",
        "nmesh2d_edge: mean",
        "mesh2d_edge_x mesh2d_edge_y",
        "internal_closed internal boundary boundary_closed",
        "0 1 2 3",
        "edge",
        "edge type (relation between edge and flow geometry",
        "mesh2d",
        "",
        ""};
    ASSERT_THAT(values, ::testing::ContainerEq(expected_values));

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
    std::vector<char> variable_name_to_retrive(name_long_length);
    string_to_char_array("mesh1d_s0", name_long_length, variable_name_to_retrive.data());

    error_code = ugridapi::ug_variable_count_dimensions(file_id, variable_name_to_retrive.data(), dimensions_count);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Get the dimensions of data variable
    std::vector<int> dimensions(dimensions_count);
    error_code = ugridapi::ug_variable_get_data_dimensions(file_id, variable_name_to_retrive.data(), dimensions.data());
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    std::string dimension_vector(dimensions.data(), dimensions.data() + dimensions_count);

    // Compute the total dimension
    int total_dimension = 1;
    for (auto const& d : dimension_vector)
    {
        total_dimension *= d;
    }

    // Get the data
    std::vector<double> data(total_dimension);
    ugridapi::ug_variable_get_data_double(file_id, variable_name_to_retrive.data(), data.data());

    // Assert the first 5 values
    std::vector<double> data_vector(data.data(), data.data() + 5);
    std::vector<double> data_expected_vector{
        -5.0,
        -5.0,
        -5.0,
        -5.0,
        -5.0,
    };
    ASSERT_THAT(data_vector, ::testing::ContainerEq(data_expected_vector));

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
    std::vector<char> variable_name(name_long_length);
    string_to_char_array("projected_coordinate_system", name_long_length, variable_name.data());

    error_code = ugridapi::ug_variable_int_define(file_id, variable_name.data());
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

TEST(ApiTest, TopologyDefineDoubleVariableOnLocation_OnExistingFile_ShouldDefineDoubleVariable)
{
    // Prepare
    int name_long_length;
    auto error_code = ugridapi::ug_name_get_long_length(name_long_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ugridapi::Mesh2D mesh2d;
    std::vector<char> name(name_long_length);
    string_to_char_array("mesh2d", name_long_length, name.data());
    mesh2d.name = name.data();
    std::vector<double> node_x{0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3};

    mesh2d.node_x = node_x.data();
    std::vector<double> node_y{0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3};

    mesh2d.node_y = node_y.data();
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

    mesh2d.edge_nodes = edge_nodes.data();
    mesh2d.num_edges = 23;

    std::vector<double> face_x{0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 2.5, 2.5, 2.5};
    mesh2d.face_x = face_x.data();
    std::vector<double> face_y{0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3};
    mesh2d.face_y = face_y.data();
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
    mesh2d.face_nodes = face_nodes.data();
    mesh2d.num_face_nodes_max = 4;

    // Open file
    std::string const file_path = TEST_WRITE_FOLDER + "/Mesh2DWriteWithAVariable.nc";
    int file_mode = -1;
    error_code = ugridapi::ug_file_replace_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int file_id = -1;
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Write mesh2d
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int topology_id = -1;
    error_code = ug_mesh2d_def(file_id, mesh2d, topology_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, topology_id);
    error_code = ug_mesh2d_put(file_id, topology_id, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Open the file for writing
    error_code = ugridapi::ug_file_write_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Write s0 double variable on topology

    std::vector<char> variable_name(name_long_length);
    string_to_char_array("mesh2d_s0", name_long_length, variable_name.data());

    std::vector<char> dimension_name(name_long_length);
    string_to_char_array("numTimeSteps", name_long_length, dimension_name.data());

    error_code = ugridapi::ug_topology_define_double_variable_on_location(file_id,
                                                                          ugridapi::TopologyType::Mesh2dTopology,
                                                                          0,
                                                                          ugridapi::MeshLocations::Nodes,
                                                                          variable_name.data(),
                                                                          dimension_name.data(),
                                                                          10);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    define_variable_attributes(file_id, "mesh2d_s0", "standard_name", "sea_surface_height");
    define_variable_attributes(file_id, "mesh2d_s0", "long_name", "Water level on previous timestep");
    define_variable_attributes(file_id, "mesh2d_s0", "units", "m");
    define_variable_attributes(file_id, "mesh2d_s0", "_FillValue", std::vector<double>{-999.0});

    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}

TEST(ApiTest, InquireAndGetFaceEdges_OneMesh2D_ShouldReadMesh2D)
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
    int num_topologies;
    error_code = ugridapi::ug_topology_get_count(file_id, ugridapi::TopologyType::Network1dTopology, num_topologies);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(num_topologies, 1);

    // get the dimensions
    ugridapi::Mesh2D mesh2d;
    error_code = ug_mesh2d_inq(file_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Allocate data variables
    int name_length;
    error_code = ugridapi::ug_name_get_length(name_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    int long_names_length;
    error_code = ugridapi::ug_name_get_long_length(long_names_length);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<int> edge_faces(mesh2d.num_edges * 2);
    mesh2d.edge_faces = edge_faces.data();

    std::vector<int> face_edges(mesh2d.num_faces * mesh2d.num_face_nodes_max, -1);
    mesh2d.face_edges = face_edges.data();

    // get the data
    error_code = ug_mesh2d_get(file_id, 0, mesh2d);

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Assert
    std::vector<int> edge_faces_expected{23, 0, 45, 0, 56, 0, 67, 0, 78, 0};
    for (size_t i = 0; i < edge_faces_expected.size(); ++i)
    {
        ASSERT_EQ(edge_faces_expected[i], edge_faces[i]);
    }

    // No faces found, the initial -1 are retained
    std::vector<int> face_edges_expected{-1, -1, -1, -1};
    for (size_t i = 0; i < face_edges_expected.size(); ++i)
    {
        ASSERT_EQ(face_edges_expected[i], face_edges[i]);
    }
}

TEST(ApiTest, UGridVariableExists_ShouldReturnCorrectValueForExistingAndNonExistingVariables)
{
    std::string const file_path = TEST_FOLDER + "/AllUGridEntities.nc";

    int file_mode = -1;
    auto error_code = ugridapi::ug_file_read_mode(file_mode);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    int file_id = -1;
    error_code = ugridapi::ug_file_open(file_path.c_str(), file_mode, file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    // Non-existing variable 
    std::vector<char> non_existing_var(ugridapi::name_long_length);
    string_to_char_array("dummy_name", ugridapi::name_long_length, non_existing_var.data());

    int exists = -42; // sentinel value to ensure it gets updated
    error_code = ugridapi::ug_variable_inq(file_id, non_existing_var.data(), &exists);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(0, exists) << "Expected 'exists' to be 0 for non-existing variable";

    // Existing variable 
    std::vector<char> existing_var(ugridapi::name_long_length);
    string_to_char_array("mesh2d", ugridapi::name_long_length, existing_var.data());

    exists = -42; // reset to sentinel
    error_code = ugridapi::ug_variable_inq(file_id, existing_var.data(), &exists);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
    ASSERT_EQ(1, exists) << "Expected 'exists' to be 1 for existing variable";

    ugridapi::ug_file_close(file_id);
}

/*
TEST(ApiTest, DefineAndPut_TwoMesh2D_ShouldWriteData)
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
    std::string mesh_name_one = "mesh2d_one";
    create_ugrid_mesh(mesh_name_one, file_id);
    std::string mesh_name_two = "mesh2d_two";
    create_ugrid_mesh(mesh_name_two, file_id);

    // Close the file
    error_code = ugridapi::ug_file_close(file_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);
}
*/
