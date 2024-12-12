using NUnit.Framework;
using System;
using System.Runtime.InteropServices;

namespace UGridNET.Tests
{

    [TestFixture]

    public class UGridNETExceptionTests
    {

        [Test]
        public void UGridNETExceptionWithoutParameters()
        {
            try
            {
                throw new UGridNETException();
            }
            catch (UGridNETException exception)
            {
                Assert.That(exception, Is.Not.Null);
                Assert.That(exception.Message, Is.EqualTo("Exception of type 'UGridNET.UGridNETException' was thrown."));
            }
        }

        [Test]
        public void UGridNETExceptionWithString()
        {
            try
            {
                throw new UGridNETException("Test exception");
            }
            catch (UGridNETException exception)
            {
                Assert.That(exception, Is.Not.Null);
                Assert.That(exception.Message, Is.EqualTo("UGridNET error: Test exception"));
            }
        }

        [Test]
        public void UGridNETExceptionWithFormattedString()
        {
            try
            {
                throw new UGridNETException("Test exception {0} {1} {2} {3} {2} {1} {0}", 1, 2.5, 'c', "str");
            }
            catch (UGridNETException exception)
            {
                Assert.That(exception, Is.Not.Null);
                Assert.That(exception.Message, Is.EqualTo("UGridNET error: Test exception 1 2,5 c str c 2,5 1"));
            }
        }

        [Test]
        public void UGridNETExceptionWithStringAndInnerException()
        {
            var exceptionMessage = "UGridNETException exception message.";
            var innerException = new InvalidOperationException("Inner exception of type InvalidOperationException.");

            try
            {
                throw new UGridNETException(exceptionMessage, innerException);
            }
            catch (UGridNETException exception)
            {
                Assert.That(exception, Is.Not.Null);
                Assert.That(exception.Message, Is.EqualTo("UGridNET error: " + exceptionMessage));
                Assert.That(exception.InnerException, Is.EqualTo(innerException));
            }
        }

    }
}