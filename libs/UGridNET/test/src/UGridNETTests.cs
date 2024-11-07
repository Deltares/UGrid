using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using NUnit.Framework;
using UGridNET.Tests.TopologyExtensions;

namespace UGridNET.Tests
{

    internal static class IntPtrExtensions
    {
        public static T[] CopyToArray<T>(this IntPtr pointer, int length)
        {
            if (pointer == IntPtr.Zero)
            {
                throw new InvalidOperationException($"IntPtr {nameof(pointer)} is null. Cannot copy to array.");
            }

            if (length <= 0)
            {
                throw new ArgumentException("Array length must be greater than zero.", nameof(length));
            }

            var array = new T[length];

            if (typeof(T) == typeof(double))
            {
                Marshal.Copy(pointer, array as double[], 0, array.Length);
            }
            else if (typeof(T) == typeof(int))
            {
                Marshal.Copy(pointer, array as int[], 0, array.Length);
            }
            else
            {
                throw new NotSupportedException("Currently only double and int are supported.");
            }

            return array;
        }

        public static void CopyFromArray<T>(this IntPtr pointer, T[] array)
        {
            if (array == null || array.Length == 0)
            {
                throw new ArgumentException("Array must not be null or empty.", nameof(array));
            }

            if (pointer == IntPtr.Zero)
            {
                throw new InvalidOperationException(
                    $"IntPtr {nameof(pointer)} is null. Cannot write array {nameof(array)} to pointer.");
            }

            if (typeof(T) == typeof(int))
            {
                Marshal.Copy(array as int[], 0, pointer, array.Length);
            }
            else if (typeof(T) == typeof(double))
            {
                Marshal.Copy(array as double[], 0, pointer, array.Length);
            }
            else
            {
                throw new NotSupportedException("Currently only int and double are supported");
            }
        }
    }

    namespace TopologyExtensions
    {
        internal static class Mesh1DExtensions
        {

            public static void Allocate(this Mesh1D mesh1D)
            {
                int int_bytes = Marshal.SizeOf<int>();
                int double_bytes = Marshal.SizeOf<double>();
                mesh1D.node_x = Marshal.AllocHGlobal(mesh1D.num_nodes * double_bytes);
                mesh1D.node_y = Marshal.AllocHGlobal(mesh1D.num_nodes * double_bytes);
                mesh1D.edge_x = Marshal.AllocHGlobal(mesh1D.num_edges * double_bytes);
                mesh1D.edge_y = Marshal.AllocHGlobal(mesh1D.num_edges * double_bytes);
                mesh1D.edge_nodes = Marshal.AllocHGlobal(mesh1D.num_edges * 2 * int_bytes);
                mesh1D.edge_edge_id = Marshal.AllocHGlobal(mesh1D.num_nodes * int_bytes); // size?
                mesh1D.node_edge_id = Marshal.AllocHGlobal(mesh1D.num_nodes * int_bytes); // size?
                mesh1D.node_edge_offset = Marshal.AllocHGlobal(mesh1D.num_nodes * double_bytes);
            }

            // This should be done in Dispose. How can i do this without inheritance?
            // Can probably extend Mesh1D in the SWIG config and have it dump the extension directly in the generated class?
            public static void Free(this Mesh1D mesh1D)
            {
                if (mesh1D.node_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.node_x);
                if (mesh1D.node_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.node_y);
                if (mesh1D.edge_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.edge_x);
                if (mesh1D.edge_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.edge_x);
                if (mesh1D.edge_nodes != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.edge_nodes);
                if (mesh1D.edge_edge_id != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.edge_edge_id);
                if (mesh1D.node_edge_id != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.node_edge_id);
                if (mesh1D.node_edge_offset != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.node_edge_offset);
            }
        }

