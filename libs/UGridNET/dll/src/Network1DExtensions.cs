using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    namespace Extensions
    {
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
}