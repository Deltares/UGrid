using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    internal static class IntPtrHelpers
    {
        internal static class Constants
        {
            public static readonly int intBytes = Marshal.SizeOf<int>();
            public static readonly int doubleBytes = Marshal.SizeOf<double>();
            public static readonly int byteBytes = Marshal.SizeOf<byte>();
        }
        public static IntPtr Allocate(int size)
        {
            return Marshal.AllocHGlobal(size);
        }

        public static void Free(IntPtr ptr)
        {
            if (ptr != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(ptr);
                ptr = IntPtr.Zero;
            }
        }
    }

}