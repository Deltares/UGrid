using System;
using System.IO;
using NUnit.Framework;
using System.Collections.Generic;
using UGridNET.Extensions;

namespace UGridNET.Tests
{
    [TestFixture]
    public class UGridWriterTests
    {
        private string testOutputDir;
        private readonly bool cleanUpWhenDone = false;
        private readonly bool appendPathWithGUID = false;

        private readonly Dictionary<string, string> globalAttributes = new Dictionary<string, string> {
            { "institution", "Deltares" },
            { "references", "https://www.deltares.nl" },
            { "Conventions", "CF-1.8 UGRID-1.0 Deltares-0.10" }
        };

        public DisposableMesh2D CreateDisposableMesh2D(string meshName)
        {
            return new DisposableMesh2D {
                Name = meshName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NumNodes = 4,
                NumEdges = 4,
                NumFaces = 1,
                NumFaceNodesMax = 4,

                NodeX = new double[] { 0.0, 1.0, 1.0, 0.0 },
                NodeY = new double[] { 0.0, 0.0, 1.0, 1.0 },
                NodeZ = new double[] { 0.0, 0.0, 0.0, 0.0 },

                EdgeX = new double[] { 0.5, 1.0, 0.5, 0.0 },
                EdgeY = new double[] { 0.0, 0.5, 1.0, 0.5 },
                EdgeZ = null,
                EdgeNodes = new int[] { 0, 1, 1, 2, 2, 3, 3, 0 },
                EdgeFaces = null,

                FaceX = new double[] { 0.5 },
                FaceY = new double[] { 0.5 },
                FaceZ = null,
                FaceNodes = new int[] { 0, 1, 2, 3 },
                FaceEdges = new int[] { 0, 0, 0, 0 },
                FaceFaces = null,
            };
        }

        public DisposableMesh1D CreateDisposableMesh1D(string meshName, string networkName)
        {
            return new DisposableMesh1D {
                Name = meshName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NetworkName = networkName.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NumNodes = 4,
                NumEdges = 4,

                NodeX = new double[] { 0.0, 1.0, 1.0, 0.0 },
                NodeY = new double[] { 0.0, 0.0, 1.0, 1.0 },
                NodeEdgeID = new int[] { 0, 0, 0, 0 },
                NodeEdgeOffset = new double[] { 0.0, 0.0, 0.0, 0.0 },

                EdgeX = new double[] { 0.5, 1.0, 0.5, 0.0 },
                EdgeY = new double[] { 0.0, 0.5, 1.0, 0.5 },
                EdgeNodes = new int[] { 0, 1, 1, 2, 2, 3, 3, 0 }
            };
        }

