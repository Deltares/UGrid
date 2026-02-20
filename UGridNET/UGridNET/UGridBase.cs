using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using UGridNET.Extensions;

namespace UGridNET
{
    public class UGridBase : IDisposable
    {
        private readonly string filePath;
        private bool disposed;

        protected int FileID = -1;
        protected readonly List<Mesh1D> Meshes1D = new List<Mesh1D>();
        protected readonly List<Mesh2D> Meshes2D = new List<Mesh2D>();
        protected readonly List<Contacts> Contacts = new List<Contacts>();
        protected readonly List<Network1D> Networks1D = new List<Network1D>();

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
            DisposeAndClearLists(Meshes1D);
            DisposeAndClearLists(Meshes2D);
            DisposeAndClearLists(Contacts);
            DisposeAndClearLists(Networks1D);
        }

        private void FreeUnmanagedMemoryInTopologyLists()
        {
            if (fileMode == FileMode.Read)
            {
                Meshes1D.ForEach(item => item.Free());
                Meshes2D.ForEach(item => item.Free());
                Contacts.ForEach(item => item.Free());
                Networks1D.ForEach(item => item.Free());
            }
        }

        public bool HasMesh1D => Meshes1D.Count > 0;

        public bool HasMesh2D => Meshes2D.Count > 0;

        public bool HasContacts => Contacts.Count > 0;

        public bool HasNetwork1D => Networks1D.Count > 0;

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Mesh1D> Mesh1DList => Meshes1D.AsReadOnly();

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Mesh2D> Mesh2DList => Meshes2D.AsReadOnly();

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Contacts> ContactsList => Contacts.AsReadOnly();

        [ExcludeFromCodeCoverage]
        public ReadOnlyCollection<Network1D> Network1DList => Networks1D.AsReadOnly();

        protected FileMode FileMode1 => FileMode2;

        protected FileMode FileMode2 => fileMode;

        private static void ProcessExitCode(int exitCode)
        {
            if (exitCode != 0)
            {
                var messageBytes = new byte[UGrid.error_message_buffer_size];
                UGrid.ug_error_get(messageBytes);
                string messageStr = messageBytes.GetStringFromNullTerminatedArray(true);
                throw new UGridNETException(string.IsNullOrWhiteSpace(messageStr) ? "Unknown exception" : messageStr);
            }
        }

        protected static void Invoke(Func<int> func)
        {
            ProcessExitCode(func());
        }

        private void Open()
        {
            Invoke(() => UGrid.ug_file_open(filePath.GetBytes(), (int)fileMode, ref FileID));
        }

        private void Close()
        {
            if (FileID > 0)
            {
                Invoke(() => UGrid.ug_file_close(FileID));
            }
        }

        public string GetPath()
        {
            return filePath;
        }

        public T[] GetVariableByName<T>(string variableNameStr) where T : struct
        {
            var dimensionsCount = 0;
            byte[] variableName = variableNameStr.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
            Invoke(() => UGrid.ug_variable_count_dimensions(FileID, variableName, ref dimensionsCount));

            var dimensionVec = new int[dimensionsCount];
            Invoke(() => UGrid.ug_variable_get_data_dimensions(FileID, variableName, dimensionVec));

            var totalDimension = 1;
            for (var i = 0; i < dimensionsCount; i++)
            {
                totalDimension *= dimensionVec[i];
            }

            var data = new T[totalDimension];

            switch (data)
            {
                case int[] intData:
                    Invoke(() => UGrid.ug_variable_get_data_int(FileID, variableName, intData));
                    break;
                case double[] doubleData:
                    Invoke(() => UGrid.ug_variable_get_data_double(FileID, variableName, doubleData));
                    break;
                case byte[] byteData:
                    Invoke(() => UGrid.ug_variable_get_data_char(FileID, variableName, byteData));
                    break;
                default:
                    throw new NotSupportedException("Currently only int, double and byte data types are supported.");
            }

            return data;
        }
    }
}