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

                return Encoding.UTF8.GetString(bytes);
            }
        }
    }
}