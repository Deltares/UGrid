using System;

namespace UGridNET
{
    namespace Extensions
    {
        internal static class Network1DExtensions
        {
            /// <summary> Allocates unmanaged memory for all required properties of the <see cref="Network1D"/> instance. </summary>
            /// <param name="network1D"> Instance of <see cref="Network1D"/>. </param>
            /// <remarks> Memory allocated by this method must be freed by <see cref="Free"> </remarks>
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

            /// <summary>
            /// Frees unmanaged memory allocated by <see cref="Allocate"/> for all required properties
            /// of the <see cref="Network1D"/> instance.
            /// </summary>
            /// <param name="network1D"> Instance of <see cref="Network1D"/>. </param>
            public static void Free(this Network1D network1D)
            {
                IntPtrHelpers.Free(() => network1D.name, value => network1D.name = value);
                IntPtrHelpers.Free(() => network1D.node_id, value => network1D.node_id = value);
                IntPtrHelpers.Free(() => network1D.node_long_name, value => network1D.node_long_name = value);
                IntPtrHelpers.Free(() => network1D.edge_id, value => network1D.edge_id = value);
                IntPtrHelpers.Free(() => network1D.edge_long_name, value => network1D.edge_long_name = value);
                IntPtrHelpers.Free(() => network1D.node_x, value => network1D.node_x = value);
                IntPtrHelpers.Free(() => network1D.node_y, value => network1D.node_y = value);
                IntPtrHelpers.Free(() => network1D.edge_nodes, value => network1D.edge_nodes = value);
                IntPtrHelpers.Free(() => network1D.edge_length, value => network1D.edge_length = value);
                IntPtrHelpers.Free(() => network1D.edge_order, value => network1D.edge_order = value);
                IntPtrHelpers.Free(() => network1D.geometry_nodes_x, value => network1D.geometry_nodes_x = value);
                IntPtrHelpers.Free(() => network1D.geometry_nodes_y, value => network1D.geometry_nodes_y = value);
                IntPtrHelpers.Free(() => network1D.num_edge_geometry_nodes, value => network1D.num_edge_geometry_nodes = value);
            }
        }
    }
}