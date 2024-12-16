using NUnit.Framework;
using System;
using UGridNET.Extensions;

namespace UGridNET.Tests
{
    public class TopologyExtensionsTests
    {
        // Test case source for different classes
        private static readonly object[] TestCases =
        {
            new TestCaseData(new Mesh1D { num_nodes = 10, num_edges = 5 }),
            new TestCaseData(new Mesh2D { num_nodes = 10, num_edges = 5, num_faces = 6, num_face_nodes_max = 7 }),
            new TestCaseData(new Contacts { num_contacts = 5 }),
            new TestCaseData(new Network1D { num_nodes = 10, num_edges = 5, num_geometry_nodes = 20 })
        };

        [TestCaseSource(nameof(TestCases))]
        public void TopologyAllocateAndFreeExtensionsDoNotThrow(object instance)
        {
            Assert.DoesNotThrow(() =>
            {
                switch (instance)
                {
                    case Mesh1D mesh1D:
                        mesh1D.Allocate();
                        mesh1D.Free();
                        break;
                    case Mesh2D mesh2D:
                        mesh2D.Allocate();
                        mesh2D.Free();
                        break;
                    case Contacts contacts:
                        contacts.Allocate();
                        contacts.Free();
                        break;
                    case Network1D network1D:
                        network1D.Allocate();
                        network1D.Free();
                        break;
                }
            });
        }
    }

}