    module UGridInterfaces

    implicit none

    interface

    integer function ugio_open ( file_id, mode, filePath) bind ( C, name = "ug_open" )
    use iso_c_binding, only : C_CHAR, C_INT
    integer ( kind = C_INT ) :: file_id
    integer ( kind = C_INT ), value :: mode
    character ( kind = C_CHAR ) :: filePath ( * )
    end function ugio_open

    integer function ugio_close ( file_id) bind ( C, name = "ug_close" )
    use iso_c_binding, only :  C_INT
    integer ( kind = C_INT ), value :: file_id
    end function ugio_close

    integer function ugio_mesh2d_def(file_id, mesh2dInstance, topology_id) bind ( C, name = "ug_mesh2d_def" )
    use iso_c_binding, only :  C_INT
    use Mesh2D
    integer ( kind = C_INT ), value :: file_id
    integer ( kind = C_INT ) :: topology_id
    type(mesh2d_c), value :: mesh2dInstance
    end function ugio_mesh2d_def

    end interface

    end module UGridInterfaces
