using System;
using System.Text;

namespace UGridNET
{
    namespace Extensions
    {
        internal static class ByteArrayExtensions
        {
            public static string GetString(this byte[] bytes)
            {
                if (bytes == null)
                {
                    throw new ArgumentNullException(nameof(bytes), "Byte array cannot be null.");
                }
                // bytes originates from a C null-terminated char*.
                // Ignore the last character in the array then trim the trailing spaces.
                return Encoding.UTF8.GetString(bytes, 0, bytes.Length - 1).TrimEnd();
            }
        }
    }
}