using NUnit.Framework;
using UGridNET.Extensions;

namespace UGridNET.Tests
{
    [TestFixture]
    public class DisposableMesh2DTests
    {
        [Test]
        public void CreateNativeObject_WithPopulatedDisposableMesh2D_ReturnsMesh2DWithCorrectValues()
        {
            const int numNodes = 4;
            const int numEdges = 4;
            const int numFaces = 1;
            const int numFaceNodesMax = 4;

            const string name = "myMesh2D";
            const string gridMapping = "projected_coordinate_system";

            var nodeX = new[] { 0.0, 1.0, 1.0, 0.0 };
            var nodeY = new[] { 0.0, 0.0, 1.0, 1.0 };
            var nodeZ = new[] { 0.0, 0.0, 0.0, 0.0 };
            var edgeX = new[] { 0.5, 1.0, 0.5, 0.0 };
            var edgeY = new[] { 0.0, 0.5, 1.0, 0.5 };
            var edgeZ = new[] { 0.0, 0.0, 0.0, 0.0 };
            var edgeNodes = new[] { 0, 1, 1, 2, 2, 3, 3, 0 };
            var edgeFaces = new[] { 0, 0, 0, 0, 0, 0, 0, 0 };
            var faceX = new[] { 0.5 };
            var faceY = new[] { 0.5 };
            var faceZ = new[] { 0.0 };
            var faceBoundsX = new[] { 0.0, 1.0, 1.0, 0.0 };
            var faceBoundsY = new[] { 0.0, 0.0, 1.0, 1.0 };
            var faceNodes = new[] { 0, 1, 2, 3 };
            var faceEdges = new[] { 0, 1, 2, 3 };
            int[] faceFaces = { -1, -1, -1, -1 };

            var disposableMesh2D = new DisposableMesh2D
            {
                NumNodes = numNodes,
                NumEdges = numEdges,
                NumFaces = numFaces,
                NumFaceNodesMax = numFaceNodesMax,
                IsSpherical = true,
                Name = name.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                GridMapping = gridMapping.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NodeX = nodeX,
                NodeY = nodeY,
                NodeZ = nodeZ,
                EdgeX = edgeX,
                EdgeY = edgeY,
                EdgeZ = edgeZ,
                EdgeNodes = edgeNodes,
                EdgeFaces = edgeFaces,
                FaceX = faceX,
                FaceY = faceY,
                FaceZ = faceZ,
                FaceBoundsX = faceBoundsX,
                FaceBoundsY = faceBoundsY,
                FaceNodes = faceNodes,
                FaceEdges = faceEdges,
                FaceFaces = faceFaces
            };

            using (disposableMesh2D)
            {
                Mesh2D nativeMesh2D = disposableMesh2D.CreateNativeObject();

                Assert.That(nativeMesh2D.num_nodes, Is.EqualTo(numNodes));
                Assert.That(nativeMesh2D.num_edges, Is.EqualTo(numEdges));
                Assert.That(nativeMesh2D.num_faces, Is.EqualTo(numFaces));
                Assert.That(nativeMesh2D.num_face_nodes_max, Is.EqualTo(numFaceNodesMax));
                Assert.That(nativeMesh2D.is_spherical, Is.EqualTo(1));
                Assert.That(nativeMesh2D.name.CopyToArray<byte>(UGrid.name_long_length).GetStringFromNullTerminatedArray(true), Is.EqualTo(name));
                Assert.That(nativeMesh2D.grid_mapping.CopyToArray<byte>(UGrid.name_long_length).GetStringFromNullTerminatedArray(true), Is.EqualTo(gridMapping));
                Assert.That(nativeMesh2D.node_x.CopyToArray<double>(numNodes), Is.EqualTo(nodeX));
                Assert.That(nativeMesh2D.node_y.CopyToArray<double>(numNodes), Is.EqualTo(nodeY));
                Assert.That(nativeMesh2D.node_z.CopyToArray<double>(numNodes), Is.EqualTo(nodeZ));
                Assert.That(nativeMesh2D.edge_x.CopyToArray<double>(numEdges), Is.EqualTo(edgeX));
                Assert.That(nativeMesh2D.edge_y.CopyToArray<double>(numEdges), Is.EqualTo(edgeY));
                Assert.That(nativeMesh2D.edge_z.CopyToArray<double>(numEdges), Is.EqualTo(edgeZ));
                Assert.That(nativeMesh2D.edge_nodes.CopyToArray<int>(numEdges * 2), Is.EqualTo(edgeNodes));
                Assert.That(nativeMesh2D.edge_faces.CopyToArray<int>(numEdges * 2), Is.EqualTo(edgeFaces));
                Assert.That(nativeMesh2D.face_x.CopyToArray<double>(numFaces), Is.EqualTo(faceX));
                Assert.That(nativeMesh2D.face_y.CopyToArray<double>(numFaces), Is.EqualTo(faceY));
                Assert.That(nativeMesh2D.face_z.CopyToArray<double>(numFaces), Is.EqualTo(faceZ));
                Assert.That(nativeMesh2D.face_x_bnd.CopyToArray<double>(numFaces * numFaceNodesMax), Is.EqualTo(faceBoundsX));
                Assert.That(nativeMesh2D.face_y_bnd.CopyToArray<double>(numFaces * numFaceNodesMax), Is.EqualTo(faceBoundsY));
                Assert.That(nativeMesh2D.face_nodes.CopyToArray<int>(numFaces * numFaceNodesMax), Is.EqualTo(faceNodes));
                Assert.That(nativeMesh2D.face_edges.CopyToArray<int>(numFaces * numFaceNodesMax), Is.EqualTo(faceEdges));
                Assert.That(nativeMesh2D.face_faces.CopyToArray<int>(numFaces * numFaceNodesMax), Is.EqualTo(faceFaces));
            }
        }
    }
}