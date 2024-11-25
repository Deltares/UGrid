using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    namespace Extensions
    {
        internal static class Mesh2DExtensions
        {
            public static void Allocate(this Mesh2D mesh2D)
            {
                try
                {
                    mesh2D.name = IntPtrHelpers.Allocate(UGrid.name_long_length);
                    mesh2D.node_x = IntPtrHelpers.Allocate(mesh2D.num_nodes * IntPtrHelpers.Constants.doubleBytes);
                    mesh2D.node_y = IntPtrHelpers.Allocate(mesh2D.num_nodes * IntPtrHelpers.Constants.doubleBytes);
                    mesh2D.node_z = IntPtrHelpers.Allocate(mesh2D.num_nodes * IntPtrHelpers.Constants.doubleBytes);
                    mesh2D.edge_x = IntPtrHelpers.Allocate(mesh2D.num_edges * IntPtrHelpers.Constants.doubleBytes);
                    mesh2D.edge_y = IntPtrHelpers.Allocate(mesh2D.num_edges * IntPtrHelpers.Constants.doubleBytes);
                    mesh2D.edge_z = IntPtrHelpers.Allocate(mesh2D.num_edges * IntPtrHelpers.Constants.doubleBytes);
                    mesh2D.face_x = IntPtrHelpers.Allocate(mesh2D.num_faces * IntPtrHelpers.Constants.doubleBytes);
                    mesh2D.face_y = IntPtrHelpers.Allocate(mesh2D.num_faces * IntPtrHelpers.Constants.doubleBytes);
                    mesh2D.face_z = IntPtrHelpers.Allocate(mesh2D.num_faces * IntPtrHelpers.Constants.doubleBytes);
                    mesh2D.edge_nodes = IntPtrHelpers.Allocate(mesh2D.num_edges * 2 * IntPtrHelpers.Constants.intBytes);
                    mesh2D.edge_faces = IntPtrHelpers.Allocate(mesh2D.num_edges * 2 * IntPtrHelpers.Constants.intBytes);
                    mesh2D.face_nodes = IntPtrHelpers.Allocate(mesh2D.num_faces * mesh2D.num_face_nodes_max * IntPtrHelpers.Constants.intBytes);
                    mesh2D.face_edges = IntPtrHelpers.Allocate(mesh2D.num_faces * mesh2D.num_face_nodes_max * IntPtrHelpers.Constants.intBytes);
                    mesh2D.face_faces = IntPtrHelpers.Allocate(mesh2D.num_faces * mesh2D.num_face_nodes_max * IntPtrHelpers.Constants.intBytes);
                }
                catch
                {
                    // AllocHGlobal may throw OutOfMemoryException exception, clean up and re-throw
                    mesh2D.Free();
                    throw;
                }
            }

            public static void Free(this Mesh2D mesh2D)
            {
                IntPtrHelpers.Free(mesh2D.name);
                IntPtrHelpers.Free(mesh2D.node_x);
                IntPtrHelpers.Free(mesh2D.node_y);
                IntPtrHelpers.Free(mesh2D.node_z);
                IntPtrHelpers.Free(mesh2D.edge_x);
                IntPtrHelpers.Free(mesh2D.edge_y);
                IntPtrHelpers.Free(mesh2D.edge_z);
                IntPtrHelpers.Free(mesh2D.face_x);
                IntPtrHelpers.Free(mesh2D.face_y);
                IntPtrHelpers.Free(mesh2D.face_z);
                IntPtrHelpers.Free(mesh2D.edge_nodes);
                IntPtrHelpers.Free(mesh2D.edge_faces);
                IntPtrHelpers.Free(mesh2D.face_nodes);
                IntPtrHelpers.Free(mesh2D.face_edges);
                IntPtrHelpers.Free(mesh2D.face_faces);
            }
        }
    }
}