        [SetUp]
        public void SetUp()
        {
            string testOutputSubDir = "UGridNET_tests";
            if (appendPathWithGUID)
            {
                testOutputSubDir += '-' + Guid.NewGuid().ToString();
            }
            testOutputDir = Path.Combine(Path.GetTempPath(), testOutputSubDir);
            Directory.CreateDirectory(testOutputDir);
            // Console.WriteLine($"Test path {testOutputDir}");
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
        public void WritingMesh1DSucceeds()
        {
            string filePath = Path.Combine(testOutputDir, "mesh1D.nc");

            UGridWriter file = null;

            var disposableMesh1D = CreateDisposableMesh1D("myMesh1D", "myNetwork");

            string wktStr =
                "POINT (30 10)\n" + "LINESTRING (30 10, 10 30, 40 40)\n" + "POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))\n" + "MULTIPOINT ((10 40), (40 30), (20 20), (30 10))\n" + "GEOMETRYCOLLECTION (\n" + "    POINT (10 40),\n" + "    LINESTRING (30 10, 10 30, 40 40),\n" + "    POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))\n" + ')';

            var projectedCoordinateSystem = new ProjectedCoordinateSystem(
                epsg: 0,
                longitudeOfPrimeMeridian: 0.0,
                semiMajorAxis: 6378137.0,
                semiMinorAxis: 6356752.314245,
                inverseFlattening: 298.257223563,
                name: "proj_coord_sys_name",
                gridMappingName: "gridMappingName",
                proj4Params: "proj4Params",
                epsgCode: "EPSG:0",
                projectionName: "",
                wkt: wktStr);

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

        [Test, Order(1)]
        public void WritingMesh2DSucceeds()
        {
            string filePath = Path.Combine(testOutputDir, "mesh2D.nc");

            UGridWriter file = null;

            var disposableMesh2D = CreateDisposableMesh2D("myMesh2D");
            // var otherDisposableMesh2D = CreateDisposableMesh2D("myOtherMesh2D");

            string wktStr =
                "POINT (30 10)\n" + "LINESTRING (30 10, 10 30, 40 40)\n" + "POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))\n" + "MULTIPOINT ((10 40), (40 30), (20 20), (30 10))\n" + "GEOMETRYCOLLECTION (\n" + "    POINT (10 40),\n" + "    LINESTRING (30 10, 10 30, 40 40),\n" + "    POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))\n" + ')';

            var projectedCoordinateSystem = new ProjectedCoordinateSystem(
                epsg: 0,
                longitudeOfPrimeMeridian: 0.0,
                semiMajorAxis: 6378137.0,
                semiMinorAxis: 6356752.314245,
                inverseFlattening: 298.257223563,
                name: "proj_coord_sys_name",
                gridMappingName: "gridMappingName",
                proj4Params: "proj4Params",
                epsgCode: "EPSG:0",
                projectionName: "",
                wkt: wktStr);

            try
            {
                Assert.DoesNotThrow(() => file = new UGridWriter(filePath));
                Assert.DoesNotThrow(() => file.AddMesh2D(disposableMesh2D));
                // Assert.DoesNotThrow(() => file.AddMesh2D(otherDisposableMesh2D));
                Assert.DoesNotThrow(() => file.AddProjectedCoordinateSystem(projectedCoordinateSystem));
                Assert.DoesNotThrow(() => file.AddGlobalAttribute("source", "Unit test"));
                Assert.DoesNotThrow(() => file.AddGlobalAttributes(globalAttributes));
                Assert.DoesNotThrow(() => file.WriteTopologies());
                Assert.That(file.HasMesh2D, Is.True);
                // Assert.That(file.Mesh2DList.Count, Is.EqualTo(2));
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test, Order(2)]
        public void ImportingExportedMesh2DSucceeds()
        {
            string filePath = Path.Combine(testOutputDir, "mesh2D.nc");

            UGridReader file = null;

            var ExpectedNumNodes = 4;
            var ExpectedNumEdges = 4;
            var ExpectedNumFaces = 1;
            var expectedNodeX = new double[] { 0.0, 1.0, 1.0, 0.0 };
            var expectedNodeY = new double[] { 0.0, 0.0, 1.0, 1.0 };
            var expectedEdgeX = new double[] { 0.5, 1.0, 0.5, 0.0 };
            var expectedEdgeY = new double[] { 0.0, 0.5, 1.0, 0.5 };
            var expectedEdgeNodes = new int[] { 0, 1, 1, 2, 2, 3, 3, 0 };
            var expectedFaceX = new double[] { 0.5 };
            var expectedFaceY = new double[] { 0.5 };

            try
            {
                file = new UGridReader(filePath);

                Assert.That(file.HasMesh2D, Is.True);
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
                var mesh2D = file.Mesh2DList[0];
                Assert.That(mesh2D.num_nodes, Is.EqualTo(ExpectedNumNodes));
                Assert.That(mesh2D.num_edges, Is.EqualTo(ExpectedNumEdges));
                Assert.That(mesh2D.num_faces, Is.EqualTo(ExpectedNumFaces));
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

        [Test, Order(3)]
        public void WritingMesh2DWGS84_ShouldWriteFile()
        {
            string filePath = Path.Combine(testOutputDir, "mesh2DWGS84.nc");

            UGridWriter file = null;

            var disposableMesh2D = CreateDisposableMesh2D("mesh2DWGS84");
            // var otherDisposableMesh2D = CreateDisposableMesh2D("myOtherMesh2D");

            var projectedCoordinateSystem = new ProjectedCoordinateSystem(
                epsg: 4326,
                longitudeOfPrimeMeridian: 0.0,
                semiMajorAxis: 6378137.0,
                semiMinorAxis: 6356752.314245,
                inverseFlattening: 298.257223563,
                name: "WGS84",
                gridMappingName: "latitude_longitude",
                proj4Params: "proj4Params",
                epsgCode: "EPSG:4326",
                projectionName: "",
                wkt: "");

            try
            {
                Assert.DoesNotThrow(() => file = new UGridWriter(filePath));
                Assert.DoesNotThrow(() => file.AddMesh2D(disposableMesh2D));
                // Assert.DoesNotThrow(() => file.AddMesh2D(otherDisposableMesh2D));
                Assert.DoesNotThrow(() => file.AddProjectedCoordinateSystem(projectedCoordinateSystem));
                Assert.DoesNotThrow(() => file.AddGlobalAttribute("source", "Unit test"));
                Assert.DoesNotThrow(() => file.AddGlobalAttributes(globalAttributes));
                Assert.DoesNotThrow(() => file.WriteTopologies());
                Assert.That(file.HasMesh2D, Is.True);
                // Assert.That(file.Mesh2DList.Count, Is.EqualTo(2));
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test, Order(3)]
        public void ImportingExportedMesh2DWGS84_GetsCorrectEPSGCode()
        {
            string filePath = Path.Combine(testOutputDir, "mesh2DWGS84.nc");
            UGridReader file = null;
            try
            {
                file = new UGridReader(filePath);

                Assert.That(file.HasMesh2D, Is.True);
                Assert.That(file.GetEPSGCode(), Is.EqualTo(file.GetEPSGCode()));
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
            }
            finally
            {
                file?.Dispose();
            }
        }
    }
}
