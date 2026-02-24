
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

## Build

**Important**: UGrid must be built before UGridNET to ensure native dependencies are set up correctly.

### Building UGrid (C++)

**Prerequisites**:
- CMake 3.19 or higher
- A C++17 compatible compiler
- The Boost libraries
- Git
- Doxygen (optional)

On windows precompiled boost binaries (with MSVC compiler) can be downloaded here:

https://sourceforge.net/projects/boost/files/boost-binaries/

Once installed, modify boost environmental variables accordingly. For example:

```powershell
BOOST_INCLUDEDIR=C:\Apps\boost_1_74_0
BOOST_LIBRARYDIR=C:\Apps\boost_1_74_0\lib64-msvc-14.2
```

### IDE

To use an IDE, such as Visual Studio:

```powershell
cmake -S . -B xbuild -G"Visual Studio 16 2019"
cmake --open xbuild
```

### Command line

To configure:

```powershell
cmake -S . -B build
```

To build:

```powershell
cmake --build build
```

To build docs (requires Doxygen, output in `build/docs/html`):

```powershell
cmake --build build --target docs
```

### Building UGridNET (C#)

**Prerequisites**:
- .NET Framework 4.8 or higher
- .NET Standard 2.0 compatible runtime
- UGrid native library must be built first (see above)
- Visual Studio 2019 or higher, JetBrains Rider, or .NET CLI

The UGridNET project is located in the `UGridNET` directory and provides C# bindings for the native UGrid library.

#### Command line

To build:

```powershell
dotnet build UGridNET.sln
```

To run tests:

```powershell
dotnet test UGridNET.sln
```
