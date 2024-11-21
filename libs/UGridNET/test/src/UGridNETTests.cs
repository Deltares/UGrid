using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
using System.IO;
using System.Runtime.InteropServices;
using NUnit.Framework;
using UGridNET.Tests.TopologyExtensions;

namespace UGridNET.Tests
{

    internal static class ByteArrayExtensions
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="bytes"></param>
        /// <returns></returns>
        /// <exception cref="ArgumentNullException"></exception>
        public static string ToString(this byte[] bytes)
        {
            if (bytes == null)
            {
                throw new ArgumentNullException(nameof(bytes));
            }
            return System.Text.Encoding.UTF8.GetString(bytes);
        }

        public static byte[] FromString(string str)
        {
            if (str == null)
            {
                throw new ArgumentNullException(nameof(str));
            }
            return System.Text.Encoding.UTF8.GetBytes(str);
        }
    }

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

            if (typeof(T) == typeof(int))
            {
                Marshal.Copy(pointer, array as int[], 0, array.Length);
            }
            else if (typeof(T) == typeof(double))
            {
                Marshal.Copy(pointer, array as double[], 0, array.Length);
            }
            else if (typeof(T) == typeof(byte))
            {
                Marshal.Copy(pointer, array as byte[], 0, array.Length);
            }
            else
            {
                throw new NotSupportedException("Currently only int, double and byte data types are supported.");
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
            else if (typeof(T) == typeof(byte))
            {
                Marshal.Copy(array as byte[], 0, pointer, array.Length);
            }
            else
            {
                throw new NotSupportedException("Currently only int and double data types are supported.");
            }
        }
    }

    namespace TopologyExtensions
    {
        internal static class Constants
        {
            public static readonly int intBytes = Marshal.SizeOf<int>();
            public static int doubleBytes = Marshal.SizeOf<double>();
            public static int byteBytes = Marshal.SizeOf<byte>();
        }


        internal static class Mesh1DExtensions
        {
            public static void Allocate(this Mesh1D mesh1D)
            {
                mesh1D.node_x = Marshal.AllocHGlobal(mesh1D.num_nodes * Constants.doubleBytes);
                mesh1D.node_y = Marshal.AllocHGlobal(mesh1D.num_nodes * Constants.doubleBytes);
                mesh1D.edge_x = Marshal.AllocHGlobal(mesh1D.num_edges * Constants.doubleBytes);
                mesh1D.edge_y = Marshal.AllocHGlobal(mesh1D.num_edges * Constants.doubleBytes);
                mesh1D.edge_nodes = Marshal.AllocHGlobal(mesh1D.num_edges * 2 * Constants.intBytes);
                mesh1D.edge_edge_id = Marshal.AllocHGlobal(mesh1D.num_nodes * Constants.intBytes); // size?
                mesh1D.node_edge_id = Marshal.AllocHGlobal(mesh1D.num_nodes * Constants.intBytes); // size?
                mesh1D.node_edge_offset = Marshal.AllocHGlobal(mesh1D.num_nodes * Constants.doubleBytes);
            }

            public static void Free(this Mesh1D mesh1D)
            {
                if (mesh1D.node_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.node_x);
                if (mesh1D.node_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.node_y);
                if (mesh1D.edge_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.edge_x);
                if (mesh1D.edge_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh1D.edge_y);
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
                mesh2D.name = Marshal.AllocHGlobal(UGrid.name_long_length);
                mesh2D.node_x = Marshal.AllocHGlobal(mesh2D.num_nodes * Constants.doubleBytes);
                mesh2D.node_y = Marshal.AllocHGlobal(mesh2D.num_nodes * Constants.doubleBytes);
                mesh2D.node_z = Marshal.AllocHGlobal(mesh2D.num_nodes * Constants.doubleBytes);
                mesh2D.edge_x = Marshal.AllocHGlobal(mesh2D.num_edges * Constants.doubleBytes);
                mesh2D.edge_y = Marshal.AllocHGlobal(mesh2D.num_edges * Constants.doubleBytes);
                mesh2D.edge_z = Marshal.AllocHGlobal(mesh2D.num_edges * Constants.doubleBytes);
                mesh2D.face_x = Marshal.AllocHGlobal(mesh2D.num_faces * Constants.doubleBytes);
                mesh2D.face_y = Marshal.AllocHGlobal(mesh2D.num_faces * Constants.doubleBytes);
                mesh2D.face_z = Marshal.AllocHGlobal(mesh2D.num_faces * Constants.doubleBytes);
                mesh2D.edge_nodes = Marshal.AllocHGlobal(mesh2D.num_edges * 2 * Constants.intBytes);
                mesh2D.edge_faces = Marshal.AllocHGlobal(mesh2D.num_edges * 2 * Constants.intBytes);
                mesh2D.face_nodes = Marshal.AllocHGlobal(mesh2D.num_faces * mesh2D.num_face_nodes_max * Constants.intBytes);
                mesh2D.face_edges = Marshal.AllocHGlobal(mesh2D.num_faces * mesh2D.num_face_nodes_max * Constants.intBytes);
                mesh2D.face_faces = Marshal.AllocHGlobal(mesh2D.num_faces * mesh2D.num_face_nodes_max * Constants.intBytes);
            }

            public static void Free(this Mesh2D mesh2D)
            {
                if (mesh2D.name != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.name);
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
                contacts.edges = Marshal.AllocHGlobal(contacts.num_contacts * 2 * Constants.intBytes);
                contacts.contact_type = Marshal.AllocHGlobal(contacts.num_contacts * Constants.intBytes);
            }

            public static void Free(this Contacts contacts)
            {
                if (contacts.edges != IntPtr.Zero) Marshal.FreeHGlobal(contacts.edges);
                if (contacts.contact_type != IntPtr.Zero) Marshal.FreeHGlobal(contacts.contact_type);
            }
        }

        internal static class Network1DExtensions
        {
            public static void Allocate(this Network1D network)
            {
                network.node_x = Marshal.AllocHGlobal(network.num_nodes * Constants.doubleBytes);
                network.node_y = Marshal.AllocHGlobal(network.num_nodes * Constants.doubleBytes);
                network.edge_nodes = Marshal.AllocHGlobal(network.num_edges * 2 * Constants.intBytes);
                network.edge_length = Marshal.AllocHGlobal(network.num_edges * Constants.doubleBytes);
                network.edge_order = Marshal.AllocHGlobal(network.num_edges * Constants.intBytes);
                network.geometry_nodes_x = Marshal.AllocHGlobal(network.num_geometry_nodes * Constants.doubleBytes);
                network.geometry_nodes_y = Marshal.AllocHGlobal(network.num_geometry_nodes * Constants.doubleBytes);
                network.num_edge_geometry_nodes = Marshal.AllocHGlobal(network.num_edges * Constants.intBytes);
            }

            public static void Free(this Network1D network)
            {
                if (network.node_x != IntPtr.Zero) Marshal.FreeHGlobal(network.node_x);
                if (network.node_y != IntPtr.Zero) Marshal.FreeHGlobal(network.node_y);
                if (network.edge_nodes != IntPtr.Zero) Marshal.FreeHGlobal(network.edge_nodes);
                if (network.edge_length != IntPtr.Zero) Marshal.FreeHGlobal(network.edge_length);
                if (network.edge_order != IntPtr.Zero) Marshal.FreeHGlobal(network.edge_order);
                if (network.geometry_nodes_x != IntPtr.Zero) Marshal.FreeHGlobal(network.geometry_nodes_x);
                if (network.geometry_nodes_y != IntPtr.Zero) Marshal.FreeHGlobal(network.geometry_nodes_y);
                if (network.num_edge_geometry_nodes != IntPtr.Zero) Marshal.FreeHGlobal(network.num_edge_geometry_nodes);
            }
        }
    }

    public sealed class UGridWrapper : IDisposable
    {
        // requires using log4net;
        //private static readonly ILog log = LogManager.GetLogger(typeof(UGridWrapper));

        private int fileID = -1;
        private bool isOpen = false;
        private List<Mesh1D> mesh1DList = new List<Mesh1D>();
        private List<Mesh2D> mesh2DList = new List<Mesh2D>();
        private List<Contacts> contactsList = new List<Contacts>();
        private List<Network1D> network1DList = new List<Network1D>();


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
                // free memory allocated by the constructor
                mesh1DList.ForEach(item => item.Free());
                mesh2DList.ForEach(item => item.Free());
                contactsList.ForEach(item => item.Free());
                network1DList.ForEach(item => item.Free());

                // close the file
                if (isOpen)
                {
                    UGrid.ug_file_close(fileID);
                }
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public bool HasMesh1D() { return mesh1DList.Count > 0; }

        public bool HasMesh2D() { return mesh2DList.Count > 0; }

        public bool HasContacts() { return contactsList.Count > 0; }

        public bool HasNetwork1D() { return network1DList.Count > 0; }

        public ReadOnlyCollection<Mesh1D> Mesh1DList() { return mesh1DList.AsReadOnly(); }

        public ReadOnlyCollection<Mesh2D> Mesh2DList() { return mesh2DList.AsReadOnly(); }

        public ReadOnlyCollection<Contacts> ContactsList() { return contactsList.AsReadOnly(); }

        public ReadOnlyCollection<Network1D> Network1DList() { return network1DList.AsReadOnly(); }

        private static void ProcessExitCode(int exitCode)
        {
            if (exitCode != 0)
            {
                byte[] messageBytes = new byte[512];
                UGrid.ug_error_get(messageBytes);
                string messageStr = System.Text.Encoding.Default.GetString(messageBytes);

                if (!string.IsNullOrWhiteSpace(messageStr))
                {
                    throw new Exception(messageStr);
                }
            }
        }

        private static void Invoke(Func<int> func)
        {
            int exitCode = func();
            ProcessExitCode(exitCode);
        }


        private void Read(string path)
        {
            int fileMode = -1;
            int exitCode = UGrid.ug_file_read_mode(ref fileMode);
            ProcessExitCode(exitCode);

            //Invoke(() => UGrid.ug_file_read_mode(ref fileMode));


            exitCode = UGrid.ug_file_open(System.Text.Encoding.Default.GetBytes(path), fileMode, ref fileID);
            isOpen = exitCode == 0;
            ProcessExitCode(exitCode);

            //Invoke(() => UGrid.ug_file_open(System.Text.Encoding.Default.GetBytes(path), fileMode, ref fileID));
        }

        private void GetMesh1D()
        {
            int count = 0;
            int exitCode = UGrid.ug_topology_get_count(fileID, TopologyType.Mesh1dTopology, ref count);
            ProcessExitCode(exitCode);
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Mesh1D mesh1D = new Mesh1D();
                    exitCode = UGrid.ug_mesh1d_inq(fileID, i, mesh1D);
                    ProcessExitCode(exitCode);
                    mesh1D.Allocate();
                    exitCode = UGrid.ug_mesh1d_get(fileID, i, mesh1D);
                    ProcessExitCode(exitCode);
                    mesh1DList.Add(mesh1D);
                }
            }
        }

        private void GetMesh2D()
        {
            int count = 0;
            int exitCode = UGrid.ug_topology_get_count(fileID, TopologyType.Mesh2dTopology, ref count);
            ProcessExitCode(exitCode);
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Mesh2D mesh2D = new Mesh2D();
                    exitCode = UGrid.ug_mesh2d_inq(fileID, i, mesh2D);
                    ProcessExitCode(exitCode);
                    mesh2D.Allocate();
                    exitCode = UGrid.ug_mesh2d_get(fileID, i, mesh2D);
                    ProcessExitCode(exitCode);
                    mesh2DList.Add(mesh2D);
                    var name = mesh2D.name.CopyToArray<byte>(UGrid.name_long_length);
                    Console.WriteLine(">>> name: {0}", System.Text.Encoding.UTF8.GetString(name));
                }
            }
        }

        private void GetContacts()
        {
            int count = 0;
            int exitCode = UGrid.ug_topology_get_count(fileID, TopologyType.ContactsTopology, ref count);
            ProcessExitCode(exitCode);
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Contacts contacts = new Contacts();
                    exitCode = UGrid.ug_contacts_inq(fileID, i, contacts);
                    ProcessExitCode(exitCode);
                    contacts.Allocate();
                    exitCode = UGrid.ug_contacts_get(fileID, i, contacts);
                    ProcessExitCode(exitCode);
                    contactsList.Add(contacts);
                }
            }
        }
        private void GetNetwork1D()
        {
            int count = 0;
            int exitCode = UGrid.ug_topology_get_count(fileID, TopologyType.Network1dTopology, ref count);
            ProcessExitCode(exitCode);
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Network1D network1D = new Network1D();
                    exitCode = UGrid.ug_network1d_inq(fileID, i, network1D);
                    ProcessExitCode(exitCode);
                    network1D.Allocate();
                    exitCode = UGrid.ug_network1d_get(fileID, i, network1D);
                    ProcessExitCode(exitCode);
                    network1DList.Add(network1D);
                }
            }
        }

        // static private byte[] Name()
        // {
        //     int length = 0;
        //     int exit_code = UGrid.ug_name_get_length(ref length);
        //     return new byte[length];
        // }

        // static private byte[] LongName()
        // {
        //     int length = 0;
        //     int exit_code = UGrid.ug_name_get_long_length(ref length);
        //     return new byte[length];
        // }

        public string GetDataVariablesNames(int topologyID, TopologyType topologyType, MeshLocations meshLocation)
        {
            byte[] names = new byte[512];
            int exitCode = UGrid.ug_topology_get_data_variables_names(fileID, topologyID, topologyType, meshLocation, names);
            ProcessExitCode(exitCode);
            return System.Text.Encoding.Default.GetString(names);
        }

        public T[] GetVariableByName<T>(string variableNameStr)
        {
            int dimensionsCount = 0;
            byte[] variableName = System.Text.Encoding.Default.GetBytes(variableNameStr);
            int exitCode = UGrid.ug_variable_count_dimensions(fileID, variableName, ref dimensionsCount);
            ProcessExitCode(exitCode);

            int[] dimensionVec = new int[dimensionsCount];
            exitCode = UGrid.ug_variable_get_data_dimensions(fileID, variableName, dimensionVec);
            ProcessExitCode(exitCode);

            int totalDimension = 1;
            for (int i = 0; i < dimensionsCount; i++)
            {
                totalDimension *= dimensionVec[i];
            }

            T[] data = new T[totalDimension];

            if (typeof(T) == typeof(double))
            {
                exitCode = UGrid.ug_variable_get_data_double(fileID, variableName, data as double[]);
            }
            else if (typeof(T) == typeof(int))
            {
                exitCode = UGrid.ug_variable_get_data_int(fileID, variableName, data as int[]);
            }
            else if (typeof(T) == typeof(byte))
            {
                exitCode = UGrid.ug_variable_get_data_char(fileID, variableName, data as byte[]);
            }
            else
            {
                throw new NotSupportedException("Currently only int, double and char data types are supported.");
            }

            ProcessExitCode(exitCode);

            return data;
        }

    }


    [TestFixture]
    public class UGridNetTests
    {

        private static string GetTestDataPath()
        {
            //string path = Environment.GetEnvironmentVariable("UGRID_TEST_DATA_DIR");

            //// Check if the environment variable is set
            //if (string.IsNullOrEmpty(path))
            //{
            //    throw new InvalidOperationException("The UGRID_TEST_DATA_DIR environment variable is not set.");
            //}

            //// Check if the path is absolute and that it exists
            //if (!(Path.IsPathRooted(path) && Directory.Exists(path)))
            //{
            //    throw new InvalidOperationException(
            //        $"The specified UGRID_TEST_DATA_DIR path must be absolute and should exist. UGRID_TEST_DATA_DIR={path}");
            //}

            //return path;

            return "C:\\Users\\sayed\\work\\repositories\\UGrid\\tests\\data";
        }

        private static readonly string TestDataPath = GetTestDataPath();

        [Test]
        public void OpenNonExistentFileThrows()
        {
            string filePath = "nonExistentFile.nc";
            UGridWrapper ugrid = null;

            try
            {
                Assert.Throws<Exception>(() => ugrid = new UGridWrapper(filePath));

            }
            finally
            {
                if (ugrid != null)
                {
                    ugrid.Dispose();
                }
            }

        }

        [Test]
        public void OpenSucceeds()
        {
            string filePath = Path.Combine(TestDataPath, "AllUGridEntities.nc");
            UGridWrapper ugrid = null;

            try
            {
                ugrid = new UGridWrapper(filePath);
                var mesh2D = ugrid.Mesh2DList();
                var node_x = mesh2D[0].node_x.CopyToArray<double>(mesh2D[0].num_nodes);
                var node_y = mesh2D[0].node_y.CopyToArray<double>(mesh2D[0].num_nodes);
                // for (int i = 0; i < node_x.Length; i++)
                // {
                //     Console.WriteLine("x = {0}, y = {1}", node_x[i], node_y[i]);
                // }

            }
            finally
            {
                if (ugrid != null)
                {
                    ugrid.Dispose();
                }
            }

        }

        [Test]
        public void GetDoubleDataByVariableName()
        {
            int result = -1;

            int fileMode = -1;
            result = UGrid.ug_file_read_mode(ref fileMode);
            Assert.That(result, Is.EqualTo(0));

            int fileID = -1;
            string filePath = Path.Combine(TestDataPath, "ResultFile.nc");
            result = UGrid.ug_file_open(System.Text.Encoding.Default.GetBytes(filePath), fileMode, ref fileID);
            Assert.That(result, Is.EqualTo(0));

            int dimensionsCount = 0;
            string variableNameStr = "mesh1d_s0                                                                       ";
            byte[] variableName = System.Text.Encoding.Default.GetBytes(variableNameStr);
            result = UGrid.ug_variable_count_dimensions(fileID, variableName, ref dimensionsCount);
            Assert.That(result, Is.EqualTo(0));

            var dimensionVec = new int[dimensionsCount];
            UGrid.ug_variable_get_data_dimensions(fileID, variableName, dimensionVec);
            Assert.That(result, Is.EqualTo(0));

            int totalDimension = 1;
            for (int i = 0; i < dimensionsCount; i++)
            {
                totalDimension *= dimensionVec[i];
            }

            var data = new double[totalDimension];
            UGrid.ug_variable_get_data_double(fileID, variableName, data);
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

            for (int i = 0; i < sampleSize; i++)
            {
                Assert.That(data[i + offset], Is.EqualTo(expectedData[i]));
            }

            result = UGrid.ug_file_close(fileID);
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

                int fileID = -1;
                string filePath = Path.Combine(TestDataPath, "AllUGridEntities.nc");
                result = UGrid.ug_file_open(System.Text.Encoding.Default.GetBytes(filePath), fileMode, ref fileID);
                Assert.That(result, Is.EqualTo(0));

                //// get the number of topologies
                int numTopologies = 0;
                result = UGrid.ug_topology_get_count(fileID, TopologyType.Mesh1dTopology, ref numTopologies);
                Assert.That(numTopologies, Is.EqualTo(1));
                Assert.That(result, Is.EqualTo(0));

                // get the mesh
                const int topologyId = 0;
                result = UGrid.ug_mesh1d_inq(fileID, topologyId, mesh1D);
                Assert.That(result, Is.EqualTo(0));
                Assert.That(mesh1D.num_nodes, Is.EqualTo(25));
                Assert.That(mesh1D.num_edges, Is.EqualTo(24));

                mesh1D.node_x = Marshal.AllocHGlobal(mesh1D.num_nodes * Constants.doubleBytes);
                mesh1D.edge_nodes = Marshal.AllocHGlobal(mesh1D.num_edges * 2 * Constants.intBytes);



                // get the data
                result = UGrid.ug_mesh1d_get(fileID, 0, mesh1D);
                Assert.That(result, Is.EqualTo(0));

                var arr_1 = mesh1D.edge_nodes.CopyToArray<int>(mesh1D.num_edges * 2);

                var src = new int[mesh1D.num_edges * 2];
                for (uint i = 0; i < mesh1D.num_edges * 2; i++)
                {
                    src[i] = 666;
                }

                mesh1D.edge_nodes.CopyFromArray(src);

                var arr_2 = mesh1D.edge_nodes.CopyToArray<int>(mesh1D.num_edges * 2);

                // for (uint i = 0; i < mesh1D.num_edges * 2; i++)
                // {
                //     Console.WriteLine("{0} : {1}", arr_1[i], arr_2[i]);
                // }

                Marshal.FreeHGlobal(mesh1D.node_x);
                Marshal.FreeHGlobal(mesh1D.edge_nodes);

                result = UGrid.ug_file_close(fileID);
                Assert.That(result, Is.EqualTo(0));
            }
            finally
            {
                mesh1D.Dispose();
            }
        }
    }
}