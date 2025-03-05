using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;

namespace UGridNET
{
    public sealed class DisposableMesh1D : DisposableNativeObject<Mesh1D>
    {
        private byte[] name;

        private byte[] networkName;

        private int numNodes;

        private int numEdges;

        private double[] nodeX;

        private double[] nodeY;

        private int[] nodeEdgeID;

        private double[] nodeEdgeOffset;

        private double[] edgeX;

        private double[] edgeY;

        private int[] edgeNodes;

        
          

        public DisposableMesh1D() {}

        public DisposableMesh1D(int nNodes, int nEdges)
        {
            NumNodes = nNodes;
            NumEdges = nEdges;

            Name = new byte[UGrid.name_long_length];
            NetworkName = new byte[UGrid.name_long_length];

            NodeX = new double[NumNodes];
            NodeY = new double[NumNodes];
            NodeEdgeID = new int[NumNodes];
            NodeEdgeOffset = new double[NumNodes];

            EdgeX = new double[NumEdges];
            EdgeY = new double[NumEdges];
            EdgeNodes = new int[NumEdges * 2];
        }

        ~DisposableMesh1D()
        {
            Dispose(false);
        }

        public byte[] Name
        {
            get { return name; }
            set { name = value; }
        }

        public byte[] NetworkName
        {
            get { return networkName; }
            set { networkName = value; }
        }

        public int NumNodes
        {
            get { return numNodes; }
            set { numNodes = value; }
        }

        public int NumEdges
        {
            get { return numEdges; }
            set { numEdges = value; }
        }

        public double[] NodeX
        {
            get { return nodeX; }
            set { nodeX = value; }
        }

        public double[] NodeY
        {
            get { return nodeY; }
            set { nodeY = value; }
        }

        public int[] NodeEdgeID
        {
            get { return nodeEdgeID; }
            set { nodeEdgeID = value; }
        }

        public double[] NodeEdgeOffset
        {
            get { return nodeEdgeOffset; }
            set { nodeEdgeOffset = value; }
        }

        public double[] EdgeX
        {
            get { return edgeX; }
            set { edgeX = value; }
        }

        public double[] EdgeY
        {
            get { return edgeY; }
            set { edgeY = value; }
        }

        public int[] EdgeNodes
        {
            get { return edgeNodes; }
            set { edgeNodes = value; }
        }

        protected override void SetNativeObject(ref Mesh1D mesh2D)
        {
            mesh2D.name = GetPinnedObjectPointer(Name);
            mesh2D.network_name = GetPinnedObjectPointer(NetworkName);

            mesh2D.num_nodes = NumNodes;
            mesh2D.num_edges = NumEdges;

            mesh2D.node_x = GetPinnedObjectPointer(NodeX);
            mesh2D.node_y = GetPinnedObjectPointer(NodeY);
            mesh2D.node_edge_id = GetPinnedObjectPointer(NodeEdgeID);
            mesh2D.node_edge_offset = GetPinnedObjectPointer(NodeEdgeOffset);
            
            mesh2D.edge_x = GetPinnedObjectPointer(EdgeX);
            mesh2D.edge_y = GetPinnedObjectPointer(EdgeY);
            mesh2D.edge_nodes = GetPinnedObjectPointer(EdgeNodes);        
        }
    }
}