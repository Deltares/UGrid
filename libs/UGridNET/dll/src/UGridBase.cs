using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Runtime.InteropServices;
using UGridNET.Extensions;

namespace UGridNET
{
    public class UGridBase : IDisposable
    {
        private bool disposed = false;
        protected int fileID = -1;
        private readonly string filePath;
        protected List<Mesh1D> mesh1DList = new List<Mesh1D>();
        protected List<Mesh2D> mesh2DList = new List<Mesh2D>();
        protected List<Contacts> contactsList = new List<Contacts>();
        protected List<Network1D> network1DList = new List<Network1D>();

        public UGridBase(string path, int openMode)
        {
            try
            {
                filePath = Path.GetFullPath(path);
                Open(openMode);
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
                // Free unmanaged memory of IntPtrs belonging to the different entities
                FreeUnmanagedMemoryInTopologyLists();

                // Close the file
                Close();

                if (disposing)
                {
                    DisposeAndClearTopologyLists();
                }

                disposed = true;
            }
        }

        private void DisposeAndClearLists<T>(List<T> list) where T : IDisposable
        {
            list.ForEach(item => item.Dispose());
            list.Clear();
        }

        private void DisposeAndClearTopologyLists()
        {
            DisposeAndClearLists(mesh1DList);
            DisposeAndClearLists(mesh2DList);
            DisposeAndClearLists(contactsList);
            DisposeAndClearLists(network1DList);
        }

        private void FreeUnmanagedMemoryInTopologyLists()
        {
            mesh1DList.ForEach(item => item.Free());
            mesh2DList.ForEach(item => item.Free());
            contactsList.ForEach(item => item.Free());
            network1DList.ForEach(item => item.Free());
        }

        public bool HasMesh1D() { return mesh1DList.Count > 0; }

        public bool HasMesh2D() { return mesh2DList.Count > 0; }

        public bool HasContacts() { return contactsList.Count > 0; }

        public bool HasNetwork1D() { return network1DList.Count > 0; }

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Mesh1D> Mesh1DList() { return mesh1DList.AsReadOnly(); }

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Mesh2D> Mesh2DList() { return mesh2DList.AsReadOnly(); }

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Contacts> ContactsList() { return contactsList.AsReadOnly(); }

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Network1D> Network1DList() { return network1DList.AsReadOnly(); }

        private static void ProcessExitCode(int exitCode)
        {
            if (exitCode != 0)
            {
                var messageBytes = new byte[UGrid.error_message_buffer_size];
                UGrid.ug_error_get(messageBytes);
                var messageStr = messageBytes.GetStringFromNullTerminatedArray(true);

                if (!string.IsNullOrWhiteSpace(messageStr))
                {
                    throw new UGridNETException(messageStr);
                }
            }
        }

        protected static void Invoke(Func<int> func)
        {
            ProcessExitCode(func());
        }


        private void Open(int openMode)
        {
            Invoke(() => UGrid.ug_file_open(filePath.GetBytes(), openMode, ref fileID));
        }

        private void Close()
        {
            Invoke(() => UGrid.ug_file_close(fileID));
        }

        public string GetPath()
        {
            return filePath;
        }

        public T[] GetVariableByName<T>(string variableNameStr) where T : struct
        {
            var dimensionsCount = 0;
            var variableName = variableNameStr.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
            Invoke(() => UGrid.ug_variable_count_dimensions(fileID, variableName, ref dimensionsCount));

            var dimensionVec = new int[dimensionsCount];
            Invoke(() => UGrid.ug_variable_get_data_dimensions(fileID, variableName, dimensionVec));

            var totalDimension = 1;
            for (var i = 0; i < dimensionsCount; i++)
            {
                totalDimension *= dimensionVec[i];
            }

            var data = new T[totalDimension];

            switch (data)
            {
                case int[] intData:
                    Invoke(() => UGrid.ug_variable_get_data_int(fileID, variableName, intData));
                    break;
                case double[] doubleData:
                    Invoke(() => UGrid.ug_variable_get_data_double(fileID, variableName, doubleData));
                    break;
                case byte[] byteData:
                    Invoke(() => UGrid.ug_variable_get_data_char(fileID, variableName, byteData));
                    break;
                default:
                    throw new NotSupportedException("Currently only int, double and byte data types are supported.");
            }

            return data;
        }
    }
}