using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
using System.IO;
using System.Runtime.InteropServices;
using NUnit.Framework;
using UGridNET.Extensions;

namespace UGridNET.Tests
{

    [TestFixture]
    public class UGridNetTests
    {

        private static string GetTestDataPath()
        {
            string path = Environment.GetEnvironmentVariable("UGRID_TEST_DATA_DIR");

            // Check if the environment variable is set
            if (string.IsNullOrEmpty(path))
            {
                throw new InvalidOperationException("The UGRID_TEST_DATA_DIR environment variable is not set.");
            }

            // Check if the path is absolute and that it exists
            if (!(Path.IsPathRooted(path) && Directory.Exists(path)))
            {
                throw new InvalidOperationException(
                    $"The specified UGRID_TEST_DATA_DIR path must be absolute and should exist. UGRID_TEST_DATA_DIR={path}");
            }

            return path;
        }

        private static readonly string TestDataPath = GetTestDataPath();

        [Test]
        public void OpenExistingFileSucceeds()
        {
            string filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader ugrid = null;

            try
            {
                Assert.DoesNotThrow(() => ugrid = new UGridReader(filePath));
                var mesh2D = ugrid.Mesh2DList();
                var node_x = mesh2D[0].node_x.CopyToArray<double>(mesh2D[0].num_nodes);
                var node_y = mesh2D[0].node_y.CopyToArray<double>(mesh2D[0].num_nodes);
            }
            finally
            {
                ugrid?.Dispose();
            }

        }

        [Test]
        public void OpenNonExistingFileFails()
        {
            string filePath = "nonExistentFile.nc";
            UGridReader ugrid = null;

            try
            {
                Assert.Throws<Exception>(() => ugrid = new UGridReader(filePath));
            }
            finally
            {
                ugrid?.Dispose();
            }

        }


        [Test]
        public void GetEPSGCode()
        {
            string filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader ugrid = null;

            try
            {
                ugrid = new UGridReader(filePath);
                string EPSGCode = ugrid.GetEPSGCode();
                Assert.That(EPSGCode, Is.EqualTo("EPSG:0"));
                Console.WriteLine("EPSG code (test): {0}", EPSGCode);
            }
            finally
            {
                ugrid?.Dispose();
            }

        }

        [Test]
        public void GetConventions()
        {
            string filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader ugrid = null;

            try
            {
                ugrid = new UGridReader(filePath);

                string conventions = ugrid.GetConventions();
                Console.WriteLine("conventions (test): \"{0}\"", conventions);
                Assert.That(conventions, Is.EqualTo("CF-1.8 UGRID-1.0 Deltares-0.10"));
            }
            finally
            {
                ugrid?.Dispose();
            }

        }

        [Test]
        public void GetMesh2DAttributesByID()
        {
            string filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader ugrid = null;

            try
            {
                ugrid = new UGridReader(filePath);
                var attributes = ugrid.GetMesh2DAttributesByID(0);
                Assert.That(attributes["cf_role"], Is.EqualTo("mesh_topology"));
                Assert.That(attributes["long_name"], Is.EqualTo("Topology data of 2D mesh"));
                Assert.That(attributes["topology_dimension"], Is.EqualTo("2"));
                Assert.That(attributes["node_coordinates"], Is.EqualTo("mesh2d_node_x mesh2d_node_y"));
                Assert.That(attributes["node_dimension"], Is.EqualTo("mesh2d_nNodes"));
                Assert.That(attributes["max_face_nodes_dimension"], Is.EqualTo("mesh2d_nMax_face_nodes"));
                Assert.That(attributes["edge_node_connectivity"], Is.EqualTo("mesh2d_edge_nodes"));
                Assert.That(attributes["edge_dimension"], Is.EqualTo("mesh2d_nEdges"));
                Assert.That(attributes["edge_coordinates"], Is.EqualTo("mesh2d_edge_x mesh2d_edge_y"));
                Assert.That(attributes["face_node_connectivity"], Is.EqualTo("mesh2d_face_nodes"));
                Assert.That(attributes["face_dimension"], Is.EqualTo("mesh2d_nFaces"));
                Assert.That(attributes["face_coordinates"], Is.EqualTo("mesh2d_face_x mesh2d_face_y"));
            }
            finally
            {
                ugrid?.Dispose();
            }
        }

        [Test]
        public void GetMesh2DAttributesByName()
        {
            string filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader ugrid = null;

            try
            {
                ugrid = new UGridReader(filePath);
                var attributes = ugrid.GetMesh2DAttributesByName("mesh2d");
                Assert.That(attributes["cf_role"], Is.EqualTo("mesh_topology"));
                Assert.That(attributes["long_name"], Is.EqualTo("Topology data of 2D mesh"));
                Assert.That(attributes["topology_dimension"], Is.EqualTo("2"));
                Assert.That(attributes["node_coordinates"], Is.EqualTo("mesh2d_node_x mesh2d_node_y"));
                Assert.That(attributes["node_dimension"], Is.EqualTo("mesh2d_nNodes"));
                Assert.That(attributes["max_face_nodes_dimension"], Is.EqualTo("mesh2d_nMax_face_nodes"));
                Assert.That(attributes["edge_node_connectivity"], Is.EqualTo("mesh2d_edge_nodes"));
                Assert.That(attributes["edge_dimension"], Is.EqualTo("mesh2d_nEdges"));
                Assert.That(attributes["edge_coordinates"], Is.EqualTo("mesh2d_edge_x mesh2d_edge_y"));
                Assert.That(attributes["face_node_connectivity"], Is.EqualTo("mesh2d_face_nodes"));
                Assert.That(attributes["face_dimension"], Is.EqualTo("mesh2d_nFaces"));
                Assert.That(attributes["face_coordinates"], Is.EqualTo("mesh2d_face_x mesh2d_face_y"));
            }
            finally
            {
                ugrid?.Dispose();
            }
        }

