using System;
using System.Collections.Generic;
using System.IO;
using NUnit.Framework;

namespace UGridNET.Tests
{
    public class UGridReaderTests
    {
        [Test]
        public void OpenExistingFileSucceeds()
        {
            string filePath = TestHelper.GetTestFilePath("OneMesh2D.nc");
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
        public void OpenEmptyFileFails()
        {
            string filePath = TestHelper.GetTestFilePath("EmptyFile.nc");
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
            string filePath = TestHelper.GetTestFilePath("OneMesh2D.nc");
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
            string filePath = TestHelper.GetTestFilePath("AllUGridEntities.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Assert.That(file.HasMesh1D, Is.True);
                Assert.That(file.HasMesh2D, Is.True);
                Assert.That(file.HasContacts, Is.True);
                Assert.That(file.HasNetwork1D, Is.True);
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void QueryTopologiesInAFileThatContainsOnlyMEsh2D()
        {
            string filePath = TestHelper.GetTestFilePath("OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Assert.That(file.HasMesh1D, Is.False);
                Assert.That(file.HasMesh2D, Is.True);
                Assert.That(file.HasContacts, Is.False);
                Assert.That(file.HasNetwork1D, Is.False);
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetEPSGCode()
        {
            string filePath = TestHelper.GetTestFilePath("ResultFile.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                int epsgCode = file.GetEPSGCode();
                Assert.That(epsgCode, Is.EqualTo(0));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetEPSGCode_28992()
        {
            string filePath = TestHelper.GetTestFilePath("1D2D_net.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                int epsgCode = file.GetEPSGCode();
                Assert.That(epsgCode, Is.EqualTo(28992));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetEPSGCodeFromAVarWithValuelessAttribute()
        {
            string filePath = TestHelper.GetTestFilePath("OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                int epsgCode = file.GetEPSGCode();
                Assert.That(epsgCode, Is.EqualTo(0));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetEPSGCodeThrowsDueToMissingProjectedCoordinateSystemVariable()
        {
            string filePath = TestHelper.GetTestFilePath("AllUGridEntities.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Assert.Throws<UGridNETException>(() => file.GetEPSGCode());
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetConventions()
        {
            string filePath = TestHelper.GetTestFilePath("ResultFile.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                string conventions = file.GetConventions();
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
            string filePath = TestHelper.GetTestFilePath("OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Dictionary<string, string> attributes = file.GetEntityAttributesByIndex(TopologyType.Mesh2dTopology, 0);
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
            string filePath = TestHelper.GetTestFilePath("OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Assert.Throws<ArgumentOutOfRangeException>(() => file.GetEntityAttributesByIndex(TopologyType.Mesh2dTopology, 1));
            }
            finally
            {
                file?.Dispose();
            }
        }

        [Test]
        public void GetMesh2DAttributesByName()
        {
            string filePath = TestHelper.GetTestFilePath("OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Dictionary<string, string> attributes = file.GetEntityAttributesByName(TopologyType.Mesh2dTopology, "mesh2d");
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
            string filePath = TestHelper.GetTestFilePath("OneMesh2D.nc");
            UGridReader file = null;

            try
            {
                file = new UGridReader(filePath);
                Assert.Throws<InvalidOperationException>(() => file.GetEntityAttributesByName(TopologyType.Mesh2dTopology, "brrrrrrr"));
            }
            finally
            {
                file?.Dispose();
            }
        }
    }
}