        internal static class Mesh2DExtensions
        {
            public static void Allocate(this Mesh2D mesh2D)
            {
                int int_bytes = Marshal.SizeOf<int>();
                int double_bytes = Marshal.SizeOf<double>();
                mesh2D.node_x = Marshal.AllocHGlobal(mesh2D.num_nodes * double_bytes);
                mesh2D.node_y = Marshal.AllocHGlobal(mesh2D.num_nodes * double_bytes);
                mesh2D.node_z = Marshal.AllocHGlobal(mesh2D.num_nodes * double_bytes);
                mesh2D.edge_x = Marshal.AllocHGlobal(mesh2D.num_edges * double_bytes);
                mesh2D.edge_y = Marshal.AllocHGlobal(mesh2D.num_edges * double_bytes);
                mesh2D.edge_z = Marshal.AllocHGlobal(mesh2D.num_edges * double_bytes);
                mesh2D.face_x = Marshal.AllocHGlobal(mesh2D.num_faces * double_bytes);
                mesh2D.face_y = Marshal.AllocHGlobal(mesh2D.num_faces * double_bytes);
                mesh2D.face_z = Marshal.AllocHGlobal(mesh2D.num_faces * double_bytes);
                mesh2D.edge_nodes = Marshal.AllocHGlobal(mesh2D.num_edges * 2 * int_bytes);
                mesh2D.edge_faces = Marshal.AllocHGlobal(mesh2D.num_edges * 2 * int_bytes);
                mesh2D.face_nodes = Marshal.AllocHGlobal(mesh2D.num_faces * mesh2D.num_face_nodes_max * int_bytes);
                mesh2D.face_edges = Marshal.AllocHGlobal(mesh2D.num_faces * mesh2D.num_face_nodes_max * int_bytes);
                mesh2D.face_faces = Marshal.AllocHGlobal(mesh2D.num_faces * mesh2D.num_face_nodes_max * int_bytes);
            }

            // This should be done in Dispose. How can i do this without inheritance?
            // Can probably extend Mesh2D in the SWIG config and have it dump the extension directly in the generated class?
            public static void Free(this Mesh2D mesh2D)
            {
                if (mesh2D.node_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.node_x);
                if (mesh2D.node_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.node_y);
                if (mesh2D.node_z != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.node_z);
                if (mesh2D.edge_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_x);
                if (mesh2D.edge_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_y);
                if (mesh2D.edge_z != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_z);
                if (mesh2D.face_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_x);
                if (mesh2D.face_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_y);
                if (mesh2D.face_z != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_z);
                if (mesh2D.edge_nodes != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_nodes);
                if (mesh2D.edge_faces != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_faces);
                if (mesh2D.face_nodes != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_nodes);
                if (mesh2D.face_edges != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_edges);
                if (mesh2D.face_faces != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_faces);
            }
        }

        internal static class ContactsExtensions
        {
            public static void Allocate(this Contacts contacts)
            {
                throw new NotImplementedException("TODO: ContactsExtensions::Allocate");
            }

            // This should be done in Dispose. How can i do this without inheritance?
            // Can probably extend Contacts in the SWIG config and have it dump the extension directly in the generated class?
            public static void Free(this Contacts mesh2D)
            {
                throw new NotImplementedException("TODO: ContactsExtensions::Free");
            }
        }

        internal static class Network1DExtensions
        {
            public static void Allocate(this Network1D contacts)
            {
                throw new NotImplementedException("TODO: Network1DExtensions::Allocate");
            }

            // This should be done in Dispose. How can i do this without inheritance?
            // Can probably extend Network1D in the SWIG config and have it dump the extension directly in the generated class?
            public static void Free(this Network1D mesh2D)
            {
                throw new NotImplementedException("TODO: Network1DExtensions::Free");
            }
        }
    }

    public sealed class UGridWrapper : IDisposable
    {
        // requires using log4net;
        //private static readonly ILog log = LogManager.GetLogger(typeof(UGridWrapper));


        private int fileId = -1;
        private int exitCode = -1;

        private List<Mesh1D> mesh1DList;
        private List<Mesh2D> mesh2DList;
        private List<Contacts> contactsList;
        private List<Network1D> network1DList;


        public UGridWrapper(string path)
        {
            Read(path);
            GetMesh1D();
            GetMesh2D();
            GetContacts();
            GetNetwork1D();
        }

