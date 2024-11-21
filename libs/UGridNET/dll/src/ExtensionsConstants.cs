using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    namespace Extensions
    {
        internal static class Constants
        {
            public static readonly int intBytes = Marshal.SizeOf<int>();
            public static readonly int doubleBytes = Marshal.SizeOf<double>();
            public static readonly int byteBytes = Marshal.SizeOf<byte>();
        }
    }
}