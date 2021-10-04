# UGrid

Deltares library for reading/writing UGrid geometrical entities. The UGrid entities available in this library are: Network1d, Mesh1d, Mesh2d and Contacts.
The library also offers APIs for:
- Getting simulation results defined on specific entity locations (data variables).
- Getting the topological entites attributes names and values, to facilitate the use of UGrid with Python xarray.

## Build

The requirements are:
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


