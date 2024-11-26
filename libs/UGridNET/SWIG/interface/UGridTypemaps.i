%include "typemaps.i"

%include "arrays_csharp.i"

CSHARP_ARRAYS_FIXED(char, byte)
CSHARP_ARRAYS(char, byte)

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

// %apply char INOUT[] { char* error_message } %{ 
//     int ug_error_get(char* error_message);
// %}

// %apply int INOUT[] { int* data } %{
//     int ug_variable_get_data_int(int file_id,
//                                  char const* variable_name,
//                                  int* data);
// %}

// %apply double INOUT[] { double* data } %{
//     int ug_variable_get_data_double(int file_id,
//                                     char const* variable_name,
//                                     double* data);
// %}

// %apply int INOUT[] { int* dimension_vec } %{
//     int ug_variable_get_data_dimensions(int file_id, 
//                                         char const* variable_name,
//                                         int* dimension_vec);
// %}

// %apply int INPUT[] { int const* attribute_values } %{
//     int ug_attribute_int_define(int file_id,
//                                 char const* variable_name,
//                                 char const* attribute_name,
//                                 int const* attribute_values,
//                                 int num_values);
// %}

// %apply double INPUT[] { double const* attribute_values } %{
//     int ug_attribute_double_define(int file_id,
//                                    char const* variable_name,
//                                    char const* attribute_name,
//                                    double const* attribute_values,
//                                    int num_values);
// %}

// --------
// pinning
// --------

%csmethodmodifiers ug_error_get "public unsafe";
%apply char FIXED[] { char* error_message } %{ 
    int ug_error_get(char* error_message);
%}

%csmethodmodifiers ug_file_open "public unsafe";
%apply char FIXED[] { const char* file_path };
 %{ 
    int ug_file_open(const char* file_path,
                     int mode, 
                     int& file_id);
%}

%csmethodmodifiers ug_topology_get_data_variables_names "public unsafe";
%apply char FIXED[] { char* data_variables_names_result } %{ 
    int ug_topology_get_data_variables_names(int file_id,
                                             ugridapi::TopologyType topology_type,
                                             int topology_id,
                                             ugridapi::MeshLocations location,
                                             char* data_variables_names_result);
%}

%csmethodmodifiers ug_topology_define_double_variable_on_location "public unsafe";
%apply char FIXED[] { const char* variable_name };
%apply char FIXED[] { const char* dimension_name };
 %{ 
    int ug_topology_define_double_variable_on_location(int file_id,
                                                       ugridapi::TopologyType topology_type,
                                                       int topology_id,
                                                       ugridapi::MeshLocations location,
                                                       const char* variable_name,
                                                       const char* dimension_name,
                                                       const int dimension_value);
%}

%csmethodmodifiers ug_variable_count_attributes "public unsafe";
%csmethodmodifiers ug_variable_count_dimensions "public unsafe";
%csmethodmodifiers ug_variable_int_define "public unsafe";
%csmethodmodifiers ug_attribute_global_char_define "public unsafe";


%csmethodmodifiers ug_variable_get_attributes_values "public unsafe";
%apply char FIXED[] { char* values } %{ 
    int ug_variable_get_attributes_values(int file_id,
                                          const char* variable_name,
                                          char* values);
%}

%csmethodmodifiers ug_variable_get_attributes_names "public unsafe";
%apply char FIXED[] { const char* variable_name };
%apply char FIXED[] { char* names } 
%{ 
    int ug_variable_get_attributes_names(int file_id,
                                         const char* variable_name,
                                         char* names);
%}

%csmethodmodifiers ug_variable_get_data_int "public unsafe";
%apply char FIXED[] { const char* variable_name };
%apply int FIXED[] {int *data}
%{
    int ug_variable_get_data_int(int file_id,
                                 const char* variable_name,
                                 int* data);
%}

%csmethodmodifiers ug_variable_get_data_double "public unsafe";
%apply char FIXED[] { const char* variable_name };
%apply double FIXED[] { double *data } %{ 
    int ug_variable_get_data_double(int file_id,
                                    const char* variable_name, 
                                    double* data);
%}

%csmethodmodifiers ug_variable_get_data_char "public unsafe";
%apply char FIXED[] { const char* variable_name };
%apply char FIXED[] { char* data } %{ 
    int ug_variable_get_data_char(int file_id,
                                  const char* variable_name,
                                  char* data);
%}

