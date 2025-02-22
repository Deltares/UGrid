using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UGridNET.Extensions;

namespace UGridNET.Tests
{
    public class ByteArrayExtensionsTests
    {
        [Test]
        public void GetStringFromNullTerminatedArrayThrowsDueToNullBytesArray()
        {
            byte[] bytes = null;
            string str;
            Assert.Throws<ArgumentNullException>(() => str = bytes.GetStringFromNullTerminatedArray());
        }

        [Test]
        public void GetStringFromNullTerminatedArrayThrowsDueToEmptyBytesArray()
        {
            var bytes = new byte[] { };
            string str;
            Assert.Throws<ArgumentException>(() => str = bytes.GetStringFromNullTerminatedArray());
        }

        [Test]
        public void GetStringFromNullTerminatedArrayThrowsDueToBytesArrayNotBeingNullTerminated()
        {
            byte[] bytes = {
                0x42, 0x6c, 0x75, 0x65, 0x20,
                0x73, 0x77, 0x69, 0x74, 0x63,
                0x68, 0x65, 0x73, 0x20, 0x61,
                0x72, 0x65, 0x20, 0x72, 0x61,
                0x64
            };
            string str;
            Assert.Throws<ArgumentException>(() => str = bytes.GetStringFromNullTerminatedArray());
        }

        [Test]
        public void GetStringFromNullTerminatedArraySucceedsWhenTrimmingIsNotRequested()
        {
            byte[] bytes = {
                0x42, 0x6c, 0x75, 0x65, 0x20,
                0x73, 0x77, 0x69, 0x74, 0x63,
                0x68, 0x65, 0x73, 0x20, 0x61,
                0x72, 0x65, 0x20, 0x72, 0x61,
                0x64, 0x20, 0x20, 0x20, 0x00
            };
            string str = bytes.GetStringFromNullTerminatedArray();
            string expectedStr = "Blue switches are rad    ";
            Assert.That(str, Is.EqualTo(expectedStr));
        }

        [Test]
        public void GetStringFromNullTerminatedArraySucceedsWhenTrimmingIsRequested()
        {
            byte[] bytes = {
                0x42, 0x6c, 0x75, 0x65, 0x20,
                0x73, 0x77, 0x69, 0x74, 0x63,
                0x68, 0x65, 0x73, 0x20, 0x61,
                0x72, 0x65, 0x20, 0x72, 0x61,
                0x64, 0x20, 0x20, 0x20, 0x00
            };
            string str = bytes.GetStringFromNullTerminatedArray(true);
            string expectedStr = "Blue switches are rad";
            Assert.That(str, Is.EqualTo(expectedStr));
        }
    }
}