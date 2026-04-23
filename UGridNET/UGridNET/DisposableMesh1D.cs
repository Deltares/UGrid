namespace UGridNET
{
    public sealed class DisposableMesh1D : DisposableNativeObject<Mesh1D>
    {
        private byte[] name;
        private byte[] networkName;
        private byte[] gridMapping;
        private bool isSpherical;
        private int numNodes;
        private int numEdges;
        private double[] nodeX;
        private double[] nodeY;
        private int[] nodeEdgeID;
        private double[] nodeEdgeOffset;
        private double[] edgeX;
        private double[] edgeY;
        private int[] edgeNodes;

        public DisposableMesh1D()
        {
        }

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
            get => name;
            set => name = value;
        }

        public byte[] NetworkName
        {
            get => networkName;
            set => networkName = value;
        }

        public byte[] GridMapping
        {
            get => gridMapping;
            set => gridMapping = value;
        }
        
        public bool IsSpherical
        {
            get => isSpherical;
            set => isSpherical = value;
        }

        public int NumNodes
        {
            get => numNodes;
            set => numNodes = value;
        }

        public int NumEdges
        {
            get => numEdges;
            set => numEdges = value;
        }

        public double[] NodeX
        {
            get => nodeX;
            set => nodeX = value;
        }

        public double[] NodeY
        {
            get => nodeY;
            set => nodeY = value;
        }

        public int[] NodeEdgeID
        {
            get => nodeEdgeID;
            set => nodeEdgeID = value;
        }

        public double[] NodeEdgeOffset
        {
            get => nodeEdgeOffset;
            set => nodeEdgeOffset = value;
        }

        public double[] EdgeX
        {
            get => edgeX;
            set => edgeX = value;
        }

        public double[] EdgeY
        {
            get => edgeY;
            set => edgeY = value;
        }

        public int[] EdgeNodes
        {
            get => edgeNodes;
            set => edgeNodes = value;
        }

        protected override void SetNativeObject(ref Mesh1D mesh1D)
        {
            mesh1D.name = GetPinnedObjectPointer(Name);
            mesh1D.network_name = GetPinnedObjectPointer(NetworkName);
            mesh1D.grid_mapping = GetPinnedObjectPointer(GridMapping);
            
            mesh1D.is_spherical = IsSpherical ? 1 : 0;

            mesh1D.num_nodes = NumNodes;
            mesh1D.num_edges = NumEdges;

            mesh1D.node_x = GetPinnedObjectPointer(NodeX);
            mesh1D.node_y = GetPinnedObjectPointer(NodeY);
            mesh1D.node_edge_id = GetPinnedObjectPointer(NodeEdgeID);
            mesh1D.node_edge_offset = GetPinnedObjectPointer(NodeEdgeOffset);

            mesh1D.edge_x = GetPinnedObjectPointer(EdgeX);
            mesh1D.edge_y = GetPinnedObjectPointer(EdgeY);
            mesh1D.edge_nodes = GetPinnedObjectPointer(EdgeNodes);
        }
    }
}