namespace UGridNET
{
    /// <summary>
    /// Represents a coordinate system.
    /// </summary>
    public sealed class CoordinateSystem
    {
        /// <summary>
        /// Gets or sets the coordinate system name.
        /// </summary>
        public string Name { get; set; }
        
        /// <summary>
        /// Gets or sets the variable name.
        /// </summary>
        public string VariableName { get; set; }
        
        /// <summary>
        /// Gets or sets whether this is a spherical coordinate system.
        /// </summary>
        public bool IsSpherical { get; set; }
        
        /// <summary>
        /// Gets or sets the EPSG code.
        /// </summary>
        public int EPSG { get; set; }
        
        /// <summary>
        /// Gets or sets the grid mapping name.
        /// </summary>
        public string GridMappingName { get; set; }

        /// <summary>
        /// Gets or sets the longitude of the prime meridian.
        /// </summary>
        public double LongitudeOfPrimeMeridian { get; set; }

        /// <summary>
        /// Gets or sets the semi-major axis.
        /// </summary>
        public double SemiMajorAxis { get; set; }

        /// <summary>
        /// Gets or sets the semi-minor axis.
        /// </summary>
        public double SemiMinorAxis { get; set; }

        /// <summary>
        /// Gets or sets the inverse flattening.
        /// </summary>
        public double InverseFlattening { get; set; }

        /// <summary>
        /// Gets or sets the EPSG code as a string.
        /// </summary>
        public string EPSGCode { get; set; }
    }
}