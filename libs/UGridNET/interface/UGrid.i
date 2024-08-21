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
  #include "Contacts.hpp"
  #include "Mesh1D.hpp"
  #include "Mesh2D.hpp"
  #include "Network1D.hpp"
  #include "UGridState.hpp"
  #include "UGrid.hpp"
%}

%include "Contacts.hpp"
%include "Mesh1D.hpp"
%include "Mesh2D.hpp"
%include "Network1D.hpp"
%include "UGridState.hpp"
%include "UGrid.hpp"
%include "UGrid.hpp"
