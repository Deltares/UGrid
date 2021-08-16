#include <exception>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TestUtils/Definitions.hpp>
#include <UGridApi/UGrid.hpp>

TEST(ApiTest, Open_AFileWithOneMesh2d_ShouldReadFile)
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


    error_code = ug_mesh2d_get(ugrid_id, 0, mesh2d);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

    std::vector<double> node_x_vector(node_x.get(), node_x.get() + mesh2d.num_nodes);
    std::vector<double> node_y_vector(node_y.get(), node_y.get() + mesh2d.num_nodes);
    std::vector<int> edge_nodes_vector(edge_nodes.get(), edge_nodes.get() + mesh2d.num_edges * 2);
    std::vector<double> face_x_vector(face_x.get(), face_x.get() + mesh2d.num_faces);
    std::vector<double> face_y_vector(face_y.get(), face_y.get() + mesh2d.num_faces);
    std::vector<int> face_nodes_vector(face_nodes.get(), face_nodes.get() + mesh2d.num_faces * mesh2d.num_face_nodes_max);

    // close the file
    error_code = ugridapi::ug_close(ugrid_id);
    ASSERT_EQ(ugridapi::UGridioApiErrors::Success, error_code);

}