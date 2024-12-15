using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

[assembly: InternalsVisibleToAttribute("UGridNET.Tests")]

namespace UGridNET
{
    namespace Extensions
    {
        internal static class StringExtensions
        {
            public static byte[] GetBytes(this string str)
            {
                // if (str == null)
                // {
                //     throw new ArgumentNullException(nameof(str), "String cannot be null.");
                // }

                return Encoding.UTF8.GetBytes(str);
            }

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

                List<string> tokens = new List<string>();
                int currentIndex = 0;

                while (currentIndex < str.Length)
                {
                    // Get the next token of the specified length
                    string token = str.Substring(currentIndex, maxTokenLength); //Math.Min(maxTokenLength, str.Length - currentIndex));
                    currentIndex += maxTokenLength;
                    tokens.Add(token.Trim());
                }

                return tokens;
            }

            public static List<List<string>> SplitString(string str, int tokenLength)
            {
                List<List<string>> tokens = new List<List<string>>();
                int currentIndex = 0;

                while (currentIndex < str.Length)
                {
                    // Get the next token of the specified length
                    string token = str.Substring(currentIndex, Math.Min(tokenLength, str.Length - currentIndex));
                    currentIndex += tokenLength;

                    // Split the token into substrings and add to the list
                    string[] substrings = token.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                    tokens.Add(new List<string>(substrings));
                }

                return tokens;
            }
        }
    }
}