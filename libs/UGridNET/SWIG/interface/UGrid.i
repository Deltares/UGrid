#define UGRID_API

%module UGrid
%include "windows.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_unordered_map.i"
%include "std_unique_ptr.i"
%include "std_shared_ptr.i"

%include "UGridTypemaps.i"

%{
  #include "UGridAPI/Contacts.hpp"
  #include "UGridAPI/Mesh1D.hpp"
  #include "UGridAPI/Mesh2D.hpp"
  #include "UGridAPI/Network1D.hpp"
  #include "UGridAPI/UGrid.hpp"
  #include "UGridAPI/UGridState.hpp"
%}

%include "UGridAPI/Contacts.hpp"
%include "UGridAPI/Mesh1D.hpp"
%include "UGridAPI/Mesh2D.hpp"
%include "UGridAPI/Network1D.hpp"
%include "UGridAPI/UGrid.hpp"
%include "UGridAPI/UGridState.hpp"