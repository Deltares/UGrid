using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using NUnit.Framework;
using UGridNET.Extensions;

namespace UGridNET.Tests
{

    public class SWIGTests
    {
        private static readonly string TestDataPath = TestUtilities.GetTestDataPath();

        [Test]
        public void GetDoubleDataByVariableName()
        {
            int result = -1;

            int fileMode = -1;
            result = UGrid.ug_file_read_mode(ref fileMode);
            Assert.That(result, Is.EqualTo(0));

            int fileID = -1;
            var filePath = Path.Combine(TestDataPath, "ResultFile.nc");
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
            Mesh1D mesh1D = new Mesh1D();
            try
            {
                int result = -1;

                int fileMode = -1;

                result = UGrid.ug_file_read_mode(ref fileMode);
                Assert.That(result, Is.EqualTo(0));

                int fileID = -1;
                var filePath = Path.Combine(TestDataPath, "AllUGridEntities.nc");
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

                result = UGrid.ug_file_close(fileID);
                Assert.That(result, Is.EqualTo(0));
            }
            finally
            {
                mesh1D?.Dispose();
            }
        }
    }
}