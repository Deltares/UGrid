//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.2.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class UGrid {
  public static SWIGTYPE_p_std__unordered_mapT_ugridapi__MeshLocations_std__string_std__hashT_ugridapi__MeshLocations_t_t locations_attribute_names {
    set {
      UGridPINVOKE.locations_attribute_names_set(SWIGTYPE_p_std__unordered_mapT_ugridapi__MeshLocations_std__string_std__hashT_ugridapi__MeshLocations_t_t.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = UGridPINVOKE.locations_attribute_names_get();
      SWIGTYPE_p_std__unordered_mapT_ugridapi__MeshLocations_std__string_std__hashT_ugridapi__MeshLocations_t_t ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_std__unordered_mapT_ugridapi__MeshLocations_std__string_std__hashT_ugridapi__MeshLocations_t_t(cPtr, false);
      return ret;
    } 
  }

  public static SWIGTYPE_p_std__unordered_mapT_ugridapi__MeshLocations_ugrid__UGridFileDimensions_std__hashT_ugridapi__MeshLocations_t_t locations_ugrid_dimensions {
    set {
      UGridPINVOKE.locations_ugrid_dimensions_set(SWIGTYPE_p_std__unordered_mapT_ugridapi__MeshLocations_ugrid__UGridFileDimensions_std__hashT_ugridapi__MeshLocations_t_t.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = UGridPINVOKE.locations_ugrid_dimensions_get();
      SWIGTYPE_p_std__unordered_mapT_ugridapi__MeshLocations_ugrid__UGridFileDimensions_std__hashT_ugridapi__MeshLocations_t_t ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_std__unordered_mapT_ugridapi__MeshLocations_ugrid__UGridFileDimensions_std__hashT_ugridapi__MeshLocations_t_t(cPtr, false);
      return ret;
    } 
  }

  public static int ug_error_get(string error_message) {
    int ret = UGridPINVOKE.ug_error_get(error_message);
    return ret;
  }

  public static int ug_name_get_length(ref int length) {
    int ret = UGridPINVOKE.ug_name_get_length(ref length);
    return ret;
  }

  public static int ug_name_get_long_length(ref int length) {
    int ret = UGridPINVOKE.ug_name_get_long_length(ref length);
    return ret;
  }

  public static int ug_entity_get_node_location_enum(ref int location) {
    int ret = UGridPINVOKE.ug_entity_get_node_location_enum(ref location);
    return ret;
  }

  public static int ug_entity_get_edge_location_enum(ref int location) {
    int ret = UGridPINVOKE.ug_entity_get_edge_location_enum(ref location);
    return ret;
  }

  public static int ug_entity_get_face_location_enum(ref int location) {
    int ret = UGridPINVOKE.ug_entity_get_face_location_enum(ref location);
    return ret;
  }

  public static int ug_topology_get_network1d_enum(ref int topology_enum) {
    int ret = UGridPINVOKE.ug_topology_get_network1d_enum(ref topology_enum);
    return ret;
  }

  public static int ug_topology_get_mesh1d_enum(ref int topology_enum) {
    int ret = UGridPINVOKE.ug_topology_get_mesh1d_enum(ref topology_enum);
    return ret;
  }

  public static int ug_topology_get_mesh2d_enum(ref int topology_enum) {
    int ret = UGridPINVOKE.ug_topology_get_mesh2d_enum(ref topology_enum);
    return ret;
  }

  public static int ug_topology_get_contacts_enum(ref int topology_enum) {
    int ret = UGridPINVOKE.ug_topology_get_contacts_enum(ref topology_enum);
    return ret;
  }

  public static int ug_topology_get_count(int file_id, int topology_type, ref int topology_count) {
    int ret = UGridPINVOKE.ug_topology_get_count(file_id, topology_type, ref topology_count);
    return ret;
  }

  public static int ug_topology_count_data_variables(int file_id, int topology_type, int topology_id, int location, ref int data_variable_count) {
    int ret = UGridPINVOKE.ug_topology_count_data_variables(file_id, topology_type, topology_id, location, ref data_variable_count);
    return ret;
  }

  public static int ug_topology_get_data_variables_names(int file_id, int topology_type, int topology_id, int location, string data_variables_names_result) {
    int ret = UGridPINVOKE.ug_topology_get_data_variables_names(file_id, topology_type, topology_id, location, data_variables_names_result);
    return ret;
  }

  public static int ug_topology_define_double_variable_on_location(int file_id, int topology_type, int topology_id, int location, string variable_name, string dimension_name, int dimension_value) {
    int ret = UGridPINVOKE.ug_topology_define_double_variable_on_location(file_id, topology_type, topology_id, location, variable_name, dimension_name, dimension_value);
    return ret;
  }

  public static int ug_variable_count_attributes(int file_id, string variable_name, ref int attributes_count) {
    int ret = UGridPINVOKE.ug_variable_count_attributes(file_id, variable_name, ref attributes_count);
    return ret;
  }

  public static int ug_variable_get_attributes_values(int file_id, string variable_name, string values) {
    int ret = UGridPINVOKE.ug_variable_get_attributes_values(file_id, variable_name, values);
    return ret;
  }

  public static int ug_variable_get_attributes_names(int file_id, string variable_name, string names) {
    int ret = UGridPINVOKE.ug_variable_get_attributes_names(file_id, variable_name, names);
    return ret;
  }

  public static int ug_variable_count_dimensions(int file_id, string variable_name, ref int dimensions_count) {
    int ret = UGridPINVOKE.ug_variable_count_dimensions(file_id, variable_name, ref dimensions_count);
    return ret;
  }

  public static int ug_variable_get_data_dimensions(int file_id, string variable_name, SWIGTYPE_p_int dimension_vec) {
    int ret = UGridPINVOKE.ug_variable_get_data_dimensions(file_id, variable_name, SWIGTYPE_p_int.getCPtr(dimension_vec));
    return ret;
  }

  public static int ug_variable_get_data_double(int file_id, string variable_name, SWIGTYPE_p_double data) {
    int ret = UGridPINVOKE.ug_variable_get_data_double(file_id, variable_name, SWIGTYPE_p_double.getCPtr(data));
    return ret;
  }

  public static int ug_variable_get_data_int(int file_id, string variable_name, SWIGTYPE_p_int data) {
    int ret = UGridPINVOKE.ug_variable_get_data_int(file_id, variable_name, SWIGTYPE_p_int.getCPtr(data));
    return ret;
  }

  public static int ug_variable_get_data_char(int file_id, string variable_name, string data) {
    int ret = UGridPINVOKE.ug_variable_get_data_char(file_id, variable_name, data);
    return ret;
  }

  public static int ug_file_read_mode(ref int mode) {
    int ret = UGridPINVOKE.ug_file_read_mode(ref mode);
    return ret;
  }

  public static int ug_file_write_mode(ref int mode) {
    int ret = UGridPINVOKE.ug_file_write_mode(ref mode);
    return ret;
  }

  public static int ug_file_replace_mode(ref int mode) {
    int ret = UGridPINVOKE.ug_file_replace_mode(ref mode);
    return ret;
  }

  public static int ug_file_open(string file_path, int mode, ref int file_id) {
    int ret = UGridPINVOKE.ug_file_open(file_path, mode, ref file_id);
    return ret;
  }

  public static int ug_file_close(int file_id) {
    int ret = UGridPINVOKE.ug_file_close(file_id);
    return ret;
  }

  public static int ug_network1d_def(int file_id, Network1D network1d_api, ref int topology_id) {
    int ret = UGridPINVOKE.ug_network1d_def(file_id, Network1D.getCPtr(network1d_api), ref topology_id);
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_network1d_put(int file_id, int topology_id, Network1D network1d_api) {
    int ret = UGridPINVOKE.ug_network1d_put(file_id, topology_id, Network1D.getCPtr(network1d_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_network1d_inq(int file_id, int topology_id, Network1D network1d_api) {
    int ret = UGridPINVOKE.ug_network1d_inq(file_id, topology_id, Network1D.getCPtr(network1d_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_network1d_get(int file_id, int topology_id, Network1D network1d_api) {
    int ret = UGridPINVOKE.ug_network1d_get(file_id, topology_id, Network1D.getCPtr(network1d_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_mesh1d_def(int file_id, Mesh1D mesh1d_api, ref int topology_id) {
    int ret = UGridPINVOKE.ug_mesh1d_def(file_id, Mesh1D.getCPtr(mesh1d_api), ref topology_id);
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_mesh1d_put(int file_id, int topology_id, Mesh1D mesh1d_api) {
    int ret = UGridPINVOKE.ug_mesh1d_put(file_id, topology_id, Mesh1D.getCPtr(mesh1d_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_mesh1d_inq(int file_id, int topology_id, Mesh1D mesh1d_api) {
    int ret = UGridPINVOKE.ug_mesh1d_inq(file_id, topology_id, Mesh1D.getCPtr(mesh1d_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_mesh1d_get(int file_id, int topology_id, Mesh1D mesh1d_api) {
    int ret = UGridPINVOKE.ug_mesh1d_get(file_id, topology_id, Mesh1D.getCPtr(mesh1d_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_mesh2d_def(int file_id, Mesh2D mesh2d_api, ref int topology_id) {
    int ret = UGridPINVOKE.ug_mesh2d_def(file_id, Mesh2D.getCPtr(mesh2d_api), ref topology_id);
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_mesh2d_put(int file_id, int topology_id, Mesh2D mesh2d_api) {
    int ret = UGridPINVOKE.ug_mesh2d_put(file_id, topology_id, Mesh2D.getCPtr(mesh2d_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_mesh2d_inq(int file_id, int topology_id, Mesh2D mesh2d_api) {
    int ret = UGridPINVOKE.ug_mesh2d_inq(file_id, topology_id, Mesh2D.getCPtr(mesh2d_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_mesh2d_get(int file_id, int topology_id, Mesh2D mesh2d_api) {
    int ret = UGridPINVOKE.ug_mesh2d_get(file_id, topology_id, Mesh2D.getCPtr(mesh2d_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_contacts_def(int file_id, Contacts contacts_api, ref int topology_id) {
    int ret = UGridPINVOKE.ug_contacts_def(file_id, Contacts.getCPtr(contacts_api), ref topology_id);
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_contacts_put(int file_id, int topology_id, Contacts contacts_api) {
    int ret = UGridPINVOKE.ug_contacts_put(file_id, topology_id, Contacts.getCPtr(contacts_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_contacts_inq(int file_id, int topology_id, Contacts contacts_api) {
    int ret = UGridPINVOKE.ug_contacts_inq(file_id, topology_id, Contacts.getCPtr(contacts_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_contacts_get(int file_id, int topology_id, Contacts contacts_api) {
    int ret = UGridPINVOKE.ug_contacts_get(file_id, topology_id, Contacts.getCPtr(contacts_api));
    if (UGridPINVOKE.SWIGPendingException.Pending) throw UGridPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int ug_variable_int_define(int file_id, string variable_name) {
    int ret = UGridPINVOKE.ug_variable_int_define(file_id, variable_name);
    return ret;
  }

  public static int ug_attribute_int_define(int file_id, string variable_name, string attribute_name, SWIGTYPE_p_int attribute_values, int num_values) {
    int ret = UGridPINVOKE.ug_attribute_int_define(file_id, variable_name, attribute_name, SWIGTYPE_p_int.getCPtr(attribute_values), num_values);
    return ret;
  }

  public static int ug_attribute_char_define(int file_id, string variable_name, string attribute_name, string attribute_values, int num_values) {
    int ret = UGridPINVOKE.ug_attribute_char_define(file_id, variable_name, attribute_name, attribute_values, num_values);
    return ret;
  }

  public static int ug_attribute_double_define(int file_id, string variable_name, string attribute_name, SWIGTYPE_p_double attribute_values, int num_values) {
    int ret = UGridPINVOKE.ug_attribute_double_define(file_id, variable_name, attribute_name, SWIGTYPE_p_double.getCPtr(attribute_values), num_values);
    return ret;
  }

  public static int ug_attribute_global_char_define(int file_id, string att_name, string attribute_values, int num_values) {
    int ret = UGridPINVOKE.ug_attribute_global_char_define(file_id, att_name, attribute_values, num_values);
    return ret;
  }

  public static int ug_get_edges_location_type(ref int type) {
    int ret = UGridPINVOKE.ug_get_edges_location_type(ref type);
    return ret;
  }

  public static int ug_get_nodes_location_type(ref int type) {
    int ret = UGridPINVOKE.ug_get_nodes_location_type(ref type);
    return ret;
  }

  public static int ug_get_faces_location_type(ref int type) {
    int ret = UGridPINVOKE.ug_get_faces_location_type(ref type);
    return ret;
  }

  public static int ug_get_int_fill_value(ref int fillValue) {
    int ret = UGridPINVOKE.ug_get_int_fill_value(ref fillValue);
    return ret;
  }

  public static int ug_get_double_fill_value(ref double fillValue) {
    int ret = UGridPINVOKE.ug_get_double_fill_value(ref fillValue);
    return ret;
  }

}
