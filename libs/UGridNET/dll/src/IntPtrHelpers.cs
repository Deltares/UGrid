using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    internal static class IntPtrHelpers
    {
        /// <summary> Allocates unmanaged memory for an array of elements of the specified type. </summary>
        /// <typeparam name="T"> The type of elements for which memory is being allocated. </typeparam>
        /// <param name="count">The number of elements to allocate memory for.</param>
        /// <returns> A pointer to the allocated unmanaged memory. </returns>
        /// <exception cref="ArgumentException"> Thrown if <paramref name="count"/> is less than or equal to zero. </exception>
        /// <remarks> The caller is responsible for freeing the memory using <see cref="Free"/>. </remarks>
        public static IntPtr Allocate<T>(int count) where T : struct
        {
            if (count <= 0)
            {
                throw new ArgumentException("Count must be greater than zero.", nameof(count));
            }
            return Marshal.AllocHGlobal(count * Marshal.SizeOf<T>());
        }


        /// <summary> Frees unmanaged memory previously allocated with <see cref="Allocate"/>. </summary>
        /// <param name="ptr"> A pointer to the unmanaged memory to free.</param>
        public static void Free(ref IntPtr ptr)
        {
            if (ptr == IntPtr.Zero) return;
            Marshal.FreeHGlobal(ptr);
            ptr = IntPtr.Zero;
        }

        /// <summary> Frees unmanaged memory previously allocated with <see cref="Allocate"/>. </summary>
        /// <param name="getPtr">A function that returns the pointer to the unmanaged memory block.</param>
        /// <param name="setPtr">An action that sets the pointer to <see cref="IntPtr.Zero"/> after freeing the memory.</param>
        /// <remarks>
        /// This overload is necessary for freeing IntPtr properties which cannot be passed by reference.
        /// Such properties are encountered often in SWIG-generated code
        /// It first retrieves the pointer using the <paramref name="getPtr"/> function. 
        /// If the pointer is not <see cref="IntPtr.Zero"/>, it frees the memory using <see cref="Marshal.FreeHGlobal"/> 
        /// and then sets the pointer to <see cref="IntPtr.Zero"/> using the <paramref name="setPtr"/> action.
        /// </remarks>
        public static void Free(Func<IntPtr> getPtr, Action<IntPtr> setPtr)
        {
            var ptr = getPtr();
            if (ptr == IntPtr.Zero) return;
            Marshal.FreeHGlobal(ptr);
            setPtr(IntPtr.Zero);
        }

    }

}