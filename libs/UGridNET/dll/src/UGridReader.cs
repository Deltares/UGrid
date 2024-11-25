using System;
using UGridNET.Extensions;

namespace UGridNET
{

    public sealed class UGridReader : UGridBase
    {
        // requires using log4net;
        //private static readonly ILog log = LogManager.GetLogger(typeof(UGridReader));

        private bool disposed = false;

        public UGridReader(string path) : base(path, 0)
        {
            try
            {
                ReadMesh1D();
                ReadMesh2D();
                ReadContacts();
                ReadNetwork1D();
            }
            catch
            {
                // If initialization fails, dispose of any partially initialized resources
                Dispose();
                // Rethrow to inform the caller of the failure
                throw;
            }
        }

        ~UGridReader()
        {
            // Cleanup in case Dispose wasn't called
            Dispose(false);
        }

        public new void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
            base.Dispose();
        }

        protected override void Dispose(bool disposing)
        {
            if (!disposed)
            {
                if (disposing)
                {
                    // clean up
                }

                disposed = true;

                base.Dispose(disposing);
            }
        }


        private void ReadMesh1D()
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

        private void ReadMesh2D()
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

        private void ReadContacts()
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
        private void ReadNetwork1D()
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

    }

}