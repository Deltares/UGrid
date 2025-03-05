using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;

namespace UGridNET
{
    public sealed class DisposableMesh2D : DisposableNativeObject<Mesh2D>
    {
        private byte[] name;

        private int numNodes;

        private int numEdges;

        private int numFaces;

        private int numFacesNodesMax;

        private double[] nodeX;

        private double[] nodeY;

        private double[] nodeZ;

        private double[] edgeX;

        private double[] edgeY;

        private double[] edgeZ;

        private int[] edgeNodes;

        private int[] edgeFaces;

        private double[] faceX;

        private double[] faceY;

        private double[] faceZ;

        private int[] faceNodes;

        private int[] faceEdges;

        private int[] faceFaces;
          

        public DisposableMesh2D() {}

        public DisposableMesh2D(int nNodes, int nEdges, int nFaces, int nFacesNodesMax)
        {
            NumNodes = nNodes;
            NumEdges = nEdges;
            NumFaces = nFaces;
            NumFaceNodesMax = nFacesNodesMax;

            Name = new byte[UGrid.name_long_length];
            NodeX = new double[NumNodes];
            NodeY = new double[NumNodes];
            //NodeZ = new double[NumNodes];

            EdgeX = new double[NumEdges];
            EdgeY = new double[NumEdges];
            //EdgeZ = new double[NumEdges];
            EdgeNodes = new int[NumEdges * 2];
            //EdgeFaces = new int[NumEdges * 2];
                        
            FaceX = new double[NumFaces];
            FaceY = new double[NumFaces];
            //FaceZ = new double[NumFaces];
            FaceNodes = new int[NumFaces * NumFaceNodesMax];
            //FaceEdges = new int[NumFaces * NumFacesNodesMax];
            //FaceFaces = new int[NumFaces * NumFacesNodesMax];
        }

        ~DisposableMesh2D()
        {
            Dispose(false);
        }

        public byte[] Name
        {
            get { return name; }
            set { name = value; }
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

        public int NumFaces
        {
            get { return numFaces; }
            set { numFaces = value; }
        }

        public int NumFaceNodesMax
        {
            get { return numFacesNodesMax; }
            set { numFacesNodesMax = value; }
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

        public double[] NodeZ
        {
            get { return nodeZ; }
            set { nodeZ = value; }
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

        public double[] EdgeZ
        {
            get { return edgeZ; }
            set { edgeZ = value; }
        }
        public int[] EdgeNodes
        {
            get { return edgeNodes; }
            set { edgeNodes = value; }
        }

        public int[] EdgeFaces
        {
            get { return edgeFaces; }
            set { edgeFaces = value; }
        }

        public double[] FaceX
        {
            get { return faceX; }
            set { faceX = value; }
        }

        public double[] FaceY
        {
            get { return faceY; }
            set { faceY = value; }
        }

        public double[] FaceZ
        {
            get { return faceZ; }
            set { faceZ = value; }
        }

        public int[] FaceNodes
        {
            get { return faceNodes; }
            set { faceNodes = value; }
        }

        public int[] FaceEdges
        {
            get { return faceEdges; }
            set { faceEdges = value; }
        }

        public int[] FaceFaces
        {
            get { return faceFaces; }
            set { faceFaces = value; }
        }
       
        protected override void SetNativeObject(ref Mesh2D mesh2D)
        {

            mesh2D.name = GetPinnedObjectPointer(Name);

            mesh2D.num_nodes = NumNodes;
            mesh2D.num_edges = NumEdges;
            mesh2D.num_faces = NumFaces;
            mesh2D.num_face_nodes_max = NumFaceNodesMax;

            mesh2D.node_x = GetPinnedObjectPointer(NodeX);
            mesh2D.node_y = GetPinnedObjectPointer(NodeY);
            mesh2D.node_z = GetPinnedObjectPointer(NodeZ);

            mesh2D.edge_x = GetPinnedObjectPointer(EdgeX);
            mesh2D.edge_y = GetPinnedObjectPointer(EdgeY);
            mesh2D.edge_z = GetPinnedObjectPointer(EdgeZ);
            mesh2D.edge_nodes = GetPinnedObjectPointer(EdgeNodes);
            mesh2D.edge_faces = GetPinnedObjectPointer(EdgeFaces);

            mesh2D.face_x = GetPinnedObjectPointer(FaceX);
            mesh2D.face_y = GetPinnedObjectPointer(FaceY);
            mesh2D.face_z = GetPinnedObjectPointer(FaceZ);
            mesh2D.face_nodes = GetPinnedObjectPointer(FaceNodes);           
            mesh2D.face_edges = GetPinnedObjectPointer(FaceEdges);
            mesh2D.face_faces = GetPinnedObjectPointer(FaceFaces);            
        }
    }
}