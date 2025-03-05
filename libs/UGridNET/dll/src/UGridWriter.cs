using System;
using System.Runtime.InteropServices;
using UGridNET.Extensions;

namespace UGridNET
{
    public sealed class UGridWriter : UGridBase
    {
        public UGridWriter(string path) : base(path, FileMode.Write)
        {
            try
            {

            }
            catch
            {
                // If initialization fails, dispose of any partially initialized resources
                Dispose();
                // Rethrow to inform the caller of the failure
                throw;
            }
        }

        public void AddMesh1D(DisposableMesh1D disposableMesh1D)
        {
            var mesh1D = disposableMesh1D.CreateNativeObject();
            mesh1DList.Add(mesh1D);
            int topologyID = -1;
            Invoke(() => UGrid.ug_mesh1d_def(fileID, mesh1D, ref topologyID));
            //Invoke(() => UGrid.ug_mesh1d_put(fileID, topologyID, mesh1DList[topologyID]));
            //Invoke(() => UGrid.ug_mesh1d_put(fileID, topologyID, mesh1D));
        }

        public void AddMesh2D(DisposableMesh2D disposableMesh2D)
        {
            var mesh2D = disposableMesh2D.CreateNativeObject();
            mesh2DList.Add(mesh2D);
            int topologyID = -1;
            Invoke(() => UGrid.ug_mesh2d_def(fileID, mesh2D, ref topologyID));
            //Invoke(() => UGrid.ug_mesh2d_put(fileID, topologyID, mesh2DList[topologyID]));
            //Invoke(() => UGrid.ug_mesh2d_put(fileID, topologyID, mesh2D));
        }

        // public int DefineContact(string name, int numContacts)
        // {
        //     var contacts = new Contacts();
        //     contacts.num_contacts = numContacts;
        //     contacts.Allocate();
        //     byte[] bytesFromString = name.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
        //     contacts.name.CopyFromArray<byte>(bytesFromString);
        //     int topologyID = -1;
        //     Invoke(() => UGrid.ug_contacts_def(fileID, contacts, ref topologyID));
        //     contactsList.Add(contacts);
        //     return topologyID;
        // }

        // public int DefineNetwork1D(string name, int numNodes, int numEdges, int numGeometryNodes)
        // {
        //     var network1D = new Network1D();
        //     network1D.num_nodes = numNodes;
        //     network1D.num_edges = numEdges;
        //     network1D.num_geometry_nodes = numGeometryNodes;
        //     network1D.Allocate();
        //     byte[] bytesFromString = name.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
        //     network1D.name.CopyFromArray<byte>(bytesFromString);
        //     int topologyID = -1;
        //     Invoke(() => UGrid.ug_network1d_def(fileID, network1D, ref topologyID));
        //     network1DList.Add(network1D);
        //     return topologyID;
        // }

        public void AddProjectedCoordinateSystem(ProjectedCoordinateSystem projectedCoordinateSystem)
        {
            string variableNameStr = "projected_coordinate_system";
            var variableName = variableNameStr.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
            Invoke(() => UGrid.ug_variable_int_define(fileID, variableName));

            // integer attributes
            Invoke(() => UGrid.ug_attribute_int_define(
                fileID,
                variableName,
                "epsg".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                new int[] { projectedCoordinateSystem.EPSG },
                1));

            // double attributes
            Invoke(() => UGrid.ug_attribute_double_define(
                fileID,
                variableName,
                "longitude_of_prime_meridian".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                new double[] { projectedCoordinateSystem.LongitudeOfPrimeMeridian },
                1));

            Invoke(() => UGrid.ug_attribute_double_define(
                fileID,
                variableName,
                "semi_major_axis".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                new double[] { projectedCoordinateSystem.SemiMajorAxis },
                1));

            Invoke(() => UGrid.ug_attribute_double_define(
                fileID,
                variableName,
                "semi_minor_axis".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                new double[] { projectedCoordinateSystem.SemiMinorAxis },
                1));

            Invoke(() => UGrid.ug_attribute_double_define(
                fileID,
                variableName,
                "inverse_flattening".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                new double[] { projectedCoordinateSystem.InverseFlattening },
                1));

            // string attributes
            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "name".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.Name.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.Name.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "grid_mapping_name".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.GridMappingName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.GridMappingName.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "proj4_params".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.Proj4Params.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.Proj4Params.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "EPSG_code".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.EPSGCode.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.EPSGCode.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "projection_name".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.ProjectionName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.ProjectionName.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "wkt".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.WKT.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.WKT.Length));
        }

        private void WriteMesh1D()
        {
            for(int i = 0; i < mesh1DList.Count; i++) {
                Invoke(() => UGrid.ug_mesh1d_put(fileID, i, mesh1DList[i]));
            }
        }

        private void WriteMesh2D()
        {
            for(int i = 0; i < mesh2DList.Count; i++) {
                Invoke(() => UGrid.ug_mesh2d_put(fileID, i, mesh2DList[i]));
            }
        }

        private void WriteContacts()
        {
            for(int i = 0; i < contactsList.Count; i++) {
                Invoke(() => UGrid.ug_contacts_put(fileID, i, contactsList[i]));
            }
        }

        private void WriteNetwork1D()
        {
            for(int i = 0; i < network1DList.Count; i++) {
                Invoke(() => UGrid.ug_network1d_put(fileID, i, network1DList[i]));
            }
        }

        public void Write()
        {
            WriteMesh1D();
            WriteMesh2D();
            WriteContacts();
            WriteNetwork1D();
        }

    }

}