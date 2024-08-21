%include "typemaps.i"
 
%define %TypeRefParam(TYPE)
    %apply TYPE& INOUT { TYPE& };
    %apply TYPE& OUTPUT { TYPE& result };
%enddef
 

%TypeRefParam(bool)
%TypeRefParam(signed char)
%TypeRefParam(unsigned char)
%TypeRefParam(short)
%TypeRefParam(unsigned short)
%TypeRefParam(int)
%TypeRefParam(unsigned int)
%TypeRefParam(long)
%TypeRefParam(unsigned long)
%TypeRefParam(long long)
%TypeRefParam(unsigned long long)
%TypeRefParam(float)
%TypeRefParam(double)
//%TypeRefParam(long double)