        [Test]
        public void GetMesh2DAttributesByNameThrows()
        {
            string filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader ugrid = null;

            try
            {
                ugrid = new UGridReader(filePath);
                var attributes = new Dictionary<string, string>();
                Assert.Throws<InvalidOperationException>(() => attributes = ugrid.GetMesh2DAttributesByName("brrrrrrr"));
            }
            finally
            {
                ugrid?.Dispose();
            }
        }

        [Test]
        public void GetDoubleDataByVariableName()
        {
            int result = -1;

            int fileMode = -1;
            result = UGrid.ug_file_read_mode(ref fileMode);
            Assert.That(result, Is.EqualTo(0));

            int fileID = -1;
            string filePath = Path.Combine(TestDataPath, "ResultFile.nc");
            result = UGrid.ug_file_open(System.Text.Encoding.Default.GetBytes(filePath), fileMode, ref fileID);
            Assert.That(result, Is.EqualTo(0));

            int dimensionsCount = 0;
            string variableNameStr = "mesh1d_s0                                                                       ";
            byte[] variableName = System.Text.Encoding.Default.GetBytes(variableNameStr);
            result = UGrid.ug_variable_count_dimensions(fileID, variableName, ref dimensionsCount);
            Assert.That(result, Is.EqualTo(0));

            var dimensionVec = new int[dimensionsCount];
            UGrid.ug_variable_get_data_dimensions(fileID, variableName, dimensionVec);
            Assert.That(result, Is.EqualTo(0));

            int totalDimension = 1;
            for (int i = 0; i < dimensionsCount; i++)
            {
                totalDimension *= dimensionVec[i];
            }

            var data = new double[totalDimension];
            UGrid.ug_variable_get_data_double(fileID, variableName, data);
            Assert.That(result, Is.EqualTo(0));

            // compare 20 values between 86 and 106
            const int offset = 85;
            const int sampleSize = 20;
            double[] expectedData = new double[sampleSize]
            {
                -5.0,
                0.27,
                0.75,
                -0.36,
                0.27,
                0.56,
                0.83,
                0.79,
                0.8,
                0.81,
                0.88,
                0.63,
                0.7,
                1.02,
                1.07,
                0.97,
                1.02,
                1.14,
                1.13,
                0.9
            };

            for (int i = 0; i < sampleSize; i++)
            {
                Assert.That(data[i + offset], Is.EqualTo(expectedData[i]));
            }

            result = UGrid.ug_file_close(fileID);
            Assert.That(result, Is.EqualTo(0));
        }

        [Test]
        public void TestMesh1DReadAndInquire()
        {
            var mesh1D = new Mesh1D();
            try
            {
                int result = -1;

                int fileMode = -1;

                result = UGrid.ug_file_read_mode(ref fileMode);
                Assert.That(result, Is.EqualTo(0));

                int fileID = -1;
                string filePath = Path.Combine(TestDataPath, "AllUGridEntities.nc");
                result = UGrid.ug_file_open(System.Text.Encoding.Default.GetBytes(filePath), fileMode, ref fileID);
                Assert.That(result, Is.EqualTo(0));

                //// get the number of topologies
                int numTopologies = 0;
                result = UGrid.ug_topology_get_count(fileID, TopologyType.Mesh1dTopology, ref numTopologies);
                Assert.That(numTopologies, Is.EqualTo(1));
                Assert.That(result, Is.EqualTo(0));

                // get the mesh
                const int topologyId = 0;
                result = UGrid.ug_mesh1d_inq(fileID, topologyId, mesh1D);
                Assert.That(result, Is.EqualTo(0));
                Assert.That(mesh1D.num_nodes, Is.EqualTo(25));
                Assert.That(mesh1D.num_edges, Is.EqualTo(24));

                mesh1D.node_x = Marshal.AllocHGlobal(mesh1D.num_nodes * Marshal.SizeOf<double>());
                mesh1D.edge_nodes = Marshal.AllocHGlobal(mesh1D.num_edges * 2 * Marshal.SizeOf<int>());



                // get the data
                result = UGrid.ug_mesh1d_get(fileID, 0, mesh1D);
                Assert.That(result, Is.EqualTo(0));

                var arr_1 = mesh1D.edge_nodes.CopyToArray<int>(mesh1D.num_edges * 2);

                var src = new int[mesh1D.num_edges * 2];
                for (uint i = 0; i < mesh1D.num_edges * 2; i++)
                {
                    src[i] = 666;
                }

                mesh1D.edge_nodes.CopyFromArray(src);

                var arr_2 = mesh1D.edge_nodes.CopyToArray<int>(mesh1D.num_edges * 2);

                // for (uint i = 0; i < mesh1D.num_edges * 2; i++)
                // {
                //     Console.WriteLine("{0} : {1}", arr_1[i], arr_2[i]);
                // }

                Marshal.FreeHGlobal(mesh1D.node_x);
                Marshal.FreeHGlobal(mesh1D.edge_nodes);

                result = UGrid.ug_file_close(fileID);
                Assert.That(result, Is.EqualTo(0));
            }
            finally
            {
                mesh1D.Dispose();
            }
        }
    }
}