        private void Dispose(bool disposing)
        {
            if (disposing)
            {
                mesh1DList.ForEach(item => item.Free());
                mesh2DList.ForEach(item => item.Free());
                contactsList.ForEach(item => item.Free());
                network1DList.ForEach(item => item.Free());
                UGrid.ug_file_close(fileId);
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void ProcessExitCode(int exitCode)
        {
            string message = "";
            UGrid.ug_error_get(message);

            if (!string.IsNullOrWhiteSpace(message))
            {
                //log.Error(errorMessage);
                // log to standard output for now
                Console.WriteLine(message);
            }
        }

        private void Read(string path)
        {
            int fileMode = -1;
            exitCode = UGrid.ug_file_read_mode(ref fileMode);
            ProcessExitCode(exitCode);
            exitCode = UGrid.ug_file_open(path, fileMode, ref fileId);
            ProcessExitCode(exitCode);
        }

        private void GetMesh1D()
        {
            int count = -1;
            exitCode = UGrid.ug_topology_get_count(fileId, TopologyType.Mesh1dTopology, ref count);
            ProcessExitCode(exitCode);
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Mesh1D mesh1D = new Mesh1D();
                    exitCode = UGrid.ug_mesh1d_inq(fileId, i, mesh1D);
                    ProcessExitCode(exitCode);
                    mesh1D.Allocate();
                    exitCode = UGrid.ug_mesh1d_get(fileId, i, mesh1D);
                    ProcessExitCode(exitCode);
                    mesh1DList.Add(mesh1D);
                }
            }
        }

