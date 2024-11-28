using System;
using System.Collections.Generic;
using System.Text;

namespace UGridNET
{
    namespace Extensions
    {
        internal static class StringExtensions
        {
            public static byte[] GetBytes(this string str)
            {
                if (str == null)
                {
                    throw new ArgumentNullException(nameof(str), "String cannot be null.");
                }

                return Encoding.UTF8.GetBytes(str);
            }

            public static string PadRightUpTo(this string str, int maxLength)
            {
                if (str == null)
                {
                    throw new ArgumentNullException(nameof(str), "String cannot be null.");
                }

                if (maxLength <= str.Length)
                {
                    return str;
                }

                int paddingLength = maxLength - str.Length;
                string padding = new string(' ', paddingLength);

                return str + padding;
            }

            public static List<string> SplitIntoSizedTokens(this string str, int tokenLength)
            {
                List<string> tokens = new List<string>();
                int currentIndex = 0;

                while (currentIndex < str.Length)
                {
                    // Get the next token of the specified length
                    string token = str.Substring(currentIndex, Math.Min(tokenLength, str.Length - currentIndex));
                    currentIndex += tokenLength;

                    // Trim the token and add to the list if it's not empty
                    token = token.Trim();
                    if (!string.IsNullOrEmpty(token))
                    {
                        tokens.Add(token);
                    }
                }

                return tokens;
            }
        }
    }
}