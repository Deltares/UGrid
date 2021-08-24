    module UGridInterfaces

    implicit none

    interface

    integer function ug_file_read_mode ( ) bind ( C, name = "ug_file_read_mode" )
    end function ug_file_read_mode

    integer function ug_file_write_mode ( ) bind ( C, name = "ug_file_write_mode" )
    end function ug_file_write_mode

    integer function ug_file_replace_mode ( ) bind ( C, name = "ug_file_replace_mode" )
    end function ug_file_replace_mode

    integer function ug_topology_get_mesh2d_type_enum() bind ( C, name = "ug_topology_get_mesh2d_type_enum" )
    end function ug_topology_get_mesh2d_type_enum

    integer function ug_open ( filePath, mode, file_id ) bind ( C, name = "ug_open" )
    use iso_c_binding, only : C_CHAR, C_INT
    integer ( kind = C_INT ) :: file_id
    integer ( kind = C_INT ), value :: mode
    character ( kind = C_CHAR ) :: filePath ( * )
    end function ug_open

    integer function ug_close ( file_id) bind ( C, name = "ug_close" )
    use iso_c_binding, only :  C_INT
    integer ( kind = C_INT ), value :: file_id
    end function ug_close

    integer function ug_topology_get_count(file_id, topology_type) bind ( C, name = "ug_topology_get_count" )
    use iso_c_binding, only :  C_INT
    integer ( kind = C_INT ), value :: file_id
    integer ( kind = C_INT ), value :: topology_type
    end function ug_topology_get_count


    integer function ug_mesh2d_def(file_id, mesh2dapi, topology_id) bind ( C, name = "ug_mesh2d_def" )
    use iso_c_binding, only :  C_INT
    use Mesh2D
    integer ( kind = C_INT ), value :: file_id
    integer ( kind = C_INT ) :: topology_id
    type(mesh2d_c) :: mesh2dapi
    end function ug_mesh2d_def

    integer function ug_mesh2d_put(file_id, topology_id, mesh2dapi) bind ( C, name = "ug_mesh2d_put" )
    use iso_c_binding, only :  C_INT
    use Mesh2D
    integer ( kind = C_INT ), value :: file_id
    integer ( kind = C_INT ), value :: topology_id
    type(mesh2d_c) :: mesh2dapi
    end function ug_mesh2d_put

    integer function ug_mesh2d_inq(file_id, topology_id, mesh2dapi) bind ( C, name = "ug_mesh2d_inq" )
    use iso_c_binding, only :  C_INT
    use Mesh2D
    integer ( kind = C_INT ), value :: file_id
    integer ( kind = C_INT ), value :: topology_id
    type(mesh2d_c) :: mesh2dapi
    end function ug_mesh2d_inq

    integer function ug_mesh2d_get(file_id, topology_id, mesh2dapi) bind ( C, name = "ug_mesh2d_get" )
    use iso_c_binding, only :  C_INT
    use Mesh2D
    integer ( kind = C_INT ), value :: file_id
    integer ( kind = C_INT ), value :: topology_id
    type(mesh2d_c) :: mesh2dapi
    end function ug_mesh2d_get




    end interface

    end module UGridInterfaces
