using System;
using System.Runtime.InteropServices;
using NUnit.Framework;
using UGridNET.Extensions;

namespace UGridNET.Tests
{
    [TestFixture]
    public class IntPtrExtensionsTests
    {
        private const int size = 10;

        private int[] array;
        private IntPtr ptr = IntPtr.Zero;

        [SetUp]
        public void SetUp()
        {
            array = new int[size];
            for (var i = 0; i < size; i++)
            {
                array[i] = i;
            }

            ptr = Marshal.AllocHGlobal(size * Marshal.SizeOf<int>());
        }

        [TearDown]
        public void TearDown()
        {
            Marshal.FreeHGlobal(ptr);
        }

        [Test]
        public void CopyArrayToPtrThrowsDueToNullPointer()
        {
            IntPtr zeroIntPtr = IntPtr.Zero;
            Assert.Throws<ArgumentNullException>(() => zeroIntPtr.CopyFromArray(array));
        }

        [Test]
        public void CopyArrayToPtrThrowsDueToNullArray()
        {
            Assert.Throws<ArgumentNullException>(() => ptr.CopyFromArray<int>(null));
        }

        [Test]
        public void CopyArrayToPtrThrowsDueToEmptyArray()
        {
            var emptyArray = new int[] {};
            Assert.Throws<ArgumentException>(() => ptr.CopyFromArray(emptyArray));
        }

        [Test]
        public void CopyArrayToPtrSucceeds()
        {
            ptr.CopyFromArray(array);
            var result = new int[size];
            for (var i = 0; i < size; i++)
            {
                result[i] = Marshal.ReadInt32(ptr, i * Marshal.SizeOf<int>());
            }

            Assert.That(result, Is.EqualTo(array));
        }

        [Test]
        public void CopyPtrToArrayThrowsDueToNullPointer()
        {
            IntPtr zeroIntPtr = IntPtr.Zero;
            Assert.Throws<ArgumentNullException>(() => zeroIntPtr.CopyToArray<int>(size));
        }

        [Test]
        public void CopyPtrToArrayThrowsDueInvalidArrayLength()
        {
            Assert.Throws<ArgumentException>(() => ptr.CopyToArray<int>(-1));
        }

        [Test]
        public void CopyPtrToArraySucceeds()
        {
            for (var i = 0; i < size; i++)
            {
                Marshal.WriteInt32(ptr, i * Marshal.SizeOf<int>(), i);
            }

            int[] result = ptr.CopyToArray<int>(size);
            Assert.That(result, Is.EqualTo(array));
        }
    }
}