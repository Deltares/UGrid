using System.Collections.Generic;
using UGridNET.Extensions;

namespace UGridNET
{
    public sealed class UGridWriter : UGridBase
    {
        public UGridWriter(string path)
            : base(path, FileMode.Write)
        {
        }

        public void AddMesh1D(DisposableMesh1D disposableMesh1D)
        {
            Mesh1D mesh1D = disposableMesh1D.CreateNativeObject();
            Meshes1D.Add(mesh1D);
            int topologyId = -1;
            Invoke(() => UGrid.ug_mesh1d_def(FileID, mesh1D, ref topologyId));
        }

        public void AddMesh2D(DisposableMesh2D disposableMesh2D)
        {
            Mesh2D mesh2D = disposableMesh2D.CreateNativeObject();
            Meshes2D.Add(mesh2D);
            int topologyId = -1;
            Invoke(() => UGrid.ug_mesh2d_def(FileID, mesh2D, ref topologyId));
        }

        public void AddGlobalAttribute(string attributeName, string attributeValue)
        {
            Invoke(() => UGrid.ug_attribute_global_char_define(FileID,
                                                               attributeName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                                                               attributeValue.GetRightPaddedNullTerminatedBytes(attributeValue.Length + 1),
                                                               attributeValue.Length));
        }

        public void AddGlobalAttributes(Dictionary<string, string> globalAttributes)
        {
            foreach (KeyValuePair<string, string> globalAttribute in globalAttributes)
            {
                string key = globalAttribute.Key;
                string value = globalAttribute.Value;
                Invoke(() => UGrid.ug_attribute_global_char_define(FileID,
                                                                   key.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                                                                   value.GetRightPaddedNullTerminatedBytes(value.Length + 1),
                                                                   value.Length));
            }
        }

        public void AddProjectedCoordinateSystem(ProjectedCoordinateSystem projectedCoordinateSystem)
        {
            var variableNameStr = "projected_coordinate_system";
            if (projectedCoordinateSystem.EPSG.Equals(4326))
            {
                variableNameStr = "wgs84";
            }

            byte[] variableName = variableNameStr.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
            Invoke(() => UGrid.ug_variable_int_define(FileID, variableName));

            // integer attributes
            Invoke(() => UGrid.ug_attribute_int_define(
                       FileID,
                       variableName,
                       "epsg".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       new[] { projectedCoordinateSystem.EPSG },
                       1));

            // double attributes
            Invoke(() => UGrid.ug_attribute_double_define(
                       FileID,
                       variableName,
                       "longitude_of_prime_meridian".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       new[] { projectedCoordinateSystem.LongitudeOfPrimeMeridian },
                       1));

            Invoke(() => UGrid.ug_attribute_double_define(
                       FileID,
                       variableName,
                       "semi_major_axis".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       new[] { projectedCoordinateSystem.SemiMajorAxis },
                       1));

            Invoke(() => UGrid.ug_attribute_double_define(
                       FileID,
                       variableName,
                       "semi_minor_axis".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       new[] { projectedCoordinateSystem.SemiMinorAxis },
                       1));

            Invoke(() => UGrid.ug_attribute_double_define(
                       FileID,
                       variableName,
                       "inverse_flattening".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       new[] { projectedCoordinateSystem.InverseFlattening },
                       1));

            // string attributes
            Invoke(() => UGrid.ug_attribute_char_define(
                       FileID,
                       variableName,
                       "name".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       projectedCoordinateSystem.Name.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.Name.Length + 1),
                       projectedCoordinateSystem.Name.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                       FileID,
                       variableName,
                       "grid_mapping_name".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       projectedCoordinateSystem.GridMappingName.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.GridMappingName.Length + 1),
                       projectedCoordinateSystem.GridMappingName.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                       FileID,
                       variableName,
                       "proj4_params".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       projectedCoordinateSystem.Proj4Params.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.Proj4Params.Length + 1),
                       projectedCoordinateSystem.Proj4Params.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                       FileID,
                       variableName,
                       "EPSG_code".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       projectedCoordinateSystem.EPSGCode.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.EPSGCode.Length + 1),
                       projectedCoordinateSystem.EPSGCode.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                       FileID,
                       variableName,
                       "projection_name".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       projectedCoordinateSystem.ProjectionName.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.ProjectionName.Length + 1),
                       projectedCoordinateSystem.ProjectionName.Length));

            Invoke(() => UGrid.ug_attribute_char_define(
                       FileID,
                       variableName,
                       "wkt".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                       projectedCoordinateSystem.WKT.GetRightPaddedNullTerminatedBytes(projectedCoordinateSystem.WKT.Length + 1),
                       projectedCoordinateSystem.WKT.Length));
        }

        private void WriteMesh1D()
        {
            for (var i = 0; i < Meshes1D.Count; i++)
            {
                int index = i;
                Invoke(() => UGrid.ug_mesh1d_put(FileID, index, Meshes1D[index]));
            }
        }

        private void WriteMesh2D()
        {
            for (var i = 0; i < Meshes2D.Count; i++)
            {
                int index = i;
                Invoke(() => UGrid.ug_mesh2d_put(FileID, index, Meshes2D[index]));
            }
        }

        public void WriteTopologies()
        {
            WriteMesh1D();
            WriteMesh2D();
        }
    }
}