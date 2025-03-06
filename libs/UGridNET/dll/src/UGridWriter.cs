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

        public void AddGlobalAttribute(string attributeName, string attributeValue) {
            Invoke(() => UGrid.ug_attribute_global_char_define(fileID,
                                                               attributeName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                                                               attributeValue.GetRightPaddedNullTerminatedBytes(attributeValue.Length+1),
                                                               attributeValue.Length));
        }

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
                projectedCoordinateSystem.Name.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.Name.Length + 1),
                projectedCoordinateSystem.Name.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "grid_mapping_name".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.GridMappingName.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.GridMappingName.Length + 1),
                projectedCoordinateSystem.GridMappingName.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "proj4_params".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.Proj4Params.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.Proj4Params.Length + 1),
                projectedCoordinateSystem.Proj4Params.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "EPSG_code".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.EPSGCode.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.EPSGCode.Length + 1),
                projectedCoordinateSystem.EPSGCode.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "projection_name".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.ProjectionName.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.ProjectionName.Length + 1),
                projectedCoordinateSystem.ProjectionName.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                fileID,
                variableName,
                "wkt".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                projectedCoordinateSystem.WKT.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.WKT.Length + 1),
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

        public void WriteTopologies()
        {
            WriteMesh1D();
            WriteMesh2D();
        }

    }

}