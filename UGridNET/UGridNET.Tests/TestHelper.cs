using System;
using System.IO;
using System.Reflection;

namespace UGridNET.Tests
{
    internal static class TestHelper
    {
        private static string testDataPath;

        public static string GetTestFilePath(string filename)
        {
            return Path.Combine(GetTestDataPath(), filename);
        }

        private static string GetTestDataPath()
        {
            if (testDataPath != null)
            {
                return testDataPath;
            }

            string assemblyPath = Assembly.GetExecutingAssembly().Location;
            string assemblyDir = Path.GetDirectoryName(assemblyPath);

            if (string.IsNullOrEmpty(assemblyDir))
            {
                throw new InvalidOperationException("Cannot determine test assembly location.");
            }

            testDataPath = Path.Combine(assemblyDir, "TestData");

            if (!Directory.Exists(testDataPath))
            {
                throw new DirectoryNotFoundException($"TestData not found. Expected: '{testDataPath}'.");
            }

            return testDataPath;
        }
    }
}