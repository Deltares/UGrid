using System;

namespace UGridNET
{
    namespace Extensions
    {
        internal static class Mesh1DExtensions
        {
            /// <summary> Allocates unmanaged memory for all required properties of the <see cref="Mesh1D"/> instance. </summary>
            /// <param name="mesh1D"> Instance of <see cref="Mesh1D"/>. </param>
            /// <remarks> Memory allocated by this method must be freed by <see cref="Free"> </remarks>
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
                    mesh1D.edge_edge_id = IntPtrHelpers.Allocate<int>(mesh1D.num_edges);
                    mesh1D.node_edge_id = IntPtrHelpers.Allocate<int>(mesh1D.num_nodes);
                    mesh1D.node_edge_offset = IntPtrHelpers.Allocate<double>(mesh1D.num_nodes);
                }
                catch
                {
                    // AllocHGlobal may throw OutOfMemoryException exception, clean up and re-throw
                    mesh1D.Free();
                    throw;

                }
            }

            /// <summary> 
            /// Frees unmanaged memory allocated by <see cref="Allocate"/> for all required properties
            /// of the <see cref="Mesh1D"/> instance.
            /// </summary>
            /// <param name="mesh1D"> Instance of <see cref="Mesh1D"/>. </param>
            public static void Free(this Mesh1D mesh1D)
            {
                IntPtrHelpers.Free(() => mesh1D.name, value => mesh1D.name = value);
                IntPtrHelpers.Free(() => mesh1D.node_long_name, value => mesh1D.node_long_name = value);
                IntPtrHelpers.Free(() => mesh1D.network_name, value => mesh1D.network_name = value);
                IntPtrHelpers.Free(() => mesh1D.node_x, value => mesh1D.node_x = value);
                IntPtrHelpers.Free(() => mesh1D.node_y, value => mesh1D.node_y = value);
                IntPtrHelpers.Free(() => mesh1D.edge_x, value => mesh1D.edge_x = value);
                IntPtrHelpers.Free(() => mesh1D.edge_y, value => mesh1D.edge_y = value);
                IntPtrHelpers.Free(() => mesh1D.edge_nodes, value => mesh1D.edge_nodes = value);
                IntPtrHelpers.Free(() => mesh1D.edge_edge_id, value => mesh1D.edge_edge_id = value);
                IntPtrHelpers.Free(() => mesh1D.node_edge_id, value => mesh1D.node_edge_id = value);
                IntPtrHelpers.Free(() => mesh1D.node_edge_offset, value => mesh1D.node_edge_offset = value);
            }
        }
    }
}