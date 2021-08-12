rem remove xbuild
rmdir /s xbuild
rem set path_old=%path%
rem set path=%path%;"D:\ENGINES\io_netcdf_investigation\io_netcdf_cpp_second\extern\hdf5_compiled\bin"
rem set HDF5_DIR="D:\ENGINES\io_netcdf_investigation\io_netcdf_cpp_second\extern\hdf5_compiled\cmake\hdf5"
rem set HDF5_ROOT="D:\ENGINES\io_netcdf_investigation\io_netcdf_cpp_second\extern\hdf5_compiled"
cmake -S . -B xbuild -G"Visual Studio 16 2019" -DCMAKE_PREFIX_PATH="D:\ENGINES\io_netcdf_investigation\io_netcdf_cpp_second\extern\netcdf_second\netCDF 4.8.0\lib\cmake\netCDF;D:\ENGINES\io_netcdf_investigation\io_netcdf_cpp_second\extern\hdf5\share\cmake\hdf5" --log-level=VERBOSE
set path=%path_old%