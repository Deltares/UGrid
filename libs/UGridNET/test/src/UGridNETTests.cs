using System;
using System.IO;
using NUnit.Framework;

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
                throw new InvalidOperationException($"The specified UGRID_TEST_DATA_DIR path must be absolute and should exist. UGRID_TEST_DATA_DIR={path}");
            }

            return path;
        }

        private static readonly string TestDataPath = GetTestDataPath();

        [Test]
        public void TestMesh1DReadAndInquire()
        {
            var mesh1D = new Mesh1D();
            try
            {
                int result = -1;

                string filePath = Path.Combine(TestDataPath, "AllUGridEntities.nc");

                Console.WriteLine(filePath);
                int fileMode = -1;

                result = UGrid.ug_file_read_mode(ref fileMode);
                Assert.That(result, Is.EqualTo(0));
                Assert.That(result, Is.EqualTo(0));

                int fileId = -1;
                result = UGrid.ug_file_open(filePath, fileMode, ref fileId);
                Assert.That(result, Is.EqualTo(0));

                // get the number of topologies
                int topologyType = 0;
                result = UGrid.ug_topology_get_mesh1d_enum(ref topologyType);
                Assert.That(result, Is.EqualTo(0));

                int numTopologies = 0;
                result = UGrid.ug_topology_get_count(fileId, topologyType, ref numTopologies);
                Assert.That(numTopologies, Is.EqualTo(1));
                Assert.That(result, Is.EqualTo(0));

                // get the mesh
                const int topologyId = 0;
                result = UGrid.ug_mesh1d_inq(fileId, topologyId, mesh1D);
                Assert.That(result, Is.EqualTo(0));
                Assert.That(mesh1D.num_nodes, Is.EqualTo(25));
                Assert.That(mesh1D.num_edges, Is.EqualTo(24));

                // get the data
                result = UGrid.ug_mesh1d_get(fileId, 0, mesh1D);
                Assert.That(result, Is.EqualTo(0));
            }
            finally
            {
                mesh1D.Dispose();
            }
        }
    }
}