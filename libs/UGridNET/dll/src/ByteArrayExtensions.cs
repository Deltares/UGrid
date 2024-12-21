using System;
using System.Runtime.CompilerServices;
using System.Text;

[assembly: InternalsVisibleToAttribute("UGridNET.Tests")]

namespace UGridNET
{
    namespace Extensions
    {
        /// <summary>
        /// Provides extension methods for byte arrays.
        /// </summary>
        internal static class ByteArrayExtensions
        {
            /// <summary>  Converts a null-terminated byte array to a string. </summary>
            /// <param name="bytes">The byte array to convert.</param>
            /// <param name="doTrimEnd">Specifies whether to trim the end of the resulting string.</param>
            /// <returns>The converted string.</returns>
            /// <exception cref="ArgumentNullException">Thrown when the byte array is null.</exception>
            /// <exception cref="ArgumentException">Thrown when the byte array is empty or not null-terminated.</exception>
            /// <remarks> If the converted string is not trimmed, a space is appended to it to preserve the original length.</remarks>
            public static string GetStringFromNullTerminatedArray(this byte[] bytes, bool doTrimEnd = false)
            {
                if (bytes == null)
                {
                    throw new ArgumentNullException(nameof(bytes), "Byte array cannot be null.");
                }

                if (bytes.Length == 0)
                {
                    throw new ArgumentException("Byte array cannot be null.", nameof(bytes));
                }

                if (bytes[bytes.Length - 1] != 0x00)
                {
                    throw new ArgumentException("Byte array is not null-terminated.", nameof(bytes));
                }

                string str = Encoding.UTF8.GetString(bytes, 0, bytes.Length - 1);
                return doTrimEnd ? str.TrimEnd() : str + ' ';
            }

        }
    }
}