using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
using System.IO;
using System.Runtime.InteropServices;
using UGridNET.Extensions;

namespace UGridNET
{
    public sealed class UGridWrapper : IDisposable
    {
        // requires using log4net;
        //private static readonly ILog log = LogManager.GetLogger(typeof(UGridWrapper));

        private bool disposed = false;
        private int fileID = -1;
        private List<Mesh1D> mesh1DList = new List<Mesh1D>();
        private List<Mesh2D> mesh2DList = new List<Mesh2D>();
        private List<Contacts> contactsList = new List<Contacts>();
        private List<Network1D> network1DList = new List<Network1D>();


        public UGridWrapper(string path)
        {
            try
            {
                Read(path);
                GetMesh1D();
                GetMesh2D();
                GetContacts();
                GetNetwork1D();
            }
            catch
            {
                // If initialization fails, dispose of any partially initialized resources
                Dispose();
                // Rethrow to inform the caller of the failure
                throw;
            }
        }

        ~UGridWrapper()
        {
            // Cleanup in case Dispose wasn't called
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            if (!disposed)
            {
                if (disposing)
                {
                    // Free memory allocated by the constructor
                    mesh1DList.ForEach(item => item.Free());
                    mesh2DList.ForEach(item => item.Free());
                    contactsList.ForEach(item => item.Free());
                    network1DList.ForEach(item => item.Free());

                    // Close the file
                    UGrid.ug_file_close(fileID);
                }

                disposed = true;
            }
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
            Invoke(() => UGrid.ug_file_read_mode(ref fileMode));
            Invoke(() => UGrid.ug_file_open(System.Text.Encoding.Default.GetBytes(path), fileMode, ref fileID));
        }

        private void GetMesh1D()
        {
            int count = 0;
            Invoke(() => UGrid.ug_topology_get_count(fileID, TopologyType.Mesh1dTopology, ref count));
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Mesh1D mesh1D = new Mesh1D();
                    Invoke(() => UGrid.ug_mesh1d_inq(fileID, i, mesh1D));
                    mesh1D.Allocate();
                    Invoke(() => UGrid.ug_mesh1d_get(fileID, i, mesh1D));
                    mesh1DList.Add(mesh1D);
                }
            }
        }

        private void GetMesh2D()
        {
            int count = 0;
            Invoke(() => UGrid.ug_topology_get_count(fileID, TopologyType.Mesh2dTopology, ref count));
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Mesh2D mesh2D = new Mesh2D();
                    Invoke(() => UGrid.ug_mesh2d_inq(fileID, i, mesh2D));
                    mesh2D.Allocate();
                    Invoke(() => UGrid.ug_mesh2d_get(fileID, i, mesh2D));
                    mesh2DList.Add(mesh2D);

                    //var name = mesh2D.name.CopyToArray<byte>(UGrid.name_long_length);
                    //Console.WriteLine(">>> name: {0}", System.Text.Encoding.UTF8.GetString(name));

                    //var name = Marshal.PtrToStringAnsi(mesh2D.name);
                    //Console.WriteLine(">>> name: {0}", name);
                }
            }
        }

        private void GetContacts()
        {
            int count = 0;
            Invoke(() => UGrid.ug_topology_get_count(fileID, TopologyType.ContactsTopology, ref count));
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Contacts contacts = new Contacts();
                    Invoke(() => UGrid.ug_contacts_inq(fileID, i, contacts));
                    contacts.Allocate();
                    Invoke(() => UGrid.ug_contacts_get(fileID, i, contacts));
                    contactsList.Add(contacts);
                }
            }
        }
        private void GetNetwork1D()
        {
            int count = 0;
            Invoke(() => UGrid.ug_topology_get_count(fileID, TopologyType.Network1dTopology, ref count));
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Network1D network1D = new Network1D();
                    Invoke(() => UGrid.ug_network1d_inq(fileID, i, network1D));
                    network1D.Allocate();
                    Invoke(() => UGrid.ug_network1d_get(fileID, i, network1D));
                    network1DList.Add(network1D);
                }
            }
        }

        // static private byte[] Name()
        // {
        //     int length = 0;
        //     Invoke(() => UGrid.ug_name_get_length(ref length));
        //     return new byte[length];
        // }

        // static private byte[] LongName()
        // {
        //     int length = 0;
        //     Invoke(() => UGrid.ug_name_get_long_length(ref length));
        //     return new byte[length];
        // }

        public string GetDataVariablesNames(int topologyID, TopologyType topologyType, MeshLocations meshLocation)
        {
            byte[] names = new byte[512];
            Invoke(() => UGrid.ug_topology_get_data_variables_names(fileID, topologyID, topologyType, meshLocation, names));
            return System.Text.Encoding.Default.GetString(names);
        }

        public T[] GetVariableByName<T>(string variableNameStr)
        {
            int dimensionsCount = 0;
            byte[] variableName = System.Text.Encoding.Default.GetBytes(variableNameStr);
            Invoke(() => UGrid.ug_variable_count_dimensions(fileID, variableName, ref dimensionsCount));

            int[] dimensionVec = new int[dimensionsCount];
            Invoke(() => UGrid.ug_variable_get_data_dimensions(fileID, variableName, dimensionVec));

            int totalDimension = 1;
            for (int i = 0; i < dimensionsCount; i++)
            {
                totalDimension *= dimensionVec[i];
            }

            T[] data = new T[totalDimension];

            if (typeof(T) == typeof(double))
            {
                Invoke(() => UGrid.ug_variable_get_data_double(fileID, variableName, data as double[]));
            }
            else if (typeof(T) == typeof(int))
            {
                Invoke(() => UGrid.ug_variable_get_data_int(fileID, variableName, data as int[]));
            }
            else if (typeof(T) == typeof(byte))
            {
                Invoke(() => UGrid.ug_variable_get_data_char(fileID, variableName, data as byte[]));
            }
            else
            {
                throw new NotSupportedException("Currently only int, double and char data types are supported.");
            }

            return data;
        }

    }

}