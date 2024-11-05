%include "typemaps.i"
%include "arrays_csharp.i"

%include "carrays.i"
%array_class(int, SwigIntArray);
%array_class(double, SwigDoubleArray);

%include "cpointer.i"
%pointer_class(int, SwigIntPointer);
%pointer_class(double, SwigDoublePointer);


%define %TypeRefParam(TYPE)
    %apply TYPE& INPUT { TYPE& };
    %apply TYPE& OUTPUT { TYPE& };
    %apply TYPE& INOUT { TYPE& };
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

%apply double INOUT[] { double* data } %{
    int ug_variable_get_data_double(int file_id, char const* variable_name, double* data);
%}

%apply int INOUT[] { int* data } %{
    int ug_variable_get_data_int(int file_id, char const* variable_name, int* data);
%}

%apply int INOUT[] { int* dimension_vec } %{
    int ug_variable_get_data_dimensions(int file_id, char const* variable_name, int* dimension_vec);
%}

%apply int INPUT[] { int const* attribute_values } %{
    int ug_attribute_int_define(int file_id, char const* variable_name, char const* attribute_name, int const* attribute_values, int num_values);
%}

%apply double INPUT[] { double const* attribute_values } %{
    int ug_attribute_double_define(int file_id, char const* variable_name, char const* attribute_name, double const* attribute_values, int num_values);
%}

%apply double INPUT[] { double const* attribute_values } %{
    int ug_attribute_double_define(int file_id, char const* variable_name, char const* attribute_name, double const* attribute_values, int num_values);
%}

//--------
// pinning
//--------
// %csmethodmodifiers ug_variable_get_data_double "public unsafe";
// %apply double FIXED[] { double *data } %{ 
//     int ug_variable_get_data_double(int file_id, char const* variable_name, double* data);
// %}

// %csmethodmodifiers ug_variable_get_data_int "public unsafe";
// %apply int FIXED[] {int *data} %{
//     int ug_variable_get_data_int(int file_id, char const* variable_name, int* data);
// %}

// %csmethodmodifiers ug_variable_get_data_dimensions "public unsafe";
// %apply int FIXED[] {int *dimension_vec} %{
//     int ug_variable_get_data_dimensions(int file_id, char const* variable_name, int* dimension_vec);
// %}

// %csmethodmodifiers ug_attribute_int_define "public unsafe";
// %apply int FIXED[] { int const* attribute_values } %{
//     int ug_attribute_int_define(int file_id, char const* variable_name, char const* attribute_name, int const* attribute_values, int num_values);
// %}

// %csmethodmodifiers ug_attribute_double_define "public unsafe";
// %apply double FIXED[] { double const* attribute_values } %{
//     int ug_attribute_double_define(int file_id, char const* variable_name, char const* attribute_name, double const* attribute_values, int num_values);
// %}
