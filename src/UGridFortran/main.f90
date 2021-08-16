    program fortran_client

    use iso_c_binding, only : C_NULL_CHAR

    use UGridInterfaces
    use Mesh2D

    implicit none

    integer, parameter           :: max_path_len = 255
    integer                      :: file_id, ugrid_entity_id, mode, err
    character(len=max_path_len)  :: filePath
    type(mesh2d_c)               :: mesh2d
    filePath = "D:/ENGINES/io_netcdf_investigation/io_netcdf_cpp/test_file.nc" // C_NULL_CHAR

    ! create a file with  io_netcdf_cpp
    mode = 1
    err = ugio_open(file_id, mode, filePath)
    err = ugio_mesh2d_def(file_id, mesh2d, ugrid_entity_id)
    err = ugio_close(file_id)

    end program fortran_client
