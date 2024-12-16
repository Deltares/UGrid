using System;
using System.IO;



namespace UGridNET.Tests
{

    internal class TestUtilities
    {

        public static string GetTestDataPath()
        {
            string path = Environment.GetEnvironmentVariable("UGRID_TEST_DATA_DIR");

            // Check if the environment variable is set
            if (string.IsNullOrEmpty(path))
            {
                throw new InvalidOperationException("The UGRID_TEST_DATA_DIR environment variable is not set.");
            }

            // Check if the path is absolute and that it exists
            if (!(Path.IsPathRooted(path) && Directory.Exists(path)))
            {
                throw new InvalidOperationException(
                    $"The specified UGRID_TEST_DATA_DIR path must be absolute and should exist. UGRID_TEST_DATA_DIR={path}");
            }

            return path;

            //return "C:\\Users\\sayed\\work\\repositories\\UGrid\\tests\\data";
        }

    }

}