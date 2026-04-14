
# UGrid

Deltares library for reading/writing UGrid files. The UGrid entities available in this library are: Network1d, Mesh1d,
Mesh2d and Contacts.

The library also offers APIs for:

- Getting simulation results defined on specific entity locations (data variables).
- Getting the topological entities attributes names and values, to facilitate the use of UGrid with Python xarray.

Network1d, Mesh1d, and Contacts concern Deltares extensions to UGrid, which will be submitted to the UGrid community.

For more information about UGrid conventions:

https://ugrid-conventions.github.io/ugrid-conventions/

## Project Structure

The repository contains two main components:

- **UGrid**: Contains the C++ core library implementation
- **UGridNET**: Contains the C# .NET wrapper for the C++ library

## Building UGrid (C++)

**Prerequisites**:
- CMake 3.19 or higher
- A C++20 compatible compiler
- Git
- Doxygen (optional)

### Dependencies

```powershell
BOOST_INCLUDEDIR=C:\Apps\boost_1_74_0
BOOST_LIBRARYDIR=C:\Apps\boost_1_74_0\lib64-msvc-14.2
```

#### NetCDF

NetCDF (C and C++ bindings) and its dependencies are required for building UGrid. A bash script [`scripts/install_netcdf_cxx4.sh`](UGrid/scripts/install_netcdf_cxx4.sh) is provided to clone and build the full dependency chain from source:

- [ZLIB](https://github.com/madler/zlib)
- [Curl](https://github.com/curl/curl)
- [HDF5](https://github.com/HDFGroup/hdf5)
- [NetCDF-C](https://github.com/Unidata/netcdf-c)
- [NetCDF-CXX4](https://github.com/Unidata/netcdf-cxx4)

To run the script, use:

```bash
./UGrid/scripts/install_netcdf_cxx4.sh \
  --work_dir /path/to/work/directory \
  --install_dir /path/to/install/directory \
  --build_type Release
```

with `/path/to/work/directory` and `/path/to/install/directory` replaced with valid paths.

For more information regarding the script's options, use:

```bash
./UGrid/scripts/install_netcdf_cxx4.sh --help
```

Upon successful installation, to build UGrid successfully, it is important to either

- add the path to the install directory to the system path, or
- configure the UGrid build with `-DCMAKE_PREFIX_PATH=/path/to/install/directory`.

In addition, the following CMake variables need to be set explicitly:

- `-DnetCDFCxx_DIR=/path/to/install/directory/netcdf_cxx4/lib/cmake/netCDF`
- `-DZLIB_ROOT=/path/to/install/directory/zlib`
- `-DHDF5_DIR=/path/to/install/directory/hdf5/cmake`
- `-DHDF5_ROOT=/path/to/install/directory/hdf5`

### Configuring and Building UGrid

Follow the steps below to configure, build and install UGrid.

#### Steps

1.  To configure under Windows with Visual Studio, a solution is generated using

    ```powershell
    cmake -S <path-to-source-dir> -B <path-to-build-dir> -G `"Visual Studio 17 2022`" --install-prefix <path-to-install-dir> `
      -DCMAKE_PREFIX_PATH=<path-to-install-dir> `
      -DnetCDFCxx_DIR=<path-to-install-dir>/netcdf_cxx4/lib/cmake/netCDF `
      -DZLIB_ROOT=<path-to-install-dir>/zlib `
      -DHDF5_DIR=<path-to-install-dir>/hdf5/cmake `
      -DHDF5_ROOT=<path-to-install-dir>/hdf5
    ```

    This example uses Visual Studio 17 2022. A different version can be specified. In the above

    - `<path-to-source-dir>` is the path to the UGrid source tree (the top-level directory containing source files provided by the project).
    - `<path-to-build-dir>` is the path to the directory where UGrid is to be built.
    - `<path-to-install-dir>` is the path to the directory where UGrid is to be installed.

    Under Linux and macOS, the generator option can be omitted or the following can be used:

    ```bash
    cmake -S <path-to-source-dir> -B <path-to-build-dir> -G "Unix Makefiles" --install-prefix <path-to-install-dir> \
      -DCMAKE_PREFIX_PATH=<path-to-install-dir> \
      -DnetCDFCxx_DIR=<path-to-install-dir>/netcdf_cxx4/lib/cmake/netCDF \
      -DHDF5_DIR=<path-to-install-dir>/hdf5/cmake \
      -DHDF5_ROOT=<path-to-install-dir>/hdf5
    ```

2.  To build the project's targets, use:

    ```powershell
    cmake --build <path-to-build-dir> --config <cfg> --parallel <jobs>
    ```
    
    where

    - `<cfg>` is the build type (`Debug`, `Release`, `RelWithDebInfo` and `MinSizeRel`), see `CMAKE_BUILD_TYPE`.
    - `<jobs>` is the maximum number of concurrent processes to use when building.

    Note: this builds the documentation by default in `<path-to-build-dir>/docs/html`.

3.  To install, use:

    ```powershell
    cmake --install <path-to-build-dir>
    ```
    or
    ```powershell
    cmake --install <path-to-build-dir> --prefix <path-to-install-dir>
    ```
    to override the installation path specified during configuration (step 1).

### Additional configuration options

UGrid can be configured with a set of options, which are summarized in the table below.

| Option               | Description                                                          | Type | Default | Notes                                                                                 |
| :------------------- |:---------------------------------------------------------------------| :--- |:--------|:--------------------------------------------------------------------------------------|
| ENABLE_UNIT_TESTING  | Enables building the unit test executables                           | Bool | ON      |                                                                                       |
| ENABLE_CODE_COVERAGE | Generates code coverage statistics                                   | Bool | OFF     | Valid only under Linux when a GNU compiler is used (requires gcov), ignored otherwise |
| BUILD_DOTNET_WRAPPER | Configures and builds the C# targets that use code generated by SWIG | Bool | OFF     | Valid only under Windows, ignored otherwise                                           |

### CMakePresets.json

As an alternative to passing `-D` variables on the command line, a `CMakePresets.json` file can be used to store the configuration. Create a `CMakePresets.json` file in the root of the source tree with the following content, replacing `<path-to-install-dir>` with the actual install directory:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "default",
      "displayName": "Visual Studio 18 2026 (local)",
      "generator": "Visual Studio 18 2026",
      "binaryDir": "${sourceDir}/build",
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "<path-to-install-dir>",
        "netCDFCxx_DIR": "<path-to-install-dir>/netcdf_cxx4/lib/cmake/netCDF",
        "ZLIB_ROOT": "<path-to-install-dir>/zlib",
        "HDF5_DIR": "<path-to-install-dir>/hdf5/cmake",
        "HDF5_ROOT": "<path-to-install-dir>/hdf5",
        "BUILD_DOTNET_WRAPPER": "ON"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "debug",
      "displayName": "Debug",
      "configurePreset": "default",
      "configuration": "Debug"
    },
    {
      "name": "release",
      "displayName": "Release",
      "configurePreset": "default",
      "configuration": "Release"
    }
  ]
}
```

#### Command line

Configure using a preset:

```powershell
cmake -S <path-to-source-dir> --preset default
```

Build using a preset:

```powershell
cmake --build --preset release
```

#### Visual Studio

When `CMakePresets.json` is present in the source root, Visual Studio detects it automatically. Open the source folder in Visual Studio via **File → Open → Folder**, then select the desired configure preset from the **Configuration** dropdown and the build preset from the **Build** dropdown in the toolbar.

## Building UGridNET (C#)

**Important**: UGrid must be built before UGridNET to ensure native dependencies are set up correctly.

**Prerequisites**:
- .NET Framework 4.8 or higher
- .NET Standard 2.0 compatible runtime
- UGrid native library must be built first (see above)
- Visual Studio 2019 or higher, JetBrains Rider, or .NET CLI

The UGridNET project is located in the `UGridNET` directory and provides C# bindings for the native UGrid library.

### Command line

To build:

```powershell
dotnet build UGridNET.sln
```

To run tests:

```powershell
dotnet test UGridNET.sln
```
