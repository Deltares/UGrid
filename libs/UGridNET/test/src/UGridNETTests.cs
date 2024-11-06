using System;
using System.IO;
using System.Runtime.InteropServices;
using NUnit.Framework;
using UGridNET.Tests.ExtensionMethods;

namespace UGridNET.Tests
{
    namespace ExtensionMethods
    {

        public static class SwigExtensions
        {


            public static SWIGTYPE_p_int NewInt(int length)
            {

                var array = new SwigIntArray((uint)length);
                return array.cast();
            }

            public static SWIGTYPE_p_double NewDouble(int length)
            {

                var array = new SwigDoubleArray((uint)length);
                return array.cast();
            }

            public static int[] ToArray(this SWIGTYPE_p_int ptr, int length)
            {
                if (ptr == null)
                {
                    throw new ArgumentNullException(nameof(ptr));
                }

                if (length <= 0)
                {
                    throw new ArgumentException("Length must be greater than zero.", nameof(length));
                }

                var swigArray = SwigIntArray.frompointer(ptr);

                var array = new int[length];

                for (uint i = 0; i < length; i++)
                {
                    array[i] = swigArray.getitem(i);
                }

                return array;
            }

            public static double[] ToArray(this SWIGTYPE_p_double ptr, int length)
            {
                if (ptr == null)
                {
                    throw new ArgumentNullException(nameof(ptr));
                }

                if (length <= 0)
                {
                    throw new ArgumentException("Length must be greater than zero.", nameof(length));
                }

                var swigArray = SwigDoubleArray.frompointer(ptr);

                var array = new double[length];

                for (uint i = 0; i < length; i++)
                {
                    array[i] = swigArray.getitem(i);
                }

                return array;
            }

            public static void FromArray(this SWIGTYPE_p_int ptr, int[] array, int length)
            {
                if (ptr == null)
                {
                    throw new ArgumentNullException(nameof(ptr));
                }

                if (length != array.Length)
                {
                    throw new ArgumentException("Length of the target does not match that of the source array.", nameof(length));
                }

                var swigArray = SwigIntArray.frompointer(ptr);

                for (uint i = 0; i < length; i++)
                {
                    swigArray.setitem(i, array[i]);
                }

            }

            public static void FromArray(this SWIGTYPE_p_double ptr, double[] array, int length)
            {
                if (ptr == null)
                {
                    throw new ArgumentNullException(nameof(ptr));
                }

                if (length != array.Length)
                {
                    throw new ArgumentException("Length of the target does not match that of the source array.", nameof(length));
                }

                var swigArray = SwigDoubleArray.frompointer(ptr);

                for (uint i = 0; i < length; i++)
                {
                    swigArray.setitem(i, array[i]);
                }

            }

        }

    }

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
                throw new InvalidOperationException($"The specified UGRID_TEST_DATA_DIR path must be absolute and should exist. UGRID_TEST_DATA_DIR={path}");
            }

            return path;
        }

        private static readonly string TestDataPath = GetTestDataPath();

        [Test]
        public void GetDoubleDataByVariableName()
        {
            int result = -1;

            int fileMode = -1;
            result = UGrid.ug_file_read_mode(ref fileMode);
            Assert.That(result, Is.EqualTo(0));

            int fileId = -1;
            string filePath = Path.Combine(TestDataPath, "ResultFile.nc");
            result = UGrid.ug_file_open(filePath, fileMode, ref fileId);
            Assert.That(result, Is.EqualTo(0));

            int dimensionsCount = 0;
            string variableName = "mesh1d_s0                                                                       ";
            result = UGrid.ug_variable_count_dimensions(fileId, variableName, ref dimensionsCount);
            Assert.That(result, Is.EqualTo(0));

            var dimensionVec = new int[dimensionsCount];
            UGrid.ug_variable_get_data_dimensions(fileId, variableName, dimensionVec);
            Assert.That(result, Is.EqualTo(0));

            int totalDimension = 1;
            for (int i = 0; i < dimensionsCount; i++)
            {
                totalDimension *= dimensionVec[i];
            }

            var data = new double[totalDimension];
            UGrid.ug_variable_get_data_double(fileId, variableName, data);
            Assert.That(result, Is.EqualTo(0));

            // compare 20 values between 86 and 106
            const int offset = 85;
            const int sampleSize = 20;
            double[] expectedData = new double[sampleSize] {
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

            //for (int i = 86; i < 106; i++)
            for (int i = 0; i < sampleSize; i++)
            {
                Assert.That(data[i + offset], Is.EqualTo(expectedData[i]));
            }

            result = UGrid.ug_file_close(fileId);
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

                int fileId = -1;
                string filePath = Path.Combine(TestDataPath, "AllUGridEntities.nc");
                result = UGrid.ug_file_open(filePath, fileMode, ref fileId);
                Assert.That(result, Is.EqualTo(0));

                //// get the number of topologies
                int numTopologies = 0;
                result = UGrid.ug_topology_get_count(fileId, TopologyType.Mesh1dTopology, ref numTopologies);
                Assert.That(numTopologies, Is.EqualTo(1));
                Assert.That(result, Is.EqualTo(0));

                // get the mesh
                const int topologyId = 0;
                result = UGrid.ug_mesh1d_inq(fileId, topologyId, mesh1D);
                Assert.That(result, Is.EqualTo(0));
                Assert.That(mesh1D.num_nodes, Is.EqualTo(25));
                Assert.That(mesh1D.num_edges, Is.EqualTo(24));

                //mesh1D.node_x = (new SwigDoubleArray((uint)mesh1D.num_nodes)).cast();
                //mesh1D.edge_nodes = (new SwigIntArray((uint)mesh1D.num_edges * 2)).cast();

                mesh1D.node_x = SwigExtensions.NewDouble(mesh1D.num_nodes);
                mesh1D.edge_nodes = SwigExtensions.NewInt(mesh1D.num_edges * 2);

                //SwigExtensions.Alloc(ref mesh1D.node_x, mesh1D.num_nodes);
                //SwigExtensions.Alloc(ref mesh1D.edge_nodes, mesh1D.num_edges * 2);

                // get the data
                result = UGrid.ug_mesh1d_get(fileId, 0, mesh1D);
                Assert.That(result, Is.EqualTo(0));


                var arr = SwigIntArray.frompointer(mesh1D.edge_nodes);
                var arr_2 = mesh1D.edge_nodes.ToArray(mesh1D.num_edges * 2);

                SWIGTYPE_p_int ptr = (new SwigIntArray((uint)mesh1D.num_edges * 2)).cast();
                ptr.FromArray(arr_2, mesh1D.num_edges * 2);
                var arr_3 = ptr.ToArray(mesh1D.num_edges * 2);

                for (uint i = 0; i < mesh1D.num_edges * 2; i++)
                {
                    Console.WriteLine("{0} : {1} : {2}", arr.getitem(i), arr_2[i], arr_3[i]);
                }


                result = UGrid.ug_file_close(fileId);
                Assert.That(result, Is.EqualTo(0));
            }
            finally
            {
                mesh1D.Dispose();
            }
        }
    }
}