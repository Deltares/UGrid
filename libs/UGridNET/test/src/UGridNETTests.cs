using System;
using NUnit.Framework;

namespace UGridNET.Tests
{

    [TestFixture]
    public class UGridNetTests
    {
        [Test]
        public void TestMesh1DReadAndInquire()
        {
            var mesh1D = new Mesh1D();
            try
            {
                int result = -1;

                const string filePath = "..\\..\\..\\..\\..\\tests\\data\\AllUGridEntities.nc";
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