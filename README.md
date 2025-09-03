# UGrid

Deltares library for reading/writing UGrid files. The UGrid entities available in this library are: Network1d, Mesh1d, Mesh2d and Contacts.

The library also offers APIs for:
- Getting simulation results defined on specific entity locations (data variables).
- Getting the topological entities attributes names and values, to facilitate the use of UGrid with Python xarray.

Network1d, Mesh1d, and Contacts concern Deltares extensions to UGrid, which will be submitted to the UGrid community.

For more information about UGrid conventions:

https://ugrid-conventions.github.io/ugrid-conventions/

## Build

The requirements are:
- CMake 3.19 or higher
- A C++20 compatible compiler
- Git
- Doxygen (optional)


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
