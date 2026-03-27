namespace UGridNET
{
    /// <summary>
    /// Represents a list of geometries with their coordinates and associated values.
    /// </summary>
    public sealed class GeometryList
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="GeometryList"/> class.
        /// </summary>
        public GeometryList()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="GeometryList"/> class with the specified number of coordinates.
        /// </summary>
        /// <param name="numberOfCoordinates">The number of coordinates to allocate.</param>
        public GeometryList(int numberOfCoordinates)
        {
            NumberOfCoordinates = numberOfCoordinates;
            XCoordinates = new double[numberOfCoordinates];
            YCoordinates = new double[numberOfCoordinates];
            Values = new double[numberOfCoordinates];
        }

        /// <summary>
        /// Total number of coordinates.
        /// </summary>
        public int NumberOfCoordinates { get; set; }

        /// <summary>
        /// The X coordinates of the geometries.
        /// </summary>
        public double[] XCoordinates { get; set; }

        /// <summary>
        /// The Y coordinates of the geometries.
        /// </summary>
        public double[] YCoordinates { get; set; }

        /// <summary>
        /// The values for the coordinates of the geometries.
        /// </summary>
        public double[] Values { get; set; }
    }
}