        private void GetMesh2D()
        {
            int count = -1;
            exitCode = UGrid.ug_topology_get_count(fileId, TopologyType.Mesh2dTopology, ref count);
            ProcessExitCode(exitCode);
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Mesh2D mesh2D = new Mesh2D();
                    exitCode = UGrid.ug_mesh2d_inq(fileId, i, mesh2D);
                    ProcessExitCode(exitCode);
                    mesh2D.Allocate();
                    exitCode = UGrid.ug_mesh2d_get(fileId, i, mesh2D);
                    ProcessExitCode(exitCode);
                    mesh2DList.Add(mesh2D);
                }
            }
        }

        private void GetContacts()
        {
            int count = -1;
            exitCode = UGrid.ug_topology_get_count(fileId, TopologyType.ContactsTopology, ref count);
            ProcessExitCode(exitCode);
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Contacts contacts = new Contacts();
                    exitCode = UGrid.ug_contacts_inq(fileId, i, contacts);
                    ProcessExitCode(exitCode);
                    contacts.Allocate();
                    exitCode = UGrid.ug_contacts_get(fileId, i, contacts);
                    ProcessExitCode(exitCode);
                    contactsList.Add(contacts);
                }
            }
        }
        private void GetNetwork1D()
        {
            int count = -1;
            exitCode = UGrid.ug_topology_get_count(fileId, TopologyType.ContactsTopology, ref count);
            ProcessExitCode(exitCode);
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Network1D network1D = new Network1D();
                    exitCode = UGrid.ug_network1d_inq(fileId, i, network1D);
                    ProcessExitCode(exitCode);
                    network1D.Allocate();
                    exitCode = UGrid.ug_network1d_get(fileId, i, network1D);
                    ProcessExitCode(exitCode);
                    network1DList.Add(network1D);
                }
            }
        }

    }


    [TestFixture]
    public class UGridNetTests
    {

        private static string GetTestDataPath()
        {
            string path = Environment.GetEnvironmentVariable("UGRID_TEST_DATA_DIR");

            // Check if the environment variable is set
            if (string.IsNullOrEmpty(path))
            {
                throw new InvalidOperationException("The UGRID_TEST_DATA_DIR environment variable is not set.");
            }

            // Check if the path is absolute and that it exists
            if (!(Path.IsPathRooted(path) && Directory.Exists(path)))
            {
                throw new InvalidOperationException(
                    $"The specified UGRID_TEST_DATA_DIR path must be absolute and should exist. UGRID_TEST_DATA_DIR={path}");
            }

            return path;
        }

        private static readonly string TestDataPath = GetTestDataPath();

        [Test]
        public void GetDoubleDataByVariableName()
        {
            int result = -1;

            int fileMode = -1;
            result = UGrid.ug_file_read_mode(ref fileMode);
            Assert.That(result, Is.EqualTo(0));

            int fileId = -1;
            string filePath = Path.Combine(TestDataPath, "ResultFile.nc");
            result = UGrid.ug_file_open(filePath, fileMode, ref fileId);
            Assert.That(result, Is.EqualTo(0));

            int dimensionsCount = 0;
            string variableName = "mesh1d_s0                                                                       ";
            result = UGrid.ug_variable_count_dimensions(fileId, variableName, ref dimensionsCount);
            Assert.That(result, Is.EqualTo(0));

            var dimensionVec = new int[dimensionsCount];
            UGrid.ug_variable_get_data_dimensions(fileId, variableName, dimensionVec);
            Assert.That(result, Is.EqualTo(0));

            int totalDimension = 1;
            for (int i = 0; i < dimensionsCount; i++)
            {
                totalDimension *= dimensionVec[i];
            }

            var data = new double[totalDimension];
            UGrid.ug_variable_get_data_double(fileId, variableName, data);
            Assert.That(result, Is.EqualTo(0));

            // compare 20 values between 86 and 106
            const int offset = 85;
            const int sampleSize = 20;
            double[] expectedData = new double[sampleSize]
            {
                -5.0,
                0.27,
                0.75,
                -0.36,
                0.27,
                0.56,
                0.83,
                0.79,
                0.8,
                0.81,
                0.88,
                0.63,
                0.7,
                1.02,
                1.07,
                0.97,
                1.02,
                1.14,
                1.13,
                0.9
            };

            //for (int i = 86; i < 106; i++)
            for (int i = 0; i < sampleSize; i++)
            {
                Assert.That(data[i + offset], Is.EqualTo(expectedData[i]));
            }

            result = UGrid.ug_file_close(fileId);
            Assert.That(result, Is.EqualTo(0));
        }

        [Test]
        public void TestMesh1DReadAndInquire()
        {
            var mesh1D = new Mesh1D();
            try
            {
                int result = -1;

                int fileMode = -1;

                result = UGrid.ug_file_read_mode(ref fileMode);
                Assert.That(result, Is.EqualTo(0));

                int fileId = -1;
                string filePath = Path.Combine(TestDataPath, "AllUGridEntities.nc");
                result = UGrid.ug_file_open(filePath, fileMode, ref fileId);
                Assert.That(result, Is.EqualTo(0));

                //// get the number of topologies
                int numTopologies = 0;
                result = UGrid.ug_topology_get_count(fileId, TopologyType.Mesh1dTopology, ref numTopologies);
                Assert.That(numTopologies, Is.EqualTo(1));
                Assert.That(result, Is.EqualTo(0));

                // get the mesh
                const int topologyId = 0;
                result = UGrid.ug_mesh1d_inq(fileId, topologyId, mesh1D);
                Assert.That(result, Is.EqualTo(0));
                Assert.That(mesh1D.num_nodes, Is.EqualTo(25));
                Assert.That(mesh1D.num_edges, Is.EqualTo(24));

                int int_bytes = Marshal.SizeOf<int>();
                int double_bytes = Marshal.SizeOf<double>();
                mesh1D.node_x = Marshal.AllocHGlobal(mesh1D.num_nodes * double_bytes);
                mesh1D.edge_nodes = Marshal.AllocHGlobal(mesh1D.num_edges * 2 * int_bytes);



                // get the data
                result = UGrid.ug_mesh1d_get(fileId, 0, mesh1D);
                Assert.That(result, Is.EqualTo(0));

                var arr_1 = mesh1D.edge_nodes.CopyToArray<int>(mesh1D.num_edges * 2);

                var src = new int[mesh1D.num_edges * 2];
                for (uint i = 0; i < mesh1D.num_edges * 2; i++)
                {
                    src[i] = 666;
                }

                mesh1D.edge_nodes.CopyFromArray(src);

                var arr_2 = mesh1D.edge_nodes.CopyToArray<int>(mesh1D.num_edges * 2);

                for (uint i = 0; i < mesh1D.num_edges * 2; i++)
                {
                    Console.WriteLine("{0} : {1}", arr_1[i], arr_2[i]);
                }

                Marshal.FreeHGlobal(mesh1D.node_x);
                Marshal.FreeHGlobal(mesh1D.edge_nodes);

                result = UGrid.ug_file_close(fileId);
                Assert.That(result, Is.EqualTo(0));
            }
            finally
            {
                mesh1D.Dispose();
            }
        }
    }
}