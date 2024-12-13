namespace UGridNET
{
    namespace Extensions
    {
        internal static class Mesh2DExtensions
        {
            /// <summary> Allocates unmanaged memory for all required properties of the <see cref="Mesh2D"/> instance. </summary>
            /// <param name="mesh2D"> Instance of <see cref="Mesh2D"/>. </param>
            /// <remarks> Memory allocated by this method must be freed by <see cref="Free"> </remarks>
            public static void Allocate(this Mesh2D mesh2D)
            {
                try
                {
                    mesh2D.name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length);
                    mesh2D.node_x = IntPtrHelpers.Allocate<double>(mesh2D.num_nodes);
                    mesh2D.node_y = IntPtrHelpers.Allocate<double>(mesh2D.num_nodes);
                    mesh2D.node_z = IntPtrHelpers.Allocate<double>(mesh2D.num_nodes);
                    mesh2D.edge_x = IntPtrHelpers.Allocate<double>(mesh2D.num_edges);
                    mesh2D.edge_y = IntPtrHelpers.Allocate<double>(mesh2D.num_edges);
                    mesh2D.edge_z = IntPtrHelpers.Allocate<double>(mesh2D.num_edges);
                    mesh2D.face_x = IntPtrHelpers.Allocate<double>(mesh2D.num_faces);
                    mesh2D.face_y = IntPtrHelpers.Allocate<double>(mesh2D.num_faces);
                    mesh2D.face_z = IntPtrHelpers.Allocate<double>(mesh2D.num_faces);
                    mesh2D.edge_nodes = IntPtrHelpers.Allocate<int>(mesh2D.num_edges * 2);
                    mesh2D.edge_faces = IntPtrHelpers.Allocate<int>(mesh2D.num_edges * 2);
                    mesh2D.face_nodes = IntPtrHelpers.Allocate<int>(mesh2D.num_faces * mesh2D.num_face_nodes_max);
                    mesh2D.face_edges = IntPtrHelpers.Allocate<int>(mesh2D.num_faces * mesh2D.num_face_nodes_max);
                    mesh2D.face_faces = IntPtrHelpers.Allocate<int>(mesh2D.num_faces * mesh2D.num_face_nodes_max);
                }
                catch
                {
                    // AllocHGlobal may throw OutOfMemoryException exception, clean up and re-throw
                    mesh2D.Free();
                    throw;
                }
            }

            /// <summary> Frees unmanaged memory allocated by <see cref="Allocate"/> for all required properties of the <see cref="Mesh2D"/> instance. </summary>
            /// <param name="mesh2D"> Instance of <see cref="Mesh2D"/>. </param>
            public static void Free(this Mesh2D mesh2D)
            {
                IntPtrHelpers.Free(() => mesh2D.name, value => mesh2D.name = value);
                IntPtrHelpers.Free(() => mesh2D.node_x, value => mesh2D.node_x = value);
                IntPtrHelpers.Free(() => mesh2D.node_y, value => mesh2D.node_y = value);
                IntPtrHelpers.Free(() => mesh2D.node_z, value => mesh2D.node_z = value);
                IntPtrHelpers.Free(() => mesh2D.edge_x, value => mesh2D.edge_x = value);
                IntPtrHelpers.Free(() => mesh2D.edge_y, value => mesh2D.edge_y = value);
                IntPtrHelpers.Free(() => mesh2D.edge_z, value => mesh2D.edge_z = value);
                IntPtrHelpers.Free(() => mesh2D.face_x, value => mesh2D.face_x = value);
                IntPtrHelpers.Free(() => mesh2D.face_y, value => mesh2D.face_y = value);
                IntPtrHelpers.Free(() => mesh2D.face_z, value => mesh2D.face_z = value);
                IntPtrHelpers.Free(() => mesh2D.edge_nodes, value => mesh2D.edge_nodes = value);
                IntPtrHelpers.Free(() => mesh2D.edge_faces, value => mesh2D.edge_faces = value);
                IntPtrHelpers.Free(() => mesh2D.face_nodes, value => mesh2D.face_nodes = value);
                IntPtrHelpers.Free(() => mesh2D.face_edges, value => mesh2D.face_edges = value);
                IntPtrHelpers.Free(() => mesh2D.face_faces, value => mesh2D.face_faces = value);
            }
        }
    }
}