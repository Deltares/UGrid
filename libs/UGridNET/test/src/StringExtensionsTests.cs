using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UGridNET.Extensions;

namespace UGridNET.Tests
{
    public class StringExtensionsTests
    {
        [Test]
        public void GetBytesThrowsDueToNullString()
        {
            string str = null;
            var bytes = new byte[] { };
            Assert.Throws<ArgumentNullException>(() => bytes = str.GetBytes());
        }


        [Test]
        public void GetBytesSucceeds()
        {
            string str = "I am not a number, I am a free man!";
            byte[] expectedBytes = {
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
            string str = null;
            int totalLength = 40;
            var bytes = new byte[] { };
            Assert.Throws<ArgumentNullException>(() => str.GetRightPaddedNullTerminatedBytes(totalLength));
        }

        [Test]
        public void GetRightPaddedNullTerminatedBytesThrowsDueToNegativeTotalLength()
        {
            string str = "I am not a number, I am a free man!";
            int totalLength = -40;
            var bytes = new byte[] { };
            Assert.Throws<ArgumentOutOfRangeException>(() => str.GetRightPaddedNullTerminatedBytes(totalLength));
        }

        [Test]
        public void GetRightPaddedNullTerminatedBytesThrowsDueToStringLengthExceedingTotalLength()
        {
            string str = "I am not a number, I am a free man!"; // 35 characters
            int totalLength = 30;
            var bytes = new byte[] { };
            Assert.Throws<ArgumentException>(() => str.GetRightPaddedNullTerminatedBytes(totalLength));
        }

        [Test]
        public void GetRightPaddedNullTerminatedBytesSucceeds()
        {
            string str = "I am not a number, I am a free man!"; // 35 characters
            int totalLength = 40;
            var expectedBytes = new List<byte> {
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
            int maxTokenLength = 40;
            string str = null;
            List<string> tokens = null;
            Assert.Throws<ArgumentNullException>(() => tokens = str.Tokenize(maxTokenLength));
        }

        [Test]
        public void GetTokenizeThrowsDueToEmptyStr()
        {
            int maxTokenLength = 40;
            var str = string.Empty;
            List<string> tokens = null;
            Assert.Throws<ArgumentException>(() => tokens = str.Tokenize(maxTokenLength));
        }

        [Test]
        public void GetTokenizeThrowsDueToNegativeMAximumTokenLength()
        {
            int maxTokenLength = -40;
            var str = "I am not a number, I am a free man!";
            List<string> tokens = null;
            Assert.Throws<ArgumentException>(() => tokens = str.Tokenize(maxTokenLength));
        }


        [Test]
        public void GetTokenizeSucceeds()
        {
            int maxTokenLength = 40;
            string str = "I am not a number, I am a free man!";
            var tokens = str.Split(' ');
            var stringBuilder = string.Empty;
            foreach (var token in tokens)
            {
                stringBuilder += token.PadRight(maxTokenLength);
            }
            Assert.That(stringBuilder.Tokenize(maxTokenLength), Is.EqualTo(tokens));
        }
    }
}