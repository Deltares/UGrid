rmdir /s xbuild
rem cmake -S . -B xbuild -G"Visual Studio 16 2019" -DCMAKE_PREFIX_PATH="..\..\netcdf_4_8_0\lib\cmake\netCDF;..\..\..\hdf5\share\cmake\hdf5" --log-level=VERBOSE
cmake -S . -B xbuild -G"Visual Studio 16 2019" -DCMAKE_PREFIX_PATH="..\..\netcdf_4_8_0\lib\cmake\netCDF;..\..\..\hdf5\share\cmake\hdf5"