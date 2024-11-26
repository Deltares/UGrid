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
                try
                {
                    mesh1D.name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length);
                    mesh1D.node_long_name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length * mesh1D.num_nodes);
                    mesh1D.network_name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length);
                    mesh1D.node_x = IntPtrHelpers.Allocate<double>(mesh1D.num_nodes);
                    mesh1D.node_y = IntPtrHelpers.Allocate<double>(mesh1D.num_nodes);
                    mesh1D.edge_x = IntPtrHelpers.Allocate<double>(mesh1D.num_edges);
                    mesh1D.edge_y = IntPtrHelpers.Allocate<double>(mesh1D.num_edges);
                    mesh1D.edge_nodes = IntPtrHelpers.Allocate<int>(mesh1D.num_edges * 2);
                    mesh1D.edge_edge_id = IntPtrHelpers.Allocate<int>(mesh1D.num_nodes); // size?
                    mesh1D.node_edge_id = IntPtrHelpers.Allocate<int>(mesh1D.num_nodes); // size?
                    mesh1D.node_edge_offset = IntPtrHelpers.Allocate<double>(mesh1D.num_nodes);
                }
                catch
                {
                    // AllocHGlobal may throw OutOfMemoryException exception, clean up and re-throw
                    mesh1D.Free();
                    throw;
                }
            }

            public static void Free(this Mesh1D mesh1D)
            {
                IntPtrHelpers.Free(mesh1D.name);
                IntPtrHelpers.Free(mesh1D.node_long_name);
                IntPtrHelpers.Free(mesh1D.network_name);
                IntPtrHelpers.Free(mesh1D.node_x);
                IntPtrHelpers.Free(mesh1D.node_y);
                IntPtrHelpers.Free(mesh1D.edge_x);
                IntPtrHelpers.Free(mesh1D.edge_y);
                IntPtrHelpers.Free(mesh1D.edge_nodes);
                IntPtrHelpers.Free(mesh1D.edge_edge_id);
                IntPtrHelpers.Free(mesh1D.node_edge_id);
                IntPtrHelpers.Free(mesh1D.node_edge_offset);
            }
        }
    }
}