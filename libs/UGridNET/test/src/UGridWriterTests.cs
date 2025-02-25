using System;
using System.IO;
using NUnit.Framework;
using NUnit.Framework.Internal;

namespace UGridNET.Tests
{

    [TestFixture]
    public class UGridWriterTests
    {
        private string testOutputDir;
        private readonly bool cleanUpWhenDone = false;

        [SetUp]
        public void SetUp()
        {
            testOutputDir = Path.Combine(Path.GetTempPath(), "UGridNET_tests-" + Guid.NewGuid().ToString());
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
        public void WritingSucceeds()
        {
            var filePath = Path.Combine(testOutputDir, "test.nc");
            string name = "myMesh2D";
            UGridWriter file = null;
            int topologyID = -1;
            double[] nodesX = { 1.0, 2.0, 1.0, 2.0, 1.0 };
            var projectedCoordinateSystem = new ProjectedCoordinateSystem(
                0,
                0.0,
                6378137.0,
                6356752.314245,
                298.257223563,
                name,
                "gridMappingName",
                "proj4Params",
                "EPSG:0",
                "",
                "wkt"
            );

            try
            {
                Assert.DoesNotThrow(() => file = new UGridWriter(filePath));
                Assert.DoesNotThrow(() => topologyID = file.DefineMesh2D(name, 5, 10, 15));
                Assert.DoesNotThrow(() => file.PopulateMesh2D(topologyID, nodesX));
                Assert.DoesNotThrow(() => file.AddProjectedCoordinateSystem(projectedCoordinateSystem));
                Assert.DoesNotThrow(() => file.WriteMesh2D(topologyID));
            }
            finally
            {
                file?.Dispose();
            }
        }

    }
}