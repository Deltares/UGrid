using System;
using System.Collections.Generic;
using System.IO;
using NUnit.Framework;
using NUnit.Framework.Interfaces;
using UGridNET.Extensions;

namespace UGridNET.Tests
{

    public class UGridReaderTests
    {
        private static readonly string TestDataPath = TestUtilities.GetTestDataPath();

        [Test]
        public void OpenExistingFileSucceeds()
        {
            var filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                Assert.DoesNotThrow(() => file = new UGridReader(filePath));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void OpenNonExistingFileFails()
        {
            var filePath = "nonExistentFile.nc";
            UGridReader file = null;

            try
            {
                Assert.Throws<UGridNETException>(() => file = new UGridReader(filePath));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetPath()
        {
            var filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Assert.That(file.GetPath(), Is.EqualTo(Path.GetFullPath(filePath)));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void QueryTopologiesInAFileThatContainsAllTopologies()
        {
            var filePath = Path.Combine(TestDataPath, "AllUGridEntities.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Assert.That(file.HasMesh1D(), Is.True);
                Assert.That(file.HasMesh2D(), Is.True);
                Assert.That(file.HasContacts(), Is.True);
                Assert.That(file.HasNetwork1D(), Is.True);
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void QueryTopologiesInAFileThatContainsOnlyMEsh2D()
        {
            var filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Assert.That(file.HasMesh1D(), Is.False);
                Assert.That(file.HasMesh2D(), Is.True);
                Assert.That(file.HasContacts(), Is.False);
                Assert.That(file.HasNetwork1D(), Is.False);
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetEPSGCode()
        {
            var filePath = Path.Combine(TestDataPath, "ResultFile.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                int EPSGCode = file.GetEPSGCode();
                Assert.That(EPSGCode, Is.EqualTo(0));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetEPSGCodeFromAVarWithValuelessAttribute()
        {
            var filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                var EPSGCode = file.GetEPSGCode();
                Assert.That(EPSGCode, Is.EqualTo(0));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetEPSGCodeThrowsDueToMissingProjectedCoordinateSystemVariable()
        {
            var filePath = Path.Combine(TestDataPath, "AllUGridEntities.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                var EPSGCode = -1;
                Assert.Throws<UGridNETException>(() => EPSGCode = file.GetEPSGCode());
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetConventions()
        {
            var filePath = Path.Combine(TestDataPath, "ResultFile.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                var conventions = file.GetConventions();
                Assert.That(conventions, Is.EqualTo("CF-1.6 UGRID-1.0/Deltares-0.8"));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetMesh2DAttributesByIndex()
        {
            var filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                var attributes = file.GetEntityAttributesByIndex(TopologyType.Mesh2dTopology, 0);
                Assert.That(attributes["cf_role"], Is.EqualTo("mesh_topology"));
                Assert.That(attributes["long_name"], Is.EqualTo("Topology data of 2D mesh"));
                Assert.That(attributes["topology_dimension"], Is.EqualTo("2"));
                Assert.That(attributes["node_coordinates"], Is.EqualTo("mesh2d_node_x mesh2d_node_y"));
                Assert.That(attributes["node_dimension"], Is.EqualTo("mesh2d_nNodes"));
                Assert.That(attributes["max_face_nodes_dimension"], Is.EqualTo("mesh2d_nMax_face_nodes"));
                Assert.That(attributes["edge_node_connectivity"], Is.EqualTo("mesh2d_edge_nodes"));
                Assert.That(attributes["edge_dimension"], Is.EqualTo("mesh2d_nEdges"));
                Assert.That(attributes["edge_coordinates"], Is.EqualTo("mesh2d_edge_x mesh2d_edge_y"));
                Assert.That(attributes["face_node_connectivity"], Is.EqualTo("mesh2d_face_nodes"));
                Assert.That(attributes["face_dimension"], Is.EqualTo("mesh2d_nFaces"));
                Assert.That(attributes["face_coordinates"], Is.EqualTo("mesh2d_face_x mesh2d_face_y"));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetMesh2DAttributesByIndexThrows()
        {
            var filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                var attributes = new Dictionary<string, string>();
                Assert.Throws<ArgumentOutOfRangeException>(() => attributes = file.GetEntityAttributesByIndex(TopologyType.Mesh2dTopology, 1));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetMesh2DAttributesByName()
        {
            var filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                var attributes = file.GetEntityAttributesByName(TopologyType.Mesh2dTopology, "mesh2d");
                Assert.That(attributes["cf_role"], Is.EqualTo("mesh_topology"));
                Assert.That(attributes["long_name"], Is.EqualTo("Topology data of 2D mesh"));
                Assert.That(attributes["topology_dimension"], Is.EqualTo("2"));
                Assert.That(attributes["node_coordinates"], Is.EqualTo("mesh2d_node_x mesh2d_node_y"));
                Assert.That(attributes["node_dimension"], Is.EqualTo("mesh2d_nNodes"));
                Assert.That(attributes["max_face_nodes_dimension"], Is.EqualTo("mesh2d_nMax_face_nodes"));
                Assert.That(attributes["edge_node_connectivity"], Is.EqualTo("mesh2d_edge_nodes"));
                Assert.That(attributes["edge_dimension"], Is.EqualTo("mesh2d_nEdges"));
                Assert.That(attributes["edge_coordinates"], Is.EqualTo("mesh2d_edge_x mesh2d_edge_y"));
                Assert.That(attributes["face_node_connectivity"], Is.EqualTo("mesh2d_face_nodes"));
                Assert.That(attributes["face_dimension"], Is.EqualTo("mesh2d_nFaces"));
                Assert.That(attributes["face_coordinates"], Is.EqualTo("mesh2d_face_x mesh2d_face_y"));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetMesh2DAttributesByNameThrows()
        {
            var filePath = Path.Combine(TestDataPath, "OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                var attributes = new Dictionary<string, string>();
                Assert.Throws<InvalidOperationException>(() => attributes = file.GetEntityAttributesByName(TopologyType.Mesh2dTopology, "brrrrrrr"));
            }
            finally
            {
                file?.Dispose();
            }
        }

    }
}