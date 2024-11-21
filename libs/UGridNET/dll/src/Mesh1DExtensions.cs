using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    namespace Extensions
    {
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
    }
}