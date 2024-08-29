using NUnit.Framework;
using System;
using System.IO;


[TestFixture]
public class UGridNETTests
{
    [Test]
    public void TestMesh1DReadAndInquire()
    {
        // open
        //const string file_path = "C:\\Users\\sayed\\work\\repositories\\UGrid\\tests\\data\\AllUGridEntities.nc";
        const string file_path = "..\\..\\..\\..\\..\\tests\\data\\AllUGridEntities.nc";
        Console.WriteLine(Path.GetFullPath(file_path));

        //const string file_path = "AllUGridEntities.nc";

        int result = -1;

        int file_mode = -1;
        result = UGridCSharpWrapper.ug_file_read_mode(ref file_mode);
        Assert.That(result, Is.EqualTo(0));
        Assert.That(result, Is.EqualTo(0));

        int file_id = -1;
        result = UGridCSharpWrapper.ug_file_open(file_path, file_mode, ref file_id);
        Assert.That(result, Is.EqualTo(0));

        Console.WriteLine("Hello dikkie dik!");
        Console.WriteLine(file_id);
        Console.WriteLine(file_mode);

        // get the number of topologies
        int topology_type = 0;
        result = UGridCSharpWrapper.ug_topology_get_mesh1d_enum(ref topology_type);
        Assert.That(result, Is.EqualTo(0));
        int num_topologies = 0;
        result = UGridCSharpWrapper.ug_topology_get_count(file_id, topology_type, ref num_topologies);
        Assert.That(num_topologies, Is.EqualTo(1));
        Assert.That(result, Is.EqualTo(0));

        // get the mesh
        var mesh1D = new Mesh1D();
        const int topology_id = 0;
        result = UGridCSharpWrapper.ug_mesh1d_inq(file_id, topology_id, mesh1D);
        Assert.That(result, Is.EqualTo(0));
        Assert.That(mesh1D.num_nodes, Is.EqualTo(25));
        Assert.That(mesh1D.num_edges, Is.EqualTo(24));

        int name_length = 0;
        result = UGridCSharpWrapper.ug_name_get_length(ref name_length);
        Assert.That(result, Is.EqualTo(0));
        int long_names_length = 0;
        result = UGridCSharpWrapper.ug_name_get_long_length(ref long_names_length);
        Assert.That(result, Is.EqualTo(0));

        // get the data
        result = UGridCSharpWrapper.ug_mesh1d_get(file_id, 0, mesh1D);
        Assert.That(result, Is.EqualTo(0));
    }
}
