using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    namespace Extensions
    {
        /// <summary>
        /// Provides extension methods for working with <see cref="IntPtr"/>.
        /// </summary>
        public static class IntPtrExtensions
        {
            /// <summary>
            /// Copies data from an unmanaged memory pointer to a managed array.
            /// </summary>
            /// <typeparam name="T">The type of elements in the array. Must be a value type.</typeparam>
            /// <param name="pointer">The unmanaged memory pointer.</param>
            /// <param name="length">The number of elements to copy.</param>
            /// <returns>A managed array containing the copied data.</returns>
            /// <exception cref="ArgumentNullException">Thrown when the pointer is null.</exception>
            /// <exception cref="ArgumentException">Thrown when the length is less than or equal to zero.</exception>
            /// <exception cref="NotSupportedException">Thrown when the type of elements is not supported.</exception>
            public static T[] CopyToArray<T>(this IntPtr pointer, int length) where T : struct
            {
                if (pointer == IntPtr.Zero)
                {
                    throw new ArgumentNullException(nameof(pointer), "Pointer is null. Cannot copy a null pointer to a array.");
                }

                if (length <= 0)
                {
                    throw new ArgumentException("Array length must be greater than zero.", nameof(length));
                }

                var array = new T[length];

                switch (array)
                {
                    case int[] intArray:
                        Marshal.Copy(pointer, intArray, 0, intArray.Length);
                        break;
                    case double[] doubleArray:
                        Marshal.Copy(pointer, doubleArray, 0, doubleArray.Length);
                        break;
                    case byte[] byteArray:
                        Marshal.Copy(pointer, byteArray, 0, byteArray.Length);
                        break;
                    default:
                        throw new NotSupportedException("Currently only int, double and byte data types are supported.");
                }

                return array;
            }

            /// <summary>
            /// Copies data from a managed array to an unmanaged memory pointer.
            /// </summary>
            /// <typeparam name="T">The type of elements in the array. Must be a value type.</typeparam>
            /// <param name="pointer">The unmanaged memory pointer.</param>
            /// <param name="array">The managed array containing the data to copy.</param>
            /// <exception cref="ArgumentNullException">Thrown when the pointer or array is null.</exception>
            /// <exception cref="ArgumentException">Thrown when the array is empty.</exception>
            /// <exception cref="NotSupportedException">Thrown when the type of elements is not supported.</exception>
            public static void CopyFromArray<T>(this IntPtr pointer, T[] array) where T : struct
            {
                if (pointer == IntPtr.Zero)
                {
                    throw new ArgumentNullException(nameof(pointer), "Pointer is null.");
                }

                if (array == null)
                {
                    throw new ArgumentNullException(nameof(array), "Array is null.");
                }

                if (array.Length == 0)
                {
                    throw new ArgumentException("Array is empty.", nameof(array));
                }

                switch (array)
                {
                    case int[] intArray:
                        Marshal.Copy(intArray, 0, pointer, intArray.Length);
                        break;
                    case double[] doubleArray:
                        Marshal.Copy(doubleArray, 0, pointer, doubleArray.Length);
                        break;
                    case byte[] byteArray:
                        Marshal.Copy(byteArray, 0, pointer, byteArray.Length);
                        break;
                    default:
                        throw new NotSupportedException("Currently only int, double and byte data types are supported.");
                }
            }
        }
    }
}