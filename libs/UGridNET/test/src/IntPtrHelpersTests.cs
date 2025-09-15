using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace UGridNET.Tests
{
    public class IntPtrHelpersBasicTests
    {
        [Test]
        public void AllocateValidTypeReturnsNonZeroPointerInt()
        {
            AllocateValidTypeReturnsNonZeroPointer<int>(10);
        }
        
        [Test]
        public void AllocateValidTypeReturnsNonZeroPointerDouble()
        {
            AllocateValidTypeReturnsNonZeroPointer<double>(10);
        }
        
        [Test]
        public void AllocateValidTypeReturnsNonZeroPointerByte()
        {
            AllocateValidTypeReturnsNonZeroPointer<byte>(10);
        }
        
        private void AllocateValidTypeReturnsNonZeroPointer<T>(int count) where T : struct
        {
            var ptr = IntPtr.Zero;
            Assert.DoesNotThrow(() => ptr = IntPtrHelpers.Allocate<T>(count));
            Assert.That(ptr, Is.Not.EqualTo(IntPtr.Zero));
            IntPtrHelpers.Free(ref ptr);
        }

        [Test]
        public void AllocateZeroedValidTypeReturnsNonZeroPointerInt()
        {
            AllocateZeroedValidTypeReturnsNonZeroPointer<int>(10);
        }
        
        [Test]
        public void AllocateZeroedValidTypeReturnsNonZeroPointerDouble()
        {
            AllocateZeroedValidTypeReturnsNonZeroPointer<double>(10);
        }
        
        [Test]
        public void AllocateZeroedValidTypeReturnsNonZeroPointerByte()
        {
            AllocateZeroedValidTypeReturnsNonZeroPointer<byte>(10);
        }
        
        private void AllocateZeroedValidTypeReturnsNonZeroPointer<T>(int count) where T : struct
        {
            var ptr = IntPtr.Zero;
            Assert.DoesNotThrow(() => ptr = IntPtrHelpers.AllocateZeroed<T>(count));
            Assert.That(ptr, Is.Not.EqualTo(IntPtr.Zero));
            IntPtrHelpers.Free(ref ptr);
        }

        [Test]
        public void AllocateZeroedInitializesMemoryToZeroInt()
        {
            AllocateZeroedInitializesMemoryToZero<int>(5);
        }

        [Test]
        public void AllocateZeroedInitializesMemoryToZeroDouble()
        {
            AllocateZeroedInitializesMemoryToZero<double>(5);
        }

        [Test]
        public void AllocateZeroedInitializesMemoryToZeroByte()
        {
            AllocateZeroedInitializesMemoryToZero<byte>(5);
        }

        private void AllocateZeroedInitializesMemoryToZero<T>(int count) where T : struct
        {
            var ptr = IntPtrHelpers.AllocateZeroed<T>(count);

            var totalBytes = count * Marshal.SizeOf<T>();
            var buffer = new byte[totalBytes];
            Marshal.Copy(ptr, buffer, 0, totalBytes);

            Assert.That(buffer, Is.All.Zero);

            IntPtrHelpers.Free(ref ptr);
        }
    }

    public class IntPtrHelpersClassWithPropertyTests
    {

        private class ClassWithProperty
        {
            public IntPtr Pointer { set; get; }
        }

        [Test]
        public void AllocateAnFreeAClassProperty()
        {
            var classWithProperty = new ClassWithProperty();
            const int count = 10;
            Assert.DoesNotThrow(() => classWithProperty.Pointer = IntPtrHelpers.Allocate<int>(count));
            Assert.That(classWithProperty.Pointer, Is.Not.EqualTo(IntPtr.Zero));
            IntPtrHelpers.Free(() => classWithProperty.Pointer, value => classWithProperty.Pointer = value);
            Assert.That(classWithProperty.Pointer, Is.EqualTo(IntPtr.Zero));
        }

    }
}