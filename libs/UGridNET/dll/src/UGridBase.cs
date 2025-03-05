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

        protected enum FileMode
        {
            /// <summary>
            /// Opens an existing file for reading.
            /// <para>The value is equal to NcFile::FileMode::read. The file must exist; otherwise, an error occurs.</para>
            /// </summary>
            Read = 0,

            /// <summary>
            /// Opens a file for writing. If the file exists, its contents are overwritten.
            /// <para>The value is equal to NcFile::FileMode::replace. If the file does not exist, a new file is created.</para>
            /// </summary>
            Write = 2
        }

        private readonly FileMode fileMode;

        private UGridBase(){}

        protected UGridBase(string path, FileMode mode)
        {
            try
            {
                filePath = Path.GetFullPath(path);
                fileMode = mode;
                Open();
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
            if(fileMode == FileMode.Read)
            {
                mesh1DList.ForEach(item => item.Free());
                mesh2DList.ForEach(item => item.Free());
                contactsList.ForEach(item => item.Free());
                network1DList.ForEach(item => item.Free());
            }
        }

        public bool HasMesh1D => mesh1DList.Count > 0;

        public bool HasMesh2D => mesh2DList.Count > 0;

        public bool HasContacts => contactsList.Count > 0;

        public bool HasNetwork1D => network1DList.Count > 0;

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Mesh1D> Mesh1DList => mesh1DList.AsReadOnly();

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Mesh2D> Mesh2DList => mesh2DList.AsReadOnly();

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Contacts> ContactsList => contactsList.AsReadOnly();

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Network1D> Network1DList => network1DList.AsReadOnly();

        protected FileMode FileMode1 => FileMode2;

        protected FileMode FileMode2 => fileMode;

        private static void ProcessExitCode(int exitCode)
        {
            if (exitCode != 0)
            {
                var messageBytes = new byte[UGrid.error_message_buffer_size];
                UGrid.ug_error_get(messageBytes);
                var messageStr = messageBytes.GetStringFromNullTerminatedArray(true);
                throw new UGridNETException(string.IsNullOrWhiteSpace(messageStr) ? "Unknown exception" : messageStr);
            }
        }

        protected static void Invoke(Func<int> func)
        {
            ProcessExitCode(func());
        }


        private void Open()
        {
            Invoke(() => UGrid.ug_file_open(filePath.GetBytes(), (int)fileMode, ref fileID));
        }

        private void Close()
        {
            if (fileID > 0) {
               Invoke(() => UGrid.ug_file_close(fileID));
            }
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