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
            Console.WriteLine($"Test path {testOutputDir}");
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
            
            // var disposableMesh2D = new DisposableMesh2D
            // {
            //     NumNodes = 4,
            //     NumEdges = 4,
            //     NumFaces = 1,
            //     NodeX = { 0.0, 1.0, 1.0, 0.0 },
            //     NodeY = { 0.0, 0.0, 1.0, 1.0 },
            //     NodeZ = null,               
            //     EdgeX = { 0.5, 1.0, 0.5, 0.0 },
            //     EdgeY = { 0.0, 0.5, 1.0, 0.5 },
            //     EdgeZ = null,
            //     EdgeNodes = { 0, 1, 1, 2, 2, 3, 3, 0 },
            //     EdgeFaces = null,
            //     FaceX = { 0.65 },
            //     FaceY = { 0.55 },
            //     FaceZ = null,
            //     FaceNodes = { 0, 1, 2, 3 },
            //     FaceEdges = null,
            //     FaceFaces = null
            // };

            var disposableMesh2D = new DisposableMesh2D();
            string name = "myOtherMesh2D";
            disposableMesh2D.Name = name.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
            disposableMesh2D.NumNodes = 4;
            disposableMesh2D.NumEdges = 4;
            disposableMesh2D.NumFaces = 1;
            disposableMesh2D.NumFaceNodesMax = 4;
            disposableMesh2D.NodeX = new double[]{ 0.0, 1.0, 1.0, 0.0 };
            disposableMesh2D.NodeY = new double[]{ 0.0, 0.0, 1.0, 1.0 };
            disposableMesh2D.NodeZ = new double[]{ 0.0, 0.0, 0.0, 0.0 };
            disposableMesh2D.EdgeX = new double[]{ 0.5, 1.0, 0.5, 0.0 };
            disposableMesh2D.EdgeY = new double[]{ 0.0, 0.5, 1.0, 0.5 };
            disposableMesh2D.EdgeZ = null;
            disposableMesh2D.EdgeNodes = new int[]{ 0, 1, 1, 2, 2, 3, 3, 0 };
            disposableMesh2D.EdgeFaces = null;
            disposableMesh2D.FaceX = new double[]{ 0.5 };
            disposableMesh2D.FaceY = new double[]{ 0.5 };
            disposableMesh2D.FaceZ = new double[]{ 0.0 }; //null;
            disposableMesh2D.FaceNodes = new int[]{ 0, 1, 2, 3 };
            disposableMesh2D.FaceEdges = new int[]{ 0, 0, 0, 0 };
            disposableMesh2D.FaceFaces = new int[]{ 0, 0, 0, 0 }; //null;

            // const int numNodes = 4;
            // const int numEdges = 4;
            // const int numFaces = 1;

            // var disposableMesh2D = new DisposableMesh2D(numNodes, numEdges, numFaces);
            // string name = "myOtherMesh2D";
            // disposableMesh2D.Name = name.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
            // disposableMesh2D.NodeX = new double[]{ 0.0, 1.0, 1.0, 0.0 };
            // disposableMesh2D.NodeY = new double[]{ 0.0, 0.0, 1.0, 1.0 };
            // disposableMesh2D.NodeZ = new double[]{ 0.0, 0.0, 0.0, 0.0 }; //null;
            // disposableMesh2D.EdgeX = new double[]{ 0.5, 1.0, 0.5, 0.0 };
            // disposableMesh2D.EdgeY = new double[]{ 0.0, 0.5, 1.0, 0.5 };
            // disposableMesh2D.EdgeZ = new double[]{ 0.0, 0.0, 0.0, 0.0 }; //null;
            // disposableMesh2D.EdgeNodes = new int[]{ 0, 1, 1, 2, 2, 3, 3, 0 };
            // disposableMesh2D.EdgeFaces = new int[]{ 0, 0, 0, 0, 0, 0, 0, 0 }; //null;
            // disposableMesh2D.FaceX = new double[]{ 0.5 };
            // disposableMesh2D.FaceY = new double[]{ 0.5 };
            // disposableMesh2D.FaceZ = new double[]{ 0.0, 0.0, 0.0, 0.0 }; //null;
            // disposableMesh2D.FaceNodes = new int[]{ 0, 1, 2, 3 };
            // disposableMesh2D.FaceEdges = new int[]{ 0, 0, 0, 0 }; //null;
            // disposableMesh2D.FaceFaces = new int[]{ 0, 0, 0, 0 }; //null;

            
            var projectedCoordinateSystem = new ProjectedCoordinateSystem(
                epsg: 0,
                longitudeOfPrimeMeridian: 0.0,
                semiMajorAxis: 6378137.0,
                semiMinorAxis: 6356752.314245,
                inverseFlattening: 298.257223563,
                name,
                gridMappingName: "gridMappingName",
                proj4Params: "proj4Params",
                epsgCode: "EPSG:0",
                projectionName: "",
                wkt: "wkt"
            );

            try
            {
                Assert.DoesNotThrow(() => file = new UGridWriter(filePath));
                Assert.DoesNotThrow(() => file.AddMesh2D(disposableMesh2D));
                Assert.DoesNotThrow(() => file.AddProjectedCoordinateSystem(projectedCoordinateSystem));
                Assert.DoesNotThrow(() => file.Write());
                Assert.That(file.HasMesh1D, Is.False);
                Assert.That(file.HasMesh2D, Is.True);
                Assert.That(file.HasContacts, Is.False);
                Assert.That(file.HasNetwork1D, Is.False);
                Assert.That(file.Mesh2DList.Count, Is.EqualTo(1));
            }
            finally
            {
                file?.Dispose();
            }
        }
    }
}