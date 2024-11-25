using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    namespace Extensions
    {
        internal static class Network1DExtensions
        {
            public static void Allocate(this Network1D network1D)
            {
                network1D.name = Marshal.AllocHGlobal(UGrid.name_long_length);
                network1D.node_id = Marshal.AllocHGlobal(UGrid.name_length * network1D.num_nodes);
                network1D.node_long_name = Marshal.AllocHGlobal(UGrid.name_long_length * network1D.num_nodes);
                network1D.edge_id = Marshal.AllocHGlobal(UGrid.name_length * network1D.num_edges);
                network1D.edge_long_name = Marshal.AllocHGlobal(UGrid.name_long_length * network1D.num_edges);
                network1D.node_x = Marshal.AllocHGlobal(network1D.num_nodes * Constants.doubleBytes);
                network1D.node_y = Marshal.AllocHGlobal(network1D.num_nodes * Constants.doubleBytes);
                network1D.edge_nodes = Marshal.AllocHGlobal(network1D.num_edges * 2 * Constants.intBytes);
                network1D.edge_length = Marshal.AllocHGlobal(network1D.num_edges * Constants.doubleBytes);
                network1D.edge_order = Marshal.AllocHGlobal(network1D.num_edges * Constants.intBytes);
                network1D.geometry_nodes_x = Marshal.AllocHGlobal(network1D.num_geometry_nodes * Constants.doubleBytes);
                network1D.geometry_nodes_y = Marshal.AllocHGlobal(network1D.num_geometry_nodes * Constants.doubleBytes);
                network1D.num_edge_geometry_nodes = Marshal.AllocHGlobal(network1D.num_edges * Constants.intBytes);
            }

            public static void Free(this Network1D network1D)
            {
                if (network1D.name != IntPtr.Zero) Marshal.FreeHGlobal(network1D.name);
                if (network1D.node_id != IntPtr.Zero) Marshal.FreeHGlobal(network1D.node_id);
                if (network1D.node_long_name != IntPtr.Zero) Marshal.FreeHGlobal(network1D.node_long_name);
                if (network1D.edge_id != IntPtr.Zero) Marshal.FreeHGlobal(network1D.edge_id);
                if (network1D.edge_long_name != IntPtr.Zero) Marshal.FreeHGlobal(network1D.edge_long_name);
                if (network1D.node_x != IntPtr.Zero) Marshal.FreeHGlobal(network1D.node_x);
                if (network1D.node_y != IntPtr.Zero) Marshal.FreeHGlobal(network1D.node_y);
                if (network1D.edge_nodes != IntPtr.Zero) Marshal.FreeHGlobal(network1D.edge_nodes);
                if (network1D.edge_length != IntPtr.Zero) Marshal.FreeHGlobal(network1D.edge_length);
                if (network1D.edge_order != IntPtr.Zero) Marshal.FreeHGlobal(network1D.edge_order);
                if (network1D.geometry_nodes_x != IntPtr.Zero) Marshal.FreeHGlobal(network1D.geometry_nodes_x);
                if (network1D.geometry_nodes_y != IntPtr.Zero) Marshal.FreeHGlobal(network1D.geometry_nodes_y);
                if (network1D.num_edge_geometry_nodes != IntPtr.Zero) Marshal.FreeHGlobal(network1D.num_edge_geometry_nodes);
            }
        }
    }
}