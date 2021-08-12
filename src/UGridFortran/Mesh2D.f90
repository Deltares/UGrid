    module Mesh2D
    use iso_c_binding
    implicit none

    !C types (used for communications)-----------------------!

    type, bind(C) :: mesh2d_c

        type(c_ptr)            :: edge_nodes
        type(c_ptr)            :: face_nodes
        type(c_ptr)            :: nodes_per_face
        type(c_ptr)            :: node_x
        type(c_ptr)            :: node_y
        type(c_ptr)            :: edge_x
        type(c_ptr)            :: edge_y
        type(c_ptr)            :: face_x
        type(c_ptr)            :: face_y

        integer(kind=c_int)    :: num_nodes
        integer(kind=c_int)    :: num_edges
        integer(kind=c_int)    :: num_faces
        integer(kind=c_int)    :: num_face_nodes

        type(c_ptr)            :: edge_faces
        type(c_ptr)            :: face_edges
        type(c_ptr)            :: face_face
        type(c_ptr)            :: node_z
        type(c_ptr)            :: edge_z
        type(c_ptr)            :: face_z
        type(c_ptr)            :: layer_zs
        type(c_ptr)            :: interface_zs
        type(c_ptr)            :: boundary_node_connectivity
        type(c_ptr)            :: volume_coordinates
        character(len=255)     :: name

        integer(c_int)         :: num_layers
        integer(c_int)         :: start_index

    end type mesh2d_c

    end module Mesh2D
