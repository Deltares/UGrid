namespace UGridNET
{
    /// <summary>
    /// Represents a projected coordinate system.
    /// </summary>
    public sealed class ProjectedCoordinateSystem
    {
        // integers
        private int epsg;

        // doubles
        private double longitudeOfPrimeMeridian;
        private double semiMajorAxis;
        private double semiMinorAxis;
        private double inverseFlattening;

        // strings
        private string name = "";
        private string gridMappingName = "";
        private string proj4Params = "";
        private string epsgCode = "";
        private string projectionName = "";
        private string wkt = "";

        public ProjectedCoordinateSystem()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="ProjectedCoordinateSystem"/> class.
        /// </summary>
        /// <param name="epsg">The EPSG code.</param>
        /// <param name="longitudeOfPrimeMeridian">The longitude of the prime meridian.</param>
        /// <param name="semiMajorAxis">The semi-major axis.</param>
        /// <param name="semiMinorAxis">The semi-minor axis.</param>
        /// <param name="inverseFlattening">The inverse flattening.</param>
        /// <param name="name">The name.</param>
        /// <param name="gridMappingName">The grid mapping name.</param>
        /// <param name="proj4Params">The Proj4 parameters.</param>
        /// <param name="epsgCode">The EPSG code as a string.</param>
        /// <param name="projectionName">The name of the projection.</param>
        /// <param name="wkt">The Well-Known Text (WKT) representation.</param>
        public ProjectedCoordinateSystem(
            int epsg,
            double longitudeOfPrimeMeridian,
            double semiMajorAxis,
            double semiMinorAxis,
            double inverseFlattening,
            string name,
            string gridMappingName,
            string proj4Params,
            string epsgCode,
            string projectionName,
            string wkt)
        {
            this.epsg = epsg;
            this.longitudeOfPrimeMeridian = longitudeOfPrimeMeridian;
            this.semiMajorAxis = semiMajorAxis;
            this.semiMinorAxis = semiMinorAxis;
            this.inverseFlattening = inverseFlattening;
            this.name = name;
            this.gridMappingName = gridMappingName;
            this.proj4Params = proj4Params;
            this.epsgCode = epsgCode;
            this.projectionName = projectionName;
            this.wkt = wkt;
        }

        /// <summary>
        /// Gets or sets the EPSG code.
        /// </summary>
        public int EPSG
        {
            get => epsg;
            set => epsg = value;
        }

        /// <summary>
        /// Gets or sets the longitude of the prime meridian.
        /// </summary>
        public double LongitudeOfPrimeMeridian
        {
            get => longitudeOfPrimeMeridian;
            set => longitudeOfPrimeMeridian = value;
        }

        /// <summary>
        /// Gets or sets the semi-major axis.
        /// </summary>
        public double SemiMajorAxis
        {
            get => semiMajorAxis;
            set => semiMajorAxis = value;
        }

        /// <summary>
        /// Gets or sets the semi-minor axis.
        /// </summary>
        public double SemiMinorAxis
        {
            get => semiMinorAxis;
            set => semiMinorAxis = value;
        }

        /// <summary>
        /// Gets or sets the inverse flattening.
        /// </summary>
        public double InverseFlattening
        {
            get => inverseFlattening;
            set => inverseFlattening = value;
        }

        /// <summary>
        /// Gets or sets the name.
        /// </summary>
        public string Name
        {
            get => name;
            set => name = value;
        }

        /// <summary>
        /// Gets or sets the grid mapping name.
        /// </summary>
        public string GridMappingName
        {
            get => gridMappingName;
            set => gridMappingName = value;
        }

        /// <summary>
        /// Gets or sets the Proj4 parameters.
        /// </summary>
        public string Proj4Params
        {
            get => proj4Params;
            set => proj4Params = value;
        }

        /// <summary>
        /// Gets or sets the EPSG code as a string.
        /// </summary>
        public string EPSGCode
        {
            get => epsgCode;
            set => epsgCode = value;
        }

        /// <summary>
        /// Gets or sets the projection name.
        /// </summary>
        public string ProjectionName
        {
            get => projectionName;
            set => projectionName = value;
        }

        /// <summary>
        /// Gets or sets the Well-Known Text (WKT) representation.
        /// </summary>
        public string WKT
        {
            get => wkt;
            set => wkt = value;
        }
    }
}