using NUnit.Framework;

namespace UGridNET.Tests
{
    [TestFixture]
    public class UGridTests
    {
        [Test]
        public void TestUgMesh1dGet_ReturnsZero()
        {
            var mesh1D = new Mesh1D();
            int result = UGrid.ug_mesh1d_get(0, 0, mesh1D); // Direct call
            Assert.That(result, Is.EqualTo(0));
        }

        //[Test]
        //public void TestUgMesh2dGet_ReturnsZero()
        //{
        //    var mesh2D = new Mesh2D();
        //    int result = UGrid.ug_mesh2d_get(0, 0, mesh2D); // Direct call
        //    Assert.That(result, Is.EqualTo(0));
        //}
    }
}
