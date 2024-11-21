using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    namespace Extensions
    {
        public static class IntPtrExtensions
        {
            public static T[] CopyToArray<T>(this IntPtr pointer, int length)
            {
                if (pointer == IntPtr.Zero)
                {
                    throw new InvalidOperationException($"IntPtr {nameof(pointer)} is null. Cannot copy to array.");
                }

                if (length <= 0)
                {
                    throw new ArgumentException("Array length must be greater than zero.", nameof(length));
                }

                var array = new T[length];

                if (typeof(T) == typeof(int))
                {
                    Marshal.Copy(pointer, array as int[], 0, array.Length);
                }
                else if (typeof(T) == typeof(double))
                {
                    Marshal.Copy(pointer, array as double[], 0, array.Length);
                }
                else if (typeof(T) == typeof(byte))
                {
                    Marshal.Copy(pointer, array as byte[], 0, array.Length);
                }
                else
                {
                    throw new NotSupportedException("Currently only int, double and byte data types are supported.");
                }

                return array;
            }

            public static void CopyFromArray<T>(this IntPtr pointer, T[] array)
            {
                if (array == null || array.Length == 0)
                {
                    throw new ArgumentException("Array must not be null or empty.", nameof(array));
                }

                if (pointer == IntPtr.Zero)
                {
                    throw new InvalidOperationException(
                        $"IntPtr {nameof(pointer)} is null. Cannot write array {nameof(array)} to pointer.");
                }

                if (typeof(T) == typeof(int))
                {
                    Marshal.Copy(array as int[], 0, pointer, array.Length);
                }
                else if (typeof(T) == typeof(double))
                {
                    Marshal.Copy(array as double[], 0, pointer, array.Length);
                }
                else if (typeof(T) == typeof(byte))
                {
                    Marshal.Copy(array as byte[], 0, pointer, array.Length);
                }
                else
                {
                    throw new NotSupportedException("Currently only int, double and byte data types are supported.");
                }
            }
        }
    }
}