using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    internal static class IntPtrHelpers
    {
        public static IntPtr Allocate<T>(int count) where T : struct
        {
            if (count <= 0)
            {
                throw new ArgumentOutOfRangeException(nameof(count), "Count must be greater than zero.");
            }

            return Marshal.AllocHGlobal(count * Marshal.SizeOf<T>());
        }

        public static void Free(IntPtr ptr)
        {
            if (ptr != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(ptr);
            }
        }
    }

}