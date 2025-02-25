using System.IO;
using NUnit.Framework;

namespace UGridNET.Tests
{

    public class UGridWriterTests
    {
        private static readonly string TestPath = "C:\\Users\\sayed\\work\\tmp\\ugrid_export_tests";

        [Test]
        public void OpenAndCloseNewFileSucceeds()
        {
            var filePath = Path.Combine(TestPath, "test.nc");
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