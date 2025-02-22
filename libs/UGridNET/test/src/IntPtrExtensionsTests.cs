using System;
using System.Runtime.InteropServices;
using NUnit.Framework;
using UGridNET.Extensions;

namespace UGridNET.Tests
{

    [TestFixture]
    public class IntPtrExtensionsTests
    {
        private int[] array = null;

        private IntPtr ptr = IntPtr.Zero;
        private readonly int size = 10;
        //private readonly int sizeOfInt = Marshal.SizeOf<int>();

        [SetUp]
        public void SetUp()
        {
            array = new int[size];
            for (int i = 0; i < size; i++)
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
            var zeroIntPtr = IntPtr.Zero;
            Assert.Throws<ArgumentNullException>(() => zeroIntPtr.CopyFromArray<int>(array));
        }

        [Test]
        public void CopyArrayToPtrThrowsDueToNullArray()
        {
            int[] nullArray = null;
            Assert.Throws<ArgumentNullException>(() => ptr.CopyFromArray<int>(nullArray));
        }

        [Test]
        public void CopyArrayToPtrThrowsDueToEmptyArray()
        {
            int[] emptyArray = new int[] { };
            Assert.Throws<ArgumentException>(() => ptr.CopyFromArray<int>(emptyArray));
        }

        [Test]
        public void CopyArrayToPtrSucceeds()
        {
            ptr.CopyFromArray<int>(array);
            int[] result = new int[size];
            for (int i = 0; i < size; i++)
            {
                result[i] = Marshal.ReadInt32(ptr, i * Marshal.SizeOf<int>());
            }
            Assert.That(result, Is.EqualTo(array));
        }

        [Test]
        public void CopyPtrToArrayThrowsDueToNullPointer()
        {
            var zeroIntPtr = IntPtr.Zero;
            int[] result;
            Assert.Throws<ArgumentNullException>(() => result = zeroIntPtr.CopyToArray<int>(size));
        }

        [Test]
        public void CopyPtrToArrayThrowsDueInvalidArrayLength()
        {
            int[] emptyArray = new int[] { };
            int[] result;
            Assert.Throws<ArgumentException>(() => result = ptr.CopyToArray<int>(-1));
        }

        [Test]
        public void CopyPtrToArraySucceeds()
        {
            for (int i = 0; i < size; i++)
            {
                Marshal.WriteInt32(ptr, i * Marshal.SizeOf<int>(), i);
            }
            var result = ptr.CopyToArray<int>(size);
            Assert.That(result, Is.EqualTo(array));
        }
    }
}