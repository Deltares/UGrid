using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

[assembly: InternalsVisibleToAttribute("UGridNET.Tests")]

namespace UGridNET
{
    namespace Extensions
    {

        /// <summary>
        /// Provides extension methods for the <see cref="string"/> class.
        /// </summary>
        internal static class StringExtensions
        {
            /// <summary>
            /// Converts the specified string to a byte array using UTF-8 encoding.
            /// </summary>
            /// <param name="str">The string to convert.</param>
            /// <returns>A byte array representing the UTF-8 encoded string.</returns>
            public static byte[] GetBytes(this string str)
            {
                return Encoding.UTF8.GetBytes(str);
            }

            /// <summary>
            /// Converts the specified string to a right-padded, null-terminated byte
            /// array of the specified length using UTF-8 encoding.
            /// </summary>
            /// <param name="str">The string to convert.</param>
            /// <param name="length">The length of the resulting byte array.</param>
            /// <returns>
            /// A byte array of the specified length, containing the UTF-8 encoded string,
            /// right-padded with spaces and null-terminated.
            /// </returns>
            /// <exception cref="ArgumentNullException">Thrown when the string is null.</exception>
            /// <exception cref="ArgumentOutOfRangeException">Thrown when the length is negative.</exception>
            /// <exception cref="ArgumentException">Thrown when the string length exceeds the allowed length minus one for the null terminator.</exception>
            public static byte[] GetRightPaddedNullTerminatedBytes(this string str, int length)
            {
                if (str == null)
                {
                    throw new ArgumentNullException(nameof(str), "String cannot be null.");
                }

                if (length < 0)
                {
                    throw new ArgumentOutOfRangeException(nameof(length), "Length cannot be negative.");
                }

                if (str.Length > length - 1)
                {
                    throw new ArgumentException("String length exceeds the allowed length minus one for the null terminator.", nameof(str));
                }

                // Trim then pad the string, reserve the last char for the null character
                var paddedString = str.Trim().PadRight(length - 1);

                // Convert the string to bytes
                var bytesFromPaddedString = Encoding.UTF8.GetBytes(paddedString);

                // Create a new array with an extra byte for the null terminator
                var bytes = new byte[length];

                // Copy the bytes of the string to the newly created bytes array as the
                Array.Copy(bytesFromPaddedString, bytes, length - 1);

                // Explicitly Set last element to null even though this is not really necessary as
                // the declaration var bytes = new byte[length]; initializes all elements to null.
                bytes[length - 1] = 0x00;

                return bytes;
            }

            /// <summary>Splits the specified string into tokens of the specified maximum length.</summary>
            /// <param name="str">The string to tokenize.</param>
            /// <param name="maxTokenLength">The maximum length of each token.</param>
            /// <returns>A list of tokens.</returns>
            /// <exception cref="ArgumentNullException">Thrown when the string is null.</exception>
            /// <exception cref="ArgumentException">Thrown when the string is empty or the maximum token length is not positive.</exception>
            public static List<string> Tokenize(this string str, int maxTokenLength)
            {
                if (str == null)
                {
                    throw new ArgumentNullException(nameof(str), "String cannot be null.");
                }

                if (str.Length == 0)
                {
                    throw new ArgumentException("String cannot be null.", nameof(str));
                }

                if (maxTokenLength <= 0)
                {
                    throw new ArgumentException("Maximum token length must be strictly positive.", nameof(maxTokenLength));
                }

                var tokens = new List<string>();
                for (int i = 0; i < str.Length; i += maxTokenLength)
                {
                    var token = str.Substring(i, maxTokenLength);
                    tokens.Add(token.Trim());
                }

                return tokens;
            }
        }
    }
}