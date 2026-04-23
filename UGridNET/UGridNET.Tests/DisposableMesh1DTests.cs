using NUnit.Framework;
using UGridNET.Extensions;

namespace UGridNET.Tests
{
    [TestFixture]
    public class DisposableMesh1DTests
    {
        [Test]
        public void CreateNativeObject_WithPopulatedDisposableMesh1D_ReturnsMesh1DWithCorrectValues()
        {
            const int numNodes = 3;
            const int numEdges = 2;

            const string name = "myMesh1D";
            const string networkName = "myNetwork";
            const string gridMapping = "wgs84";

            var nodeX = new[] { 1.0, 2.0, 3.0 };
            var nodeY = new[] { 4.0, 5.0, 6.0 };
            var nodeEdgeID = new[] { 0, 1, 2 };
            var nodeEdgeOffset = new[] { 0.1, 0.2, 0.3 };
            var edgeX = new[] { 1.5, 2.5 };
            var edgeY = new[] { 4.5, 5.5 };
            var edgeNodes = new[] { 0, 1, 1, 2 };

            var disposableMesh1D = new DisposableMesh1D
            {
                NumNodes = numNodes,
                NumEdges = numEdges,
                IsSpherical = true,
                Name = name.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NetworkName = networkName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                GridMapping = gridMapping.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NodeX = nodeX,
                NodeY = nodeY,
                NodeEdgeID = nodeEdgeID,
                NodeEdgeOffset = nodeEdgeOffset,
                EdgeX = edgeX,
                EdgeY = edgeY,
                EdgeNodes = edgeNodes
            };

            using (disposableMesh1D)
            {
                Mesh1D nativeMesh1D = disposableMesh1D.CreateNativeObject();

                Assert.That(nativeMesh1D.num_nodes, Is.EqualTo(numNodes));
                Assert.That(nativeMesh1D.num_edges, Is.EqualTo(numEdges));
                Assert.That(nativeMesh1D.is_spherical, Is.EqualTo(1));
                Assert.That(nativeMesh1D.name.CopyToArray<byte>(UGrid.name_long_length).GetStringFromNullTerminatedArray(true), Is.EqualTo(name));
                Assert.That(nativeMesh1D.network_name.CopyToArray<byte>(UGrid.name_long_length).GetStringFromNullTerminatedArray(true), Is.EqualTo(networkName));
                Assert.That(nativeMesh1D.grid_mapping.CopyToArray<byte>(UGrid.name_long_length).GetStringFromNullTerminatedArray(true), Is.EqualTo(gridMapping));
                Assert.That(nativeMesh1D.node_x.CopyToArray<double>(numNodes), Is.EqualTo(nodeX));
                Assert.That(nativeMesh1D.node_y.CopyToArray<double>(numNodes), Is.EqualTo(nodeY));
                Assert.That(nativeMesh1D.node_edge_id.CopyToArray<int>(numNodes), Is.EqualTo(nodeEdgeID));
                Assert.That(nativeMesh1D.node_edge_offset.CopyToArray<double>(numNodes), Is.EqualTo(nodeEdgeOffset));
                Assert.That(nativeMesh1D.edge_x.CopyToArray<double>(numEdges), Is.EqualTo(edgeX));
                Assert.That(nativeMesh1D.edge_y.CopyToArray<double>(numEdges), Is.EqualTo(edgeY));
                Assert.That(nativeMesh1D.edge_nodes.CopyToArray<int>(numEdges * 2), Is.EqualTo(edgeNodes));
            }
        }
    }
}