using System;
using System.IO;
using NUnit.Framework;
using NUnit.Framework.Internal;
using System.Collections.Generic;
using System.Linq;
using UGridNET.Extensions;


namespace UGridNET.Tests
{
    [TestFixture]
    public class UGridWriterTests
    {
        private string testOutputDir;
        private readonly bool cleanUpWhenDone = false;
        private readonly bool appendPathWithGUID = false;

        [SetUp]
        public void SetUp()
        {
            string testOutputSubDir = "UGridNET_tests";
            if(appendPathWithGUID) {
                testOutputSubDir += '-' + Guid.NewGuid().ToString();
            }
            testOutputDir = Path.Combine(Path.GetTempPath(), testOutputSubDir);
            Directory.CreateDirectory(testOutputDir);
            //Console.WriteLine($"Test path {testOutputDir}");
        }

        [TearDown]
        public void TearDown()
        {
            if(cleanUpWhenDone) {
                Directory.Delete(testOutputDir, true);
            }
        }

        [Test]
        public void WritingNativeObjectSucceeds()
        {
            string filePath = Path.Combine(testOutputDir, "test.nc");

            UGridWriter file = null;

            var disposableMesh1D = new DisposableMesh1D
            {
                Name = "myMesh1D".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NetworkName = "myNetwork".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NumNodes = 4,
                NumEdges = 4,
                NodeX = new double[]{ 0.0, 1.0, 1.0, 0.0 },
                NodeY = new double[]{ 0.0, 0.0, 1.0, 1.0 },
                NodeEdgeID = new int[]{ 0, 0, 0, 0 },
                NodeEdgeOffset = new double[]{ 0.0, 0.0, 0.0, 0.0 },
                EdgeX = new double[]{ 0.5, 1.0, 0.5, 0.0 },
                EdgeY = new double[]{ 0.0, 0.5, 1.0, 0.5 },
                EdgeNodes = new int[]{ 0, 1, 1, 2, 2, 3, 3, 0 }
            };

            var disposableMesh2D = new DisposableMesh2D
            {
                Name = "myMesh2D".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NumNodes = 4,
                NumEdges = 4,
                NumFaces = 1,
                NumFaceNodesMax = 4,
                NodeX = new double[]{ 0.0, 1.0, 1.0, 0.0 },
                NodeY = new double[]{ 0.0, 0.0, 1.0, 1.0 },
                NodeZ = new double[]{ 0.0, 0.0, 0.0, 0.0 },
                EdgeX = new double[]{ 0.5, 1.0, 0.5, 0.0 },
                EdgeY = new double[]{ 0.0, 0.5, 1.0, 0.5 },
                EdgeZ = null,
                EdgeNodes = new int[]{ 0, 1, 1, 2, 2, 3, 3, 0 },
                EdgeFaces = null,
                FaceX = new double[]{ 0.5 },
                FaceY = new double[]{ 0.5 },
                FaceZ = new double[]{ 0.0 }, //null,
                FaceNodes = new int[]{ 0, 1, 2, 3 },
                FaceEdges = new int[]{ 0, 0, 0, 0 },
                FaceFaces = new int[]{ 0, 0, 0, 0 } //null
            };

            var otherDisposableMesh2D = new DisposableMesh2D
            {
                Name = "myOtherMesh2D".GetRightPaddedNullTerminatedBytes(UGrid.name_long_length),
                NumNodes = 4,
                NumEdges = 4,
                NumFaces = 1,
                NumFaceNodesMax = 4,
                NodeX = new double[]{ 0.0, 1.0, 1.0, 0.0 },
                NodeY = new double[]{ 0.0, 0.0, 1.0, 1.0 },
                NodeZ = new double[]{ 0.0, 0.0, 0.0, 0.0 },
                EdgeX = new double[]{ 0.5, 1.0, 0.5, 0.0 },
                EdgeY = new double[]{ 0.0, 0.5, 1.0, 0.5 },
                EdgeZ = null,
                EdgeNodes = new int[]{ 0, 1, 1, 2, 2, 3, 3, 0 },
                EdgeFaces = null,
                FaceX = new double[]{ 0.5 },
                FaceY = new double[]{ 0.5 },
                FaceZ = new double[]{ 0.0 }, //null,
                FaceNodes = new int[]{ 0, 1, 2, 3 },
                FaceEdges = new int[]{ 0, 0, 0, 0 },
                FaceFaces = new int[]{ 0, 0, 0, 0 } //null
            };

            string wktStr = 
                "POINT (30 10)\n"
                + "LINESTRING (30 10, 10 30, 40 40)\n"
                + "POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))\n"
                + "MULTIPOINT ((10 40), (40 30), (20 20), (30 10))\n"
                + "GEOMETRYCOLLECTION (\n"
                + "    POINT (10 40),\n"
                + "    LINESTRING (30 10, 10 30, 40 40),\n"
                + "    POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))\n"
                + ')';

            var projectedCoordinateSystem = new ProjectedCoordinateSystem
            (
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
                wkt: wktStr
            );

            try
            {
                Assert.DoesNotThrow(() => file = new UGridWriter(filePath));
                //Assert.DoesNotThrow(() => file.AddMesh1D(disposableMesh1D));
                Assert.DoesNotThrow(() => file.AddMesh2D(disposableMesh2D));
                //Assert.DoesNotThrow(() => file.AddMesh2D(otherDisposableMesh2D));
                Assert.DoesNotThrow(() => file.AddProjectedCoordinateSystem(projectedCoordinateSystem));
                Assert.DoesNotThrow(() => file.AddGlobalAttribute("Institution", "Deltares"));
                var otherGlobalAttributes = new Dictionary<string, string> 
                {
                    {"references", "https://www.deltares.nl" ,
                    {"Conventions", "CF-1.8 UGRID-1.0 Deltares-0.10"}
                };
                Assert.DoesNotThrow(() => file.AddGlobalAttributes(otherGlobalAttributes));
                Assert.DoesNotThrow(() => file.WriteTopologies());
                //Assert.That(file.HasMesh1D, Is.True);
                Assert.That(file.HasMesh2D, Is.True);
                //Assert.That(file.Mesh1DList.Count, Is.EqualTo(1));
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
            }
            finally
            {
                file?.Dispose();
            }
        }
    }
}