%csmethodmodifiers ug_variable_get_data_dimensions "public unsafe";
%apply char FIXED[] { const char* variable_name };
%apply int FIXED[] {int *dimension_vec} %{
    int ug_variable_get_data_dimensions(int file_id,
                                        const char* variable_name, 
                                        int* dimension_vec);
%}


%csmethodmodifiers ug_attribute_int_define "public unsafe";
%apply char FIXED[] { const char* variable_name }; 
%apply char FIXED[] { const char* attribute_name };
%apply int FIXED[] { int const* attribute_values }; 
%{
    int ug_attribute_int_define(int file_id,
                                const char* variable_name,
                                const char* attribute_name,
                                int const* attribute_values,
                                int num_values);
%}

%csmethodmodifiers ug_attribute_double_define "public unsafe";
%apply char FIXED[] { const char* variable_name }; 
%apply char FIXED[] { const char* attribute_name };
%apply double FIXED[] { double const* attribute_values };
%{
    int ug_attribute_double_define(int file_id,
                                   const char* variable_name,
                                   const char* attribute_name,
                                   double const* attribute_values,
                                   int num_values);
%}

%csmethodmodifiers ug_attribute_char_define "public unsafe";
%apply char FIXED[] { const char* variable_name };
%apply char FIXED[] { const char* attribute_name };
%apply char FIXED[] { const char* attribute_values };
%{
int ug_attribute_char_define(int file_id,
                             const char* variable_name,
                             const char* attribute_name,
                             const char* attribute_values,
                             int num_values);
%}

%define %TreatPointerToTypeAsSystemIntPtr(TYPE)
    %typemap(ctype)  TYPE* "TYPE*"
    %typemap(imtype) TYPE* "System.IntPtr"
    %typemap(cstype) TYPE* "System.IntPtr"
    %typemap(csin)   TYPE* "$csinput"
    %typemap(in)     TYPE* %{ $1 = $input; %}
    %typemap(out)    TYPE* %{ $result = $1; %}
    %typemap(csout, excode=SWIGEXCODE) TYPE* %{
        System.IntPtr cPtr = $imcall;$excode
        return cPtr;
    %}
    %typemap(csvarout, excode=SWIGEXCODE2) TYPE* %{
        get {
            System.IntPtr cPtr = $imcall;$excode
            return cPtr;
        }
    %}
%enddef

%TreatPointerToTypeAsSystemIntPtr(int)
%TreatPointerToTypeAsSystemIntPtr(double)
%TreatPointerToTypeAsSystemIntPtr(char)

// An extra step is needed after %TreatPointerToTypeAsSystemIntPtr(char)
// Disable default memory management for char*
// see:
//  https://www.swig.org/Doc4.2/SWIGDocumentation.html#SWIG_nn25
//  https://www.swig.org/Doc4.2/SWIGDocumentation.html#Perl5_nn19
// 
// Objective: Assign the pointer directly, do not delete then reallocate. Reallocation changes the address of the ptr.
// Reason: Client code must mange the ptr.
//
// Consider:
//
// struct MyStruct {
//   char* str;
// }
//
// By default SWIG generates:
//
// SWIGEXPORT void SWIGSTDCALL CSharp_UGridNET_MyStruct_str_set(void * jarg1, char* jarg2) {
//   ugridapi::MyStruct *arg1 = (ugridapi::MyStruct *) 0 ;
//   char *arg2 = (char *) 0 ;
//  
//   arg1 = (ugridapi::MyStruct *)jarg1; 
//   arg2 = jarg2; 
//   {
//     delete [] arg1->str;
//     if (arg2) {
//       arg1->str = (char *) (new char[strlen((const char *)arg2)+1]);
//       strcpy((char *)arg1->str, (const char *)arg2);
//     } else {
//       arg1->str = 0;
//     }
//   }
// }
//
// What is desired is
//
// SWIGEXPORT void SWIGSTDCALL CSharp_UGridNET_MyStruct_str_set(void * jarg1, char* jarg2) {
//   ugridapi::MyStruct *arg1 = (ugridapi::MyStruct *) 0 ;
//   char *arg2 = (char *) 0 ;
//  
//   arg1 = (ugridapi::MyStruct *)jarg1; 
//   arg2 = jarg2; 
//   arg1->str = arg2; 
// }

%typemap(memberin) char* %{ $1 = $input; %}
