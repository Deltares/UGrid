using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace UGridNET.Tests
{
    public class IntPtrHelpersBasicTests
    {
        private static IEnumerable<TestCaseData> AllocateTestCases()
        {
            yield return new TestCaseData(typeof(int), 10);
            yield return new TestCaseData(typeof(double), 10);
            yield return new TestCaseData(typeof(byte), 10);
        }

        private IntPtr AllocateHelper(Type type, int count)
        {
            if (type == typeof(int))
            {
                return IntPtrHelpers.Allocate<int>(count);
            }
            else if (type == typeof(double))
            {
                return IntPtrHelpers.Allocate<double>(count);
            }
            else if (type == typeof(byte))
            {
                return IntPtrHelpers.Allocate<byte>(count);
            }
            else
            {
                throw new ArgumentException("Type not covered by test", nameof(type));
            }
        }

        [Test, TestCaseSource(nameof(AllocateTestCases))]
        public void AllocateValidTypeReturnsNonZeroPointer(Type type, int count)
        {
            var ptr = IntPtr.Zero;
            Assert.DoesNotThrow(() => ptr = AllocateHelper(type, count));
            Assert.That(ptr, Is.Not.EqualTo(IntPtr.Zero));
            IntPtrHelpers.Free(ref ptr);
            Assert.That(ptr, Is.EqualTo(IntPtr.Zero));
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