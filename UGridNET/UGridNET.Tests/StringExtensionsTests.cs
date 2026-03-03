using System;
using System.Collections.Generic;
using System.Linq;
using NUnit.Framework;
using UGridNET.Extensions;

namespace UGridNET.Tests
{
    public class StringExtensionsTests
    {
        [Test]
        public void GetBytesThrowsDueToNullString()
        {
            Assert.Throws<ArgumentNullException>(() => ((string)null).GetBytes());
        }

        [Test]
        public void GetBytesSucceeds()
        {
            const string str = "I am not a number, I am a free man!";
            byte[] expectedBytes =
            {
                0x49, 0x20, 0x61, 0x6d, 0x20,
                0x6e, 0x6f, 0x74, 0x20, 0x61,
                0x20, 0x6e, 0x75, 0x6d, 0x62,
                0x65, 0x72, 0x2c, 0x20, 0x49,
                0x20, 0x61, 0x6d, 0x20, 0x61,
                0x20, 0x66, 0x72, 0x65, 0x65,
                0x20, 0x6d, 0x61, 0x6e, 0x21
            };
            byte[] bytes = str.GetBytes();
            Assert.That(bytes, Is.EqualTo(expectedBytes));
        }

        [Test]
        public void GetRightPaddedNullTerminatedBytesThrowsDueToNullStr()
        {
            const int totalLength = 40;
            Assert.Throws<ArgumentNullException>(() => ((string)null).GetRightPaddedNullTerminatedBytes(totalLength));
        }

        [Test]
        public void GetRightPaddedNullTerminatedBytesThrowsDueToNegativeTotalLength()
        {
            const string str = "I am not a number, I am a free man!";
            const int totalLength = -40;
            Assert.Throws<ArgumentOutOfRangeException>(() => str.GetRightPaddedNullTerminatedBytes(totalLength));
        }

        [Test]
        public void GetRightPaddedNullTerminatedBytesThrowsDueToStringLengthExceedingTotalLength()
        {
            const string str = "I am not a number, I am a free man!"; // 35 characters
            const int totalLength = 30;
            Assert.Throws<ArgumentException>(() => str.GetRightPaddedNullTerminatedBytes(totalLength));
        }

        [Test]
        public void GetRightPaddedNullTerminatedBytesSucceeds()
        {
            const string str = "I am not a number, I am a free man!"; // 35 characters
            const int totalLength = 40;
            var expectedBytes = new List<byte>
            {
                0x49, 0x20, 0x61, 0x6d, 0x20,
                0x6e, 0x6f, 0x74, 0x20, 0x61,
                0x20, 0x6e, 0x75, 0x6d, 0x62,
                0x65, 0x72, 0x2c, 0x20, 0x49,
                0x20, 0x61, 0x6d, 0x20, 0x61,
                0x20, 0x66, 0x72, 0x65, 0x65,
                0x20, 0x6d, 0x61, 0x6e, 0x21
            };
            // pad with spaces up to total length -1
            expectedBytes.AddRange(Enumerable.Repeat((byte)0x20, totalLength - expectedBytes.Count - 1));
            // null terminate
            expectedBytes.Add(0x00);

            byte[] bytes = str.GetRightPaddedNullTerminatedBytes(totalLength);
            Assert.That(bytes, Is.EqualTo(expectedBytes.ToArray()));
        }

        [Test]
        public void GetTokenizeThrowsDueToNullStr()
        {
            const int maxTokenLength = 40;
            Assert.Throws<ArgumentNullException>(() => ((string)null).Tokenize(maxTokenLength));
        }

        [Test]
        public void GetTokenizeThrowsDueToEmptyStr()
        {
            const int maxTokenLength = 40;
            var str = string.Empty;
            Assert.Throws<ArgumentException>(() => str.Tokenize(maxTokenLength));
        }

        [Test]
        public void GetTokenizeThrowsDueToNegativeMaximumTokenLength()
        {
            const int maxTokenLength = -40;
            const string str = "I am not a number, I am a free man!";
            Assert.Throws<ArgumentException>(() => str.Tokenize(maxTokenLength));
        }

        [Test]
        public void GetTokenizeSucceeds()
        {
            const int maxTokenLength = 40;
            const string str = "I am not a number, I am a free man!";

            string[] tokens = str.Split(' ');
            string stringBuilder = tokens.Aggregate(string.Empty, (current, token) => current + token.PadRight(maxTokenLength));

            Assert.That(stringBuilder.Tokenize(maxTokenLength), Is.EqualTo(tokens));
        }
    }
}