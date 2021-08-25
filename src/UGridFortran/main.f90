    program fortran_client

    use iso_c_binding, only : C_NULL_CHAR
    use UGridInterfaces
    use UGridSingleFunctions
    use Mesh2D

    implicit none

    integer, parameter                          :: max_path_len = 255
    integer                                     :: file_id, topology_id, err, file_mode, topology_type, mesh2d_count
    character(len=max_path_len)                 :: filePath
    type(mesh2d_c)                              :: mesh2d_put, mesh2d_get
    ! data dimension
    integer, parameter                          :: num_edges=4, num_nodes=4, num_faces=1, num_face_nodes_max=4
    ! variables
    double precision, dimension(:), allocatable :: node_x_put, node_y_put,node_x_get,node_y_get
    integer, dimension(:), allocatable          :: edge_nodes_put, edge_nodes_get
    character, dimension(:), allocatable        :: name_get

    filePath = "D:/ENGINES/io_netcdf_investigation/UGrid/UGrid/tests/data/test_file.nc" // C_NULL_CHAR

    ! Define geometry
    allocate(node_x_put(num_nodes))
    allocate(node_y_put(num_nodes))
    allocate(edge_nodes_put(num_edges))
    node_x_put(1) = 0
    node_x_put(2) = 1
    node_x_put(3) = 1
    node_x_put(4) = 0

    node_y_put(1) = 0
    node_y_put(2) = 0
    node_y_put(3) = 1
    node_y_put(4) = 1

    edge_nodes_put(1) = 1
    edge_nodes_put(2) = 2
    edge_nodes_put(3) = 2
    edge_nodes_put(4) = 3
    edge_nodes_put(5) = 3
    edge_nodes_put(6) = 4
    edge_nodes_put(7) = 4
    edge_nodes_put(8) = 1

    mesh2d_put%num_nodes =  num_nodes
    mesh2d_put%num_edges = num_edges
    mesh2d_put%num_face_nodes_max= num_face_nodes_max

    mesh2d_put%name =   c_loc("mesh2d" // C_NULL_CHAR)
    mesh2d_put%node_x = c_loc(node_x_put(1))
    mesh2d_put%node_y = c_loc(node_y_put(1))
    mesh2d_put%edge_nodes = c_loc(edge_nodes_put(1))
    mesh2d_put%start_index = 1
    ! --------------------------------------------------------------------------------!
    ! Write
    ! --------------------------------------------------------------------------------!

    ! Step 1. Open file in replace mode, ask the file mode for replacement to the library
    file_mode = ug_file_replace_mode()
    err = ug_open(filePath, file_mode, file_id)

    ! Step 2. Define 2d mesh into file (header part only)
    err = ug_mesh2d_def(file_id, mesh2d_put, topology_id)

    ! Step 3. Put 2d mesh into file
    err = ug_mesh2d_put(file_id, topology_id, mesh2d_put)

    ! Step 4. Close the file, end of the writing part
    err = ug_close(file_id)

    ! --------------------------------------------------------------------------------!
    ! Read
    ! --------------------------------------------------------------------------------!

    ! Step 1. Open file in read mode, ask the file mode for reading to the library
    file_mode = ug_file_read_mode()
    err = ug_open(filePath, file_mode, file_id)

    ! Step 2. Count the number of mesh2d topologies
    topology_type = ug_topology_get_mesh2d_type_enum()
    mesh2d_count = ug_topology_get_count(file_id, topology_type)

    ! Step 3. Inquire dimensions for the last mesh (mesh2d_count - 1 in zero based system, we can hide this because mesh2d has a start_index field)
    topology_id = mesh2d_count - 1
    err = ug_mesh2d_inq(file_id, topology_id, mesh2d_get)

    ! read with a single function
    err = ug_mesh2d_get_node_coordinates( file_id, topology_id, node_x_get, node_y_get)

    ! Step 4a. Get a single array: node_x_get needs to be allocated and assigned to the correct pointer in mes2d api structure
    allocate(node_x_get(mesh2d_get%num_nodes))
    mesh2d_get%node_x = c_loc(node_x_get(1))
    err = ug_mesh2d_get(file_id, topology_id, mesh2d_get)

    ! Step 4b. Get multiple arrays: each array needs to be allocated into the correct pointer in mes2d api structure
    allocate(node_y_get(mesh2d_get%num_nodes))
    allocate(edge_nodes_get(mesh2d_get%num_edges*2))
    mesh2d_get%node_y = c_loc(node_y_get(1))
    mesh2d_get%edge_nodes = c_loc(edge_nodes_get(1))
    err = ug_mesh2d_get(file_id, topology_id, mesh2d_get)

    ! Step 4c. Get a name
    allocate(name_get(max_path_len))
    mesh2d_get%name = c_loc(name_get(1))
    err = ug_mesh2d_get(file_id, topology_id, mesh2d_get)

    err = ug_close(file_id)

    end program fortran_client
