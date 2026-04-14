using System;
using System.Collections.Generic;
using System.IO;
using NUnit.Framework;
using UGridNET.Extensions;

namespace UGridNET.Tests
{
    [TestFixture]
    public class UGridWriterTests
    {
        private string testOutputDir;
        private readonly bool cleanUpWhenDone = false;
        private readonly bool appendPathWithGuid = false;

        private readonly Dictionary<string, string> globalAttributes = new Dictionary<string, string>
        {
            { "institution", "Deltares" },
            { "references", "https://www.deltares.nl" },
            { "Conventions", "CF-1.8 UGRID-1.0 Deltares-0.10" }
        };

        [SetUp]
        public void SetUp()
        {
            var testOutputSubDir = "UGridNET_tests";
            if (appendPathWithGuid)
            {
                testOutputSubDir += '-' + Guid.NewGuid().ToString();
            }

            testOutputDir = Path.Combine(Path.GetTempPath(), testOutputSubDir);
            Directory.CreateDirectory(testOutputDir);
        }

        [TearDown]
        public void TearDown()
        {
            if (cleanUpWhenDone)
            {
                Directory.Delete(testOutputDir, true);
            }
        }

        [Test]
        [Order(0)]
        public void WritingMesh1DSucceeds()
        {
            string filePath = Path.Combine(testOutputDir, "mesh1D.nc");

            UGridWriter file = null;

            DisposableMesh1D disposableMesh1D = CreateDisposableMesh1D("myMesh1D", "myNetwork");
            ProjectedCoordinateSystem projectedCoordinateSystem = CreateCoordinateSystem();

            try
            {
                Assert.DoesNotThrow(() => file = new UGridWriter(filePath));
                Assert.DoesNotThrow(() => file.AddMesh1D(disposableMesh1D));
                Assert.DoesNotThrow(() => file.AddProjectedCoordinateSystem(projectedCoordinateSystem));
                Assert.DoesNotThrow(() => file.AddGlobalAttribute("source", "Unit test"));
                Assert.DoesNotThrow(() => file.AddGlobalAttributes(globalAttributes));
                Assert.DoesNotThrow(() => file.WriteTopologies());
                Assert.That(file.HasMesh1D, Is.True);
                Assert.That(file.Mesh1DList.Count, Is.EqualTo(1));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        [Order(1)]
        public void WritingMesh2DSucceeds()
        {
            string filePath = Path.Combine(testOutputDir, "mesh2D.nc");

            UGridWriter file = null;

            DisposableMesh2D disposableMesh2D = CreateDisposableMesh2D("myMesh2D");
            ProjectedCoordinateSystem projectedCoordinateSystem = CreateCoordinateSystem();

            try
            {
                Assert.DoesNotThrow(() => file = new UGridWriter(filePath));
                Assert.DoesNotThrow(() => file.AddMesh2D(disposableMesh2D));
                Assert.DoesNotThrow(() => file.AddProjectedCoordinateSystem(projectedCoordinateSystem));
                Assert.DoesNotThrow(() => file.AddGlobalAttribute("source", "Unit test"));
                Assert.DoesNotThrow(() => file.AddGlobalAttributes(globalAttributes));
                Assert.DoesNotThrow(() => file.WriteTopologies());
                Assert.That(file.HasMesh2D, Is.True);
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        [Order(2)]
        public void ImportingExportedMesh2DSucceeds()
        {
            string filePath = Path.Combine(testOutputDir, "mesh2D.nc");

            UGridReader file = null;

            var expectedNumNodes = 4;
            var expectedNumEdges = 4;
            var expectedNumFaces = 1;
            var expectedNodeX = new[] { 0.0, 1.0, 1.0, 0.0 };
            var expectedNodeY = new[] { 0.0, 0.0, 1.0, 1.0 };
            var expectedEdgeX = new[] { 0.5, 1.0, 0.5, 0.0 };
            var expectedEdgeY = new[] { 0.0, 0.5, 1.0, 0.5 };
            var expectedEdgeNodes = new[] { 0, 1, 1, 2, 2, 3, 3, 0 };
            var expectedFaceX = new[] { 0.5 };
            var expectedFaceY = new[] { 0.5 };

            try
            {
                file = new UGridReader(filePath);

                Assert.That(file.HasMesh2D, Is.True);
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
                Mesh2D mesh2D = file.Mesh2DList[0];
                Assert.That(mesh2D.num_nodes, Is.EqualTo(expectedNumNodes));
                Assert.That(mesh2D.num_edges, Is.EqualTo(expectedNumEdges));
                Assert.That(mesh2D.num_faces, Is.EqualTo(expectedNumFaces));
                Assert.That(mesh2D.node_x.CopyToArray<double>(mesh2D.num_nodes), Is.EqualTo(expectedNodeX));
                Assert.That(mesh2D.node_y.CopyToArray<double>(mesh2D.num_nodes), Is.EqualTo(expectedNodeY));
                Assert.That(mesh2D.edge_x.CopyToArray<double>(mesh2D.num_edges), Is.EqualTo(expectedEdgeX));
                Assert.That(mesh2D.edge_y.CopyToArray<double>(mesh2D.num_edges), Is.EqualTo(expectedEdgeY));
                Assert.That(mesh2D.edge_nodes.CopyToArray<int>(mesh2D.num_edges * 2), Is.EqualTo(expectedEdgeNodes));
                Assert.That(mesh2D.face_x.CopyToArray<double>(mesh2D.num_faces), Is.EqualTo(expectedFaceX));
                Assert.That(mesh2D.face_y.CopyToArray<double>(mesh2D.num_faces), Is.EqualTo(expectedFaceY));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        [Order(3)]
        public void WritingMesh2DWGS84_ShouldWriteFile()
        {
            string filePath = Path.Combine(testOutputDir, "mesh2DWGS84.nc");

            UGridWriter file = null;

            DisposableMesh2D disposableMesh2D = CreateDisposableMesh2D("mesh2DWGS84");
            ProjectedCoordinateSystem projectedCoordinateSystem = CreateGeographicCoordinateSystem();

            try
            {
                Assert.DoesNotThrow(() => file = new UGridWriter(filePath));
                Assert.DoesNotThrow(() => file.AddMesh2D(disposableMesh2D));
                Assert.DoesNotThrow(() => file.AddProjectedCoordinateSystem(projectedCoordinateSystem));
                Assert.DoesNotThrow(() => file.AddGlobalAttribute("source", "Unit test"));
                Assert.DoesNotThrow(() => file.AddGlobalAttributes(globalAttributes));
                Assert.DoesNotThrow(() => file.WriteTopologies());
                Assert.That(file.HasMesh2D, Is.True);
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        [Order(4)]
        public void ImportingExportedMesh2DWGS84_GetsCorrectEPSGCode()
        {
            string filePath = Path.Combine(testOutputDir, "mesh2DWGS84.nc");
            UGridReader file = null;
            try
            {
                file = new UGridReader(filePath);

                Assert.That(file.HasMesh2D, Is.True);
                Assert.That(file.GetEPSGCode(), Is.EqualTo(4326));
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
            }
            finally
            {
                file?.Dispose();
            }
        }
        
        private static DisposableMesh2D CreateDisposableMesh2D(string meshName)
        {
            return new DisposableMesh2D
            {
                Name = meshName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NumNodes = 4,
                NumEdges = 4,
                NumFaces = 1,
                NumFaceNodesMax = 4,

                NodeX = new[] { 0.0, 1.0, 1.0, 0.0 },
                NodeY = new[] { 0.0, 0.0, 1.0, 1.0 },
                NodeZ = new[] { 0.0, 0.0, 0.0, 0.0 },

                EdgeX = new[] { 0.5, 1.0, 0.5, 0.0 },
                EdgeY = new[] { 0.0, 0.5, 1.0, 0.5 },
                EdgeZ = null,
                EdgeNodes = new[] { 0, 1, 1, 2, 2, 3, 3, 0 },
                EdgeFaces = null,

                FaceX = new[] { 0.5 },
                FaceY = new[] { 0.5 },
                FaceZ = null,
                FaceNodes = new[] { 0, 1, 2, 3 },
                FaceEdges = new[] { 0, 0, 0, 0 },
                FaceFaces = null,
            };
        }

        private static DisposableMesh1D CreateDisposableMesh1D(string meshName, string networkName)
        {
            return new DisposableMesh1D
            {
                Name = meshName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NetworkName = networkName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NumNodes = 4,
                NumEdges = 4,

                NodeX = new[] { 0.0, 1.0, 1.0, 0.0 },
                NodeY = new[] { 0.0, 0.0, 1.0, 1.0 },
                NodeEdgeID = new[] { 0, 0, 0, 0 },
                NodeEdgeOffset = new[] { 0.0, 0.0, 0.0, 0.0 },

                EdgeX = new[] { 0.5, 1.0, 0.5, 0.0 },
                EdgeY = new[] { 0.0, 0.5, 1.0, 0.5 },
                EdgeNodes = new[] { 0, 1, 1, 2, 2, 3, 3, 0 }
            };
        }

        private static ProjectedCoordinateSystem CreateCoordinateSystem()
        {
            string wktStr =
                "POINT (30 10)\n" + 
                "LINESTRING (30 10, 10 30, 40 40)\n" + 
                "POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))\n" + 
                "MULTIPOINT ((10 40), (40 30), (20 20), (30 10))\n" + 
                "GEOMETRYCOLLECTION (\n" + 
                "    POINT (10 40),\n" + 
                "    LINESTRING (30 10, 10 30, 40 40),\n" + 
                "    POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))\n" + ')';

            return new ProjectedCoordinateSystem(
                0,
                0.0,
                6378137.0,
                6356752.314245,
                298.257223563,
                "proj_coord_sys_name",
                "gridMappingName",
                "proj4Params",
                "EPSG:0",
                "",
                wktStr);
        }

        private static ProjectedCoordinateSystem CreateGeographicCoordinateSystem()
        {
            return new ProjectedCoordinateSystem(
                4326,
                0.0,
                6378137.0,
                6356752.314245,
                298.257223563,
                "WGS84",
                "latitude_longitude",
                "proj4Params",
                "EPSG:4326",
                "",
                "");
        }
    }
}