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
                try
                {
                    network1D.name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length);
                    network1D.node_id = IntPtrHelpers.Allocate<byte>(UGrid.name_length * network1D.num_nodes);
                    network1D.node_long_name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length * network1D.num_nodes);
                    network1D.edge_id = IntPtrHelpers.Allocate<byte>(UGrid.name_length * network1D.num_edges);
                    network1D.edge_long_name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length * network1D.num_edges);
                    network1D.node_x = IntPtrHelpers.Allocate<double>(network1D.num_nodes);
                    network1D.node_y = IntPtrHelpers.Allocate<double>(network1D.num_nodes);
                    network1D.edge_nodes = IntPtrHelpers.Allocate<int>(network1D.num_edges * 2);
                    network1D.edge_length = IntPtrHelpers.Allocate<double>(network1D.num_edges);
                    network1D.edge_order = IntPtrHelpers.Allocate<int>(network1D.num_edges);
                    network1D.geometry_nodes_x = IntPtrHelpers.Allocate<double>(network1D.num_geometry_nodes);
                    network1D.geometry_nodes_y = IntPtrHelpers.Allocate<double>(network1D.num_geometry_nodes);
                    network1D.num_edge_geometry_nodes = IntPtrHelpers.Allocate<int>(network1D.num_edges);
                }
                catch
                {
                    // AllocHGlobal may throw OutOfMemoryException exception, clean up and re-throw
                    network1D.Free();
                    throw;
                }
            }

            public static void Free(this Network1D network1D)
            {
                IntPtrHelpers.Free(network1D.name);
                IntPtrHelpers.Free(network1D.node_id);
                IntPtrHelpers.Free(network1D.node_long_name);
                IntPtrHelpers.Free(network1D.edge_id);
                IntPtrHelpers.Free(network1D.edge_long_name);
                IntPtrHelpers.Free(network1D.node_x);
                IntPtrHelpers.Free(network1D.node_y);
                IntPtrHelpers.Free(network1D.edge_nodes);
                IntPtrHelpers.Free(network1D.edge_length);
                IntPtrHelpers.Free(network1D.edge_order);
                IntPtrHelpers.Free(network1D.geometry_nodes_x);
                IntPtrHelpers.Free(network1D.geometry_nodes_y);
                IntPtrHelpers.Free(network1D.num_edge_geometry_nodes);
            }
        }
    }
}