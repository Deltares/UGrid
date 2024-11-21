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
                mesh2D.name = Marshal.AllocHGlobal(UGrid.name_long_length);
                mesh2D.node_x = Marshal.AllocHGlobal(mesh2D.num_nodes * Constants.doubleBytes);
                mesh2D.node_y = Marshal.AllocHGlobal(mesh2D.num_nodes * Constants.doubleBytes);
                mesh2D.node_z = Marshal.AllocHGlobal(mesh2D.num_nodes * Constants.doubleBytes);
                mesh2D.edge_x = Marshal.AllocHGlobal(mesh2D.num_edges * Constants.doubleBytes);
                mesh2D.edge_y = Marshal.AllocHGlobal(mesh2D.num_edges * Constants.doubleBytes);
                mesh2D.edge_z = Marshal.AllocHGlobal(mesh2D.num_edges * Constants.doubleBytes);
                mesh2D.face_x = Marshal.AllocHGlobal(mesh2D.num_faces * Constants.doubleBytes);
                mesh2D.face_y = Marshal.AllocHGlobal(mesh2D.num_faces * Constants.doubleBytes);
                mesh2D.face_z = Marshal.AllocHGlobal(mesh2D.num_faces * Constants.doubleBytes);
                mesh2D.edge_nodes = Marshal.AllocHGlobal(mesh2D.num_edges * 2 * Constants.intBytes);
                mesh2D.edge_faces = Marshal.AllocHGlobal(mesh2D.num_edges * 2 * Constants.intBytes);
                mesh2D.face_nodes = Marshal.AllocHGlobal(mesh2D.num_faces * mesh2D.num_face_nodes_max * Constants.intBytes);
                mesh2D.face_edges = Marshal.AllocHGlobal(mesh2D.num_faces * mesh2D.num_face_nodes_max * Constants.intBytes);
                mesh2D.face_faces = Marshal.AllocHGlobal(mesh2D.num_faces * mesh2D.num_face_nodes_max * Constants.intBytes);
            }

            public static void Free(this Mesh2D mesh2D)
            {
                if (mesh2D.name != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.name);
                if (mesh2D.node_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.node_x);
                if (mesh2D.node_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.node_y);
                if (mesh2D.node_z != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.node_z);
                if (mesh2D.edge_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_x);
                if (mesh2D.edge_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_y);
                if (mesh2D.edge_z != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_z);
                if (mesh2D.face_x != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_x);
                if (mesh2D.face_y != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_y);
                if (mesh2D.face_z != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_z);
                if (mesh2D.edge_nodes != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_nodes);
                if (mesh2D.edge_faces != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.edge_faces);
                if (mesh2D.face_nodes != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_nodes);
                if (mesh2D.face_edges != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_edges);
                if (mesh2D.face_faces != IntPtr.Zero) Marshal.FreeHGlobal(mesh2D.face_faces);
            }
        }
    }
}