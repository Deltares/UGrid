using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    internal static class IntPtrHelpers
    {
        /// <summary> Allocates unmanaged memory for an array of elements of the specified type. </summary>
        /// <typeparam name="T"> The type of elements for which memory is being allocated. </typeparam>
        /// <param name="count">The number of elements to allocate memory for.</param>
        /// <returns> A pointer to the allocated unmanaged memory. The caller is responsible for freeing the memory using <see cref="Free"/>. </returns>
        /// <exception cref="ArgumentException"> Thrown if <paramref name="count"/> is less than or equal to zero. </exception>
        public static IntPtr Allocate<T>(int count) where T : struct
        {
            if (count <= 0)
            {
                throw new ArgumentException("Count must be greater than zero.", nameof(count));
            }
            int size = count * Marshal.SizeOf<T>();
            return Marshal.AllocHGlobal(size);
        }


        /// <summary> Frees unmanaged memory previously allocated with <see cref="Allocate"/>. </summary>
        /// <param name="ptr"> A pointer to the unmanaged memory to free.</param>
        public static void Free(IntPtr ptr)
        {
            if (ptr != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(ptr);
            }
        }
    }

}