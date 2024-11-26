using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
using System.IO;
using System.Runtime.InteropServices;
using UGridNET.Extensions;

namespace UGridNET
{
    public class UGridBase : IDisposable
    {
        // requires using log4net;
        //private static readonly ILog log = LogManager.GetLogger(typeof(UGridBase));

        private bool disposed = false;
        protected int fileID = -1;
        protected List<Mesh1D> mesh1DList = new List<Mesh1D>();
        protected List<Mesh2D> mesh2DList = new List<Mesh2D>();
        protected List<Contacts> contactsList = new List<Contacts>();
        protected List<Network1D> network1DList = new List<Network1D>();

        public UGridBase(string path, int openMode)
        {
            try
            {
                Open(path, openMode);
            }
            catch
            {
                // If initialization fails, dispose of any partially initialized resources
                Dispose();
                // Rethrow to inform the caller of the failure
                throw;
            }
        }

        ~UGridBase()
        {
            // Cleanup in case Dispose wasn't called
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
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
                byte[] messageBytes = new byte[UGrid.error_message_buffer_size];
                UGrid.ug_error_get(messageBytes);
                string messageStr = System.Text.Encoding.Default.GetString(messageBytes);

                if (!string.IsNullOrWhiteSpace(messageStr))
                {
                    throw new Exception(messageStr);
                }
            }
        }

        protected static void Invoke(Func<int> func)
        {
            int exitCode = func();
            ProcessExitCode(exitCode);
        }


        private void Open(string path, int openMode)
        {
            Invoke(() => UGrid.ug_file_open(System.Text.Encoding.Default.GetBytes(path), openMode, ref fileID));
        }

        public string GetDataVariablesNames(TopologyType topologyType, int topologyID, MeshLocations meshLocation)
        {
            byte[] names = new byte[UGrid.error_message_buffer_size];
            Invoke(() => UGrid.ug_topology_get_data_variables_names(fileID, topologyType, topologyID, meshLocation